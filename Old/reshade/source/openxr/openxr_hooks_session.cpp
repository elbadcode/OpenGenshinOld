/*
 * Copyright (C) 2023 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause OR MIT
 */

#include "openxr_hooks.hpp"
#include "openxr_impl_swapchain.hpp"
#include "d3d11/d3d11_device.hpp"
#include "d3d11/d3d11_device_context.hpp"
#include "d3d12/d3d12_device.hpp"
#include "d3d12/d3d12_command_queue.hpp"
#include "opengl/opengl_impl_device_context.hpp"
#include "opengl/opengl_impl_type_convert.hpp"
#include "vulkan/vulkan_hooks.hpp"
#include "vulkan/vulkan_impl_device.hpp"
#include "vulkan/vulkan_impl_command_queue.hpp"
#include "dll_log.hpp"
#include "com_utils.hpp"
#include "hook_manager.hpp"
#include "lockfree_linear_map.hpp"
#include <deque>

#define XR_USE_GRAPHICS_API_D3D11
#define XR_USE_GRAPHICS_API_D3D12
#define XR_USE_GRAPHICS_API_OPENGL
#define XR_USE_GRAPHICS_API_VULKAN
#include <openxr/openxr_platform.h>

struct openxr_session_data
{
	const openxr_dispatch_table *dispatch_table = nullptr;
	reshade::openxr::swapchain_impl *swapchain_impl = nullptr;
};
struct openxr_swapchain_data
{
	const openxr_dispatch_table *dispatch_table = nullptr;
	XrSwapchainCreateFlags create_flags = 0;
	std::vector<reshade::api::resource> surface_images;
	std::deque<uint32_t> acquired_index;
	uint32_t last_released_index = 0;
};

extern lockfree_linear_map<XrInstance, openxr_dispatch_table, 16> g_openxr_instances;
static lockfree_linear_map<XrSession, openxr_session_data, 16> s_openxr_sessions;
static lockfree_linear_map<XrSwapchain, openxr_swapchain_data, 16> s_openxr_swapchains;

#define GET_DISPATCH_PTR_FROM(name, data) \
	assert((data) != nullptr); \
	PFN_xr##name trampoline = (data)->name; \
	assert(trampoline != nullptr)

