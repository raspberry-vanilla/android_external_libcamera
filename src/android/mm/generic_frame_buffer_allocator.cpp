/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021, Google Inc.
 *
 * Allocate FrameBuffer using gralloc API
 */

#include <dlfcn.h>
#include <memory>
#include <vector>

#include <libcamera/base/log.h>
#include <libcamera/base/shared_fd.h>

#include "libcamera/internal/formats.h"
#include "libcamera/internal/framebuffer.h"

#include <hardware/camera3.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wextra-semi"
#include <ui/GraphicBufferAllocator.h>
#pragma GCC diagnostic pop
#include <utils/Errors.h>

#include "../camera_device.h"
#include "../frame_buffer_allocator.h"
#include "../hal_framebuffer.h"

using namespace libcamera;

LOG_DECLARE_CATEGORY(HAL)

namespace {
class GenericFrameBufferData : public FrameBuffer::Private
{
	LIBCAMERA_DECLARE_PUBLIC(FrameBuffer)

public:
	GenericFrameBufferData(android::GraphicBufferAllocator &allocDevice,
			       buffer_handle_t handle,
			       const std::vector<FrameBuffer::Plane> &planes)
		: FrameBuffer::Private(planes), allocDevice_(allocDevice),
		  handle_(handle)
	{
		ASSERT(handle_);
	}

	~GenericFrameBufferData() override
	{
		/*
		 * \todo Thread safety against alloc_device_t is not documented.
		 * Is it no problem to call alloc/free in parallel?
		 */
		android::status_t status = allocDevice_.free(handle_);
		if (status != android::NO_ERROR)
			LOG(HAL, Error) << "Error freeing framebuffer: " << status;
	}

private:
	android::GraphicBufferAllocator &allocDevice_;
	const buffer_handle_t handle_;
};
} /* namespace */

class PlatformFrameBufferAllocator::Private : public Extensible::Private
{
	LIBCAMERA_DECLARE_PUBLIC(PlatformFrameBufferAllocator)

public:
	Private(CameraDevice *const cameraDevice)
		: cameraDevice_(cameraDevice),
		  allocDevice_(android::GraphicBufferAllocator::get())
	{
	}

	~Private() = default;

	std::unique_ptr<HALFrameBuffer>
	allocate(int halPixelFormat, const libcamera::Size &size, uint32_t usage);

private:
	const CameraDevice *const cameraDevice_;
	android::GraphicBufferAllocator &allocDevice_;
};

std::unique_ptr<HALFrameBuffer>
PlatformFrameBufferAllocator::Private::allocate(int halPixelFormat,
						const libcamera::Size &size,
						uint32_t usage)
{
	uint32_t stride = 0;
	buffer_handle_t handle = nullptr;

	android::status_t status = allocDevice_.allocate(size.width, size.height, halPixelFormat,
							 1 /*layerCount*/, usage, &handle, &stride,
							 "libcameraHAL");
	if (status != android::NO_ERROR) {
		LOG(HAL, Error) << "failed buffer allocation: " << status;
		return nullptr;
	}
	if (!handle) {
		LOG(HAL, Fatal) << "invalid buffer_handle_t";
		return nullptr;
	}

	/* This code assumes the planes are mapped consecutively. */
	const libcamera::PixelFormat pixelFormat =
		cameraDevice_->capabilities()->toPixelFormat(halPixelFormat);
	const auto &info = PixelFormatInfo::info(pixelFormat);
	std::vector<FrameBuffer::Plane> planes(info.numPlanes());

	SharedFD fd{ handle->data[0] };
	size_t offset = 0;
	for (auto [i, plane] : utils::enumerate(planes)) {
		const size_t planeSize = info.planeSize(size.height, i, stride);

		plane.fd = fd;
		plane.offset = offset;
		plane.length = planeSize;
		offset += planeSize;
	}

	return std::make_unique<HALFrameBuffer>(
		std::make_unique<GenericFrameBufferData>(
			allocDevice_, handle, planes),
		handle);
}

PUBLIC_FRAME_BUFFER_ALLOCATOR_IMPLEMENTATION
