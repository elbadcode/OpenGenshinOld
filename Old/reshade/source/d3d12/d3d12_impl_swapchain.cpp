/*
 * Copyright (C) 2014 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "d3d12_impl_device.hpp"
#include "d3d12_impl_swapchain.hpp"
#include "d3d12_impl_type_convert.hpp"
#include <CoreWindow.h>

reshade::d3d12::swapchain_impl::swapchain_impl(device_impl *device, IDXGISwapChain3 *swapchain) :
	api_object_impl(swapchain),
	_device_impl(device)
{
#ifndef NDEBUG
	DXGI_SWAP_CHAIN_DESC swap_desc = {};
	_orig->GetDesc(&swap_desc);

	assert(swap_desc.BufferUsage & DXGI_USAGE_RENDER_TARGET_OUTPUT);
#endif
}

reshade::api::device *reshade::d3d12::swapchain_impl::get_device()
{
	return _device_impl;
}

void *reshade::d3d12::swapchain_impl::get_hwnd() const
{
	if (HWND hwnd = nullptr;
		SUCCEEDED(_orig->GetHwnd(&hwnd)))
		return hwnd;
	else if (com_ptr<ICoreWindowInterop> window_interop; // Get window handle of the core window
		SUCCEEDED(_orig->GetCoreWindow(IID_PPV_ARGS(&window_interop))) && SUCCEEDED(window_interop->get_WindowHandle(&hwnd)))
		return hwnd;

	DXGI_SWAP_CHAIN_DESC swap_desc = {};
	_orig->GetDesc(&swap_desc);

	return swap_desc.OutputWindow;
}

reshade::api::resource reshade::d3d12::swapchain_impl::get_back_buffer(uint32_t index)
{
	com_ptr<ID3D12Resource> back_buffer;
	_orig->GetBuffer(index, IID_PPV_ARGS(&back_buffer));
	assert(back_buffer != nullptr);

	return to_handle(back_buffer.get());
}

uint32_t reshade::d3d12::swapchain_impl::get_back_buffer_count() const
{
	DXGI_SWAP_CHAIN_DESC swap_desc = {};
	_orig->GetDesc(&swap_desc);

	return swap_desc.BufferCount;
}

uint32_t reshade::d3d12::swapchain_impl::get_current_back_buffer_index() const
{
	return _orig->GetCurrentBackBufferIndex();
}

bool reshade::d3d12::swapchain_impl::check_color_space_support(api::color_space color_space) const
{
	if (color_space == api::color_space::unknown)
		return false;

	UINT support;
	return SUCCEEDED(_orig->CheckColorSpaceSupport(convert_color_space(color_space), &support)) && (support & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) != 0;
}

reshade::api::color_space reshade::d3d12::swapchain_impl::get_color_space() const
{
	if (_color_space == DXGI_COLOR_SPACE_CUSTOM)
		return api::color_space::unknown;

	return convert_color_space(_color_space);
}

reshade::d3d12::swapchain_d3d12on7_impl::swapchain_d3d12on7_impl(device_impl *device, ID3D12CommandQueueDownlevel *command_queue_downlevel) :
	api_object_impl(command_queue_downlevel),
	_device_impl(device)
{
	// Default to three back buffers for d3d12on7
	_back_buffers.resize(3);
}

reshade::api::device *reshade::d3d12::swapchain_d3d12on7_impl::get_device()
{
	return _device_impl;
}

void *reshade::d3d12::swapchain_d3d12on7_impl::get_hwnd() const
{
	return _hwnd;
}

reshade::api::resource reshade::d3d12::swapchain_d3d12on7_impl::get_back_buffer(uint32_t index)
{
	return to_handle(_back_buffers[index].get());
}

uint32_t reshade::d3d12::swapchain_d3d12on7_impl::get_back_buffer_count() const
{
	return static_cast<uint32_t>(_back_buffers.size());
}

uint32_t reshade::d3d12::swapchain_d3d12on7_impl::get_current_back_buffer_index() const
{
	return _swap_index;
}