XrResult XRAPI_CALL xrCreateSession(XrInstance instance, const XrSessionCreateInfo *pCreateInfo, XrSession *pSession)
{
	LOG(INFO) << "Redirecting " << "xrCreateSession" << '(' << "instance = " << instance << ", pCreateInfo = " << pCreateInfo << ", pSession = " << pSession << ')' << " ...";

	assert(pCreateInfo != nullptr && pSession != nullptr);

	const openxr_dispatch_table &dispatch_table = g_openxr_instances.at(instance);
	GET_DISPATCH_PTR_FROM(CreateSession, &dispatch_table);

	const XrResult result = trampoline(instance, pCreateInfo, pSession);
	if (XR_FAILED(result))
	{
		LOG(WARN) << "xrCreateSession" << " failed with error code " << result << '.';
		return result;
	}

	auto enum_view_configurations = dispatch_table.EnumerateViewConfigurations;
	assert(enum_view_configurations != nullptr);

	uint32_t num_view_configurations = 0;
	enum_view_configurations(instance, pCreateInfo->systemId, num_view_configurations, &num_view_configurations, nullptr);
	std::vector<XrViewConfigurationType> view_configurations(num_view_configurations);
	enum_view_configurations(instance, pCreateInfo->systemId, num_view_configurations, &num_view_configurations, view_configurations.data());

	reshade::openxr::swapchain_impl *swapchain_impl = nullptr;

	if (std::find(view_configurations.begin(), view_configurations.end(), XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO) != view_configurations.end())
	{
		// Initialize OpenVR hooks, in case this OpenXR instance is using the SteamVR runtime, so that the VR dashboard overlay can be added
		extern void check_and_init_openvr_hooks();
		check_and_init_openvr_hooks();

		if (const auto binding_d3d11 = find_in_structure_chain<XrGraphicsBindingD3D11KHR>(pCreateInfo, XR_TYPE_GRAPHICS_BINDING_D3D11_KHR))
		{
			if (const auto device_proxy = get_private_pointer_d3dx<D3D11Device>(binding_d3d11->device))
			{
				swapchain_impl = new reshade::openxr::swapchain_impl(device_proxy, device_proxy->_immediate_context, *pSession);
			}
			else
			{
				LOG(WARN) << "Skipping OpenXR session because it was created without a proxy Direct3D 11 device.";
			}
		}
		else
		if (const auto binding_d3d12 = find_in_structure_chain<XrGraphicsBindingD3D12KHR>(pCreateInfo, XR_TYPE_GRAPHICS_BINDING_D3D12_KHR))
		{
			if (const auto device_proxy = get_private_pointer_d3dx<D3D12Device>(binding_d3d12->device))
			{
				if (com_ptr<D3D12CommandQueue> command_queue_proxy;
					SUCCEEDED(binding_d3d12->queue->QueryInterface(&command_queue_proxy)))
				{
					swapchain_impl = new reshade::openxr::swapchain_impl(device_proxy, command_queue_proxy.get(), *pSession);
				}
			}
			else
			{
				LOG(WARN) << "Skipping OpenXR session because it was created without a proxy Direct3D 12 device.";
			}
		}
		else
		if (const auto binding_opengl = find_in_structure_chain<XrGraphicsBindingOpenGLWin32KHR>(pCreateInfo, XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR))
		{
			extern thread_local reshade::opengl::device_context_impl *g_current_context;

			if (g_current_context != nullptr)
			{
				assert(reinterpret_cast<HGLRC>(g_current_context->get_native()) == binding_opengl->hGLRC);

				swapchain_impl = new reshade::openxr::swapchain_impl(g_current_context->get_device(), g_current_context, *pSession);
			}
		}
		else
		if (const auto binding_vulkan = find_in_structure_chain<XrGraphicsBindingVulkanKHR>(pCreateInfo, XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR))
		{
			extern lockfree_linear_map<void *, reshade::vulkan::device_impl *, 8> g_vulkan_devices;

			if (reshade::vulkan::device_impl *const device = g_vulkan_devices.at(dispatch_key_from_handle(binding_vulkan->device)))
			{
				VkQueue queue_handle = VK_NULL_HANDLE;
				device->_dispatch_table.GetDeviceQueue(binding_vulkan->device, binding_vulkan->queueFamilyIndex, binding_vulkan->queueIndex, &queue_handle);

				reshade::vulkan::command_queue_impl *queue = nullptr;
				if (const auto queue_it = std::find_if(device->_queues.cbegin(), device->_queues.cend(),
						[queue_handle](reshade::vulkan::command_queue_impl *queue) { return queue->_orig == queue_handle; });
					queue_it != device->_queues.cend())
					queue = *queue_it;

				if (queue != nullptr)
				{
					swapchain_impl = new reshade::openxr::swapchain_impl(device, queue, *pSession);
				}
			}
			else
			{
				LOG(WARN) << "Skipping OpenXR session because it was created without a known Vulkan device.";
			}
		}
	}
	else
	{
		LOG(WARN) << "Skipping OpenXR session because the system does not support the stereo view configuration.";
	}

	s_openxr_sessions.emplace(*pSession, openxr_session_data { &dispatch_table, swapchain_impl });

#if RESHADE_VERBOSE_LOG
	LOG(DEBUG) << "Returning OpenXR session " << *pSession << '.';
#endif
	return result;
}

XrResult XRAPI_CALL xrDestroySession(XrSession session)
{
	LOG(INFO) << "Redirecting " << "xrDestroySession" << '(' << "session = " << session << ')' << " ...";

	assert(session != XR_NULL_HANDLE);

	const openxr_session_data &data = s_openxr_sessions.at(session);
	GET_DISPATCH_PTR_FROM(DestroySession, data.dispatch_table);

	delete data.swapchain_impl;

	s_openxr_sessions.erase(session);

	return trampoline(session);
}

