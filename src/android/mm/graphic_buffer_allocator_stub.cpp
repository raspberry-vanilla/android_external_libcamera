/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2023, Ideas on Board
 * Copyright (C) 2023, BayLibre
 *
 * graphic_buffer_allocator_stub.cpp - Android GraphicBufferAllocator
 * stub for compile-testing
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wextra-semi"
#include <ui/GraphicBufferAllocator.h>
#pragma GCC diagnostic pop

namespace android {

ANDROID_SINGLETON_STATIC_INSTANCE(GraphicBufferAllocator)

GraphicBufferAllocator::GraphicBufferAllocator()
{
}
GraphicBufferAllocator::~GraphicBufferAllocator()
{
}

uint64_t GraphicBufferAllocator::getTotalSize() const
{
	return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
status_t GraphicBufferAllocator::allocate(uint32_t width,
					  uint32_t height,
					  PixelFormat format,
					  uint32_t layerCount,
					  uint64_t usage,
					  buffer_handle_t *handle,
					  uint32_t *stride,
					  std::string requestorName)
{
	*handle = nullptr;
	*stride = 0;
	return INVALID_OPERATION;
}

status_t GraphicBufferAllocator::free(buffer_handle_t handle)
{
	return INVALID_OPERATION;
}
#pragma GCC diagnostic pop

} // namespace android
