/*
 * Copyright (C) 2014 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <GL/gl3w.h>
#include "reshade_api_object_impl.hpp"

namespace reshade::opengl
{
	class device_impl;

	class swapchain_impl : public api::api_object_impl<HDC, api::swapchain>
	{
	public:
		swapchain_impl(device_impl *device, HDC hdc);

		api::device *get_device() final;

		void *get_hwnd() const final;

		api::resource get_back_buffer(uint32_t index = 0) final;

		uint32_t get_back_buffer_count() const final { return 1; }
		uint32_t get_current_back_buffer_index() const final { return 0; }

		bool check_color_space_support(api::color_space color_space) const final { return color_space == api::color_space::srgb_nonlinear || color_space == api::color_space::extended_srgb_linear; }

		api::color_space get_color_space() const final { return api::color_space::unknown; }

	private:
		device_impl *const _device_impl;
	};
}