XrResult XRAPI_CALL xrCreateSwapchain(XrSession session, const XrSwapchainCreateInfo *pCreateInfo, XrSwapchain *pSwapchain)
{
	LOG(INFO) << "Redirecting " << "xrCreateSwapchain" << '(' << "session = " << session << ", pCreateInfo = " << pCreateInfo << ", pSwapchain = " << pSwapchain << ')' << " ...";

	const openxr_session_data &data = s_openxr_sessions.at(session);
	GET_DISPATCH_PTR_FROM(CreateSwapchain, data.dispatch_table);

	assert(pCreateInfo != nullptr && pSwapchain != nullptr);

	XrSwapchainCreateInfo create_info = *pCreateInfo;
	// Add required usage flags to create info
	create_info.usageFlags |= XR_SWAPCHAIN_USAGE_TRANSFER_SRC_BIT | XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;

	const XrResult result = trampoline(session, &create_info, pSwapchain);
	if (XR_FAILED(result))
	{
		LOG(WARN) << "xrCreateSwapchain" << " failed with error code " << result << '.';
		return result;
	}

	std::vector<reshade::api::resource> images;

	if (data.swapchain_impl != nullptr)
	{
		auto enum_swapchain_images = data.dispatch_table->EnumerateSwapchainImages;
		assert(enum_swapchain_images != nullptr);

		uint32_t num_images = 0;
		enum_swapchain_images(*pSwapchain, num_images, &num_images, nullptr);
		images.reserve(num_images);

		switch (data.swapchain_impl->get_device()->get_api())
		{
		case reshade::api::device_api::d3d11:
		{
			std::vector<XrSwapchainImageD3D11KHR> images_d3d11(num_images, { XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR });
			enum_swapchain_images(*pSwapchain, num_images, &num_images, reinterpret_cast<XrSwapchainImageBaseHeader *>(images_d3d11.data()));
			for (const XrSwapchainImageD3D11KHR &image_d3d11 : images_d3d11)
				images.push_back({ reinterpret_cast<uintptr_t>(image_d3d11.texture) });
			break;
		}
		case reshade::api::device_api::d3d12:
		{
			std::vector<XrSwapchainImageD3D12KHR> images_d3d12(num_images, { XR_TYPE_SWAPCHAIN_IMAGE_D3D12_KHR });
			enum_swapchain_images(*pSwapchain, num_images, &num_images, reinterpret_cast<XrSwapchainImageBaseHeader *>(images_d3d12.data()));
			for (const XrSwapchainImageD3D12KHR &image_d3d12 : images_d3d12)
				images.push_back({ reinterpret_cast<uintptr_t>(image_d3d12.texture) });
			break;
		}
		case reshade::api::device_api::opengl:
		{
			std::vector<XrSwapchainImageOpenGLKHR> images_opengl(num_images, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR });
			enum_swapchain_images(*pSwapchain, num_images, &num_images, reinterpret_cast<XrSwapchainImageBaseHeader *>(images_opengl.data()));
			for (const XrSwapchainImageOpenGLKHR &image_opengl : images_opengl)
				images.push_back(reshade::opengl::make_resource_handle(GL_TEXTURE_2D, image_opengl.image));
			break;
		}
		case reshade::api::device_api::vulkan:
		{
			std::vector<XrSwapchainImageVulkanKHR> images_vulkan(num_images, { XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR });
			enum_swapchain_images(*pSwapchain, num_images, &num_images, reinterpret_cast<XrSwapchainImageBaseHeader *>(images_vulkan.data()));
			for (const XrSwapchainImageVulkanKHR &image_vulkan : images_vulkan)
				images.push_back({ (uint64_t)image_vulkan.image });
			break;
		}
		}
	}

	s_openxr_swapchains.emplace(*pSwapchain, openxr_swapchain_data { data.dispatch_table, create_info.createFlags, std::move(images) });

#if RESHADE_VERBOSE_LOG
	LOG(DEBUG) << "Returning OpenXR swap chain " << *pSwapchain << '.';
#endif
	return result;
}

XrResult XRAPI_CALL xrDestroySwapchain(XrSwapchain swapchain)
{
	LOG(INFO) << "Redirecting " << "xrDestroySwapchain" << '(' << "swapchain = " << swapchain << ')' << " ...";

	const openxr_swapchain_data &data = s_openxr_swapchains.at(swapchain);
	GET_DISPATCH_PTR_FROM(DestroySwapchain, data.dispatch_table);

	s_openxr_swapchains.erase(swapchain);

	return trampoline(swapchain);
}

