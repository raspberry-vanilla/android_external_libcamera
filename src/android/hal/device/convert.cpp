/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "LibcameraDevice"
#include <log/log.h>

#include "convert.h"

#include <aidl/android/hardware/graphics/common/BufferUsage.h>
#include <aidl/android/hardware/graphics/common/PixelFormat.h>
#include <hardware/camera_common.h>

namespace android {
namespace hardware {
namespace camera {
namespace device {
namespace implementation {

using ::aidl::android::hardware::camera::device::ErrorCode;
using ::aidl::android::hardware::camera::device::ErrorMsg;
using ::aidl::android::hardware::camera::device::ShutterMsg;
using ::aidl::android::hardware::graphics::common::BufferUsage;
using ::aidl::android::hardware::graphics::common::PixelFormat;

Status getAidlStatus(int status) {
    switch (status) {
        case 0: return Status::OK;
        case -ENOSYS: return Status::OPERATION_NOT_SUPPORTED;
        case -EBUSY : return Status::CAMERA_IN_USE;
        case -EUSERS: return Status::MAX_CAMERAS_IN_USE;
        case -ENODEV: return Status::INTERNAL_ERROR;
        case -EINVAL: return Status::ILLEGAL_ARGUMENT;
        default:
            ALOGE("%s: unknown HAL status code %d", __FUNCTION__, status);
            return Status::INTERNAL_ERROR;
    }
}

void convertToAidl(const camera_metadata_t* src, CameraMetadata* dest) {
    if (src == nullptr) {
        return;
    }

    size_t size = get_camera_metadata_size(src);
    auto* src_start = (uint8_t*)src;
    uint8_t* src_end = src_start + size;
    dest->metadata.assign(src_start, src_end);
}

bool convertFromAidl(const CameraMetadata& src, const camera_metadata_t** dst) {
    const std::vector<uint8_t>& metadata = src.metadata;
    if (metadata.empty()) {
        // Special case for null metadata
        *dst = nullptr;
        return true;
    }

    const uint8_t* data = metadata.data();
    // check that the size of CameraMetadata match underlying camera_metadata_t
    if (get_camera_metadata_size((camera_metadata_t*)data) != metadata.size()) {
        ALOGE("%s: input CameraMetadata is corrupt!", __FUNCTION__);
        return false;
    }
    *dst = (camera_metadata_t*)data;
    return true;
}

void convertFromAidl(const Stream& src, camera_stream_t* dst) {
    dst->stream_type = static_cast<int>(src.streamType);
    dst->width = src.width;
    dst->height = src.height;
    dst->format = static_cast<int>(src.format);
    dst->data_space = static_cast<android_dataspace_t>(src.dataSpace);
    dst->usage = static_cast<uint32_t>(src.usage);
    dst->physical_camera_id = src.physicalCameraId.c_str();
    dst->rotation = static_cast<int>(src.rotation);
}

void convertFromAidl(const Stream &src, Camera3Stream* dst) {
    dst->mId = src.id;
    dst->stream_type = static_cast<int>(src.streamType);
    dst->width = src.width;
    dst->height = src.height;
    dst->format = static_cast<int>(src.format);
    dst->data_space = static_cast<android_dataspace_t>(src.dataSpace);
    dst->usage = static_cast<uint32_t>(src.usage);
    dst->physical_camera_id = src.physicalCameraId.c_str();
    dst->rotation = static_cast<int>(src.rotation);
    // Fields to be filled by HAL (max_buffers, priv) are initialized to 0
    dst->max_buffers = 0;
    dst->priv = 0;
}

void convertToAidl(const Camera3Stream* src, HalStream* dst) {
    dst->id = src->mId;
    dst->overrideFormat = (PixelFormat) src->format;
    dst->maxBuffers = src->max_buffers;
    if (src->stream_type == CAMERA3_STREAM_OUTPUT) {
        dst->consumerUsage = (BufferUsage)0;
        dst->producerUsage = (BufferUsage)src->usage;
    } else if (src->stream_type == CAMERA3_STREAM_INPUT) {
        dst->producerUsage = (BufferUsage)0;
        dst->consumerUsage = (BufferUsage)src->usage;
    } else {
        //Should not reach here per current HIDL spec, but we might end up adding
        // bi-directional stream to HIDL.
        ALOGW("%s: Stream type %d is not currently supported!",
                __FUNCTION__, src->stream_type);
    }
}

void convertFromAidl(
        buffer_handle_t* bufPtr, BufferStatus status, camera3_stream_t* stream, int acquireFence,
        camera3_stream_buffer_t* dst) {
    dst->stream = stream;
    dst->buffer = bufPtr;
    dst->status = (int) status;
    dst->acquire_fence = acquireFence;
    dst->release_fence = -1; // meant for HAL to fill in
}

void convertToAidl(const camera3_notify_msg* src, NotifyMsg* dst) {
    switch (src->type) {
        case CAMERA3_MSG_ERROR:
            {
                // The camera3_stream_t* must be the same as what wrapper HAL passed to conventional
                // HAL, or the ID lookup will return garbage. Caller should validate the ID here is
                // indeed one of active stream IDs
                Camera3Stream* stream = static_cast<Camera3Stream*>(
                        src->message.error.error_stream);
                aidl::android::hardware::camera::device::ErrorMsg errorMsg = {
                    .frameNumber = static_cast<int32_t>(src->message.error.frame_number),
                    .errorStreamId = (stream != nullptr) ? stream->mId : -1,
                    .errorCode = (ErrorCode) src->message.error.error_code};
                dst->set<NotifyMsg::Tag::error>(errorMsg);
            }
            break;
        case CAMERA3_MSG_SHUTTER:
            {
                aidl::android::hardware::camera::device::ShutterMsg shutterMsg = {
                        .frameNumber = static_cast<int32_t>(src->message.shutter.frame_number),
                        .timestamp = static_cast<int64_t>(src->message.shutter.timestamp)};
                dst->set<NotifyMsg::Tag::shutter>(shutterMsg);
            }
            break;
        default:
            ALOGE("%s: AIDL type converion failed. Unknown msg type 0x%x",
                    __func__, src->type);
    }
}

}  // namespace implementation
}  // namespace device
}  // namespace camera
}  // namespace hardware
}  // namespace android