XrResult XRAPI_CALL xrAcquireSwapchainImage(XrSwapchain swapchain, const XrSwapchainImageAcquireInfo *pAcquireInfo, uint32_t *pIndex)
{
	openxr_swapchain_data &data = s_openxr_swapchains.at(swapchain);
	GET_DISPATCH_PTR_FROM(AcquireSwapchainImage, data.dispatch_table);

	const XrResult result = trampoline(swapchain, pAcquireInfo, pIndex);
	if (XR_FAILED(result))
		return result;

	data.acquired_index.push_back(*pIndex);

	return result;
}

XrResult XRAPI_CALL xrReleaseSwapchainImage(XrSwapchain swapchain, const XrSwapchainImageReleaseInfo *pReleaseInfo)
{
	openxr_swapchain_data &data = s_openxr_swapchains.at(swapchain);
	GET_DISPATCH_PTR_FROM(ReleaseSwapchainImage, data.dispatch_table);

	const XrResult result = trampoline(swapchain, pReleaseInfo);
	if (XR_FAILED(result))
		return result;

	// 'xrReleaseSwapchainImage' releases the last image from 'xrWaitSwapchainImage', which will implicitly wait on the oldest acquired swapchain image which has not yet been successfully waited on
	data.last_released_index = data.acquired_index.front();
	data.acquired_index.pop_front();

	return result;
}

XrResult XRAPI_CALL xrEndFrame(XrSession session, const XrFrameEndInfo *frameEndInfo)
{
	const openxr_session_data &data = s_openxr_sessions.at(session);

	if (data.swapchain_impl != nullptr)
	{
		reshade::api::resource left_texture = {};
		reshade::api::rect left_rect;
		uint32_t left_layer = 0;
		reshade::api::resource right_texture = {};
		reshade::api::rect right_rect;
		uint32_t right_layer = 0;

		for (uint32_t i = 0; i < frameEndInfo->layerCount; ++i)
		{
			if (frameEndInfo->layers[i]->type == XR_TYPE_COMPOSITION_LAYER_PROJECTION)
			{
				const XrCompositionLayerProjection *const layer = reinterpret_cast<const XrCompositionLayerProjection *>(frameEndInfo->layers[i]);

				if (layer->viewCount != 2)
					continue; // Only support stereo views

				XrSwapchainSubImage const &left_sub_image = layer->views[0].subImage;
				XrSwapchainSubImage const &right_sub_image = layer->views[1].subImage;

				const openxr_swapchain_data &left_data = s_openxr_swapchains.at(left_sub_image.swapchain);
				const openxr_swapchain_data &right_data = s_openxr_swapchains.at(right_sub_image.swapchain);

				if (left_data.create_flags & XR_SWAPCHAIN_CREATE_STATIC_IMAGE_BIT || right_data.create_flags & XR_SWAPCHAIN_CREATE_STATIC_IMAGE_BIT)
					continue; // Cannot apply effects to a static image, since it would just stack on top of the previous result every frame

				left_texture = left_data.surface_images[left_data.last_released_index];
				left_rect.left = left_sub_image.imageRect.offset.x;
				left_rect.top = left_sub_image.imageRect.offset.y;
				left_rect.right = left_sub_image.imageRect.offset.x + left_sub_image.imageRect.extent.width;
				left_rect.bottom = left_sub_image.imageRect.offset.y + left_sub_image.imageRect.extent.height;
				left_layer = left_sub_image.imageArrayIndex;

				right_texture = right_data.surface_images[right_data.last_released_index];
				right_rect.left = right_sub_image.imageRect.offset.x;
				right_rect.top = right_sub_image.imageRect.offset.y;
				right_rect.right = right_sub_image.imageRect.offset.x + right_sub_image.imageRect.extent.width;
				right_rect.bottom = right_sub_image.imageRect.offset.y + right_sub_image.imageRect.extent.height;
				right_layer = right_sub_image.imageArrayIndex;
				break;
			}
		}

		if (left_texture != 0 && right_texture != 0)
			data.swapchain_impl->on_present(left_texture, left_rect, left_layer, right_texture, right_rect, right_layer);
	}

	GET_DISPATCH_PTR_FROM(EndFrame, data.dispatch_table);
	return trampoline(session, frameEndInfo);
}
