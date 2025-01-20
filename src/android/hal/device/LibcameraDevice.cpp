/*
 * Copyright (C) 2025 BayLibre SAS
 * Author: Mattijs Korpershoek <mkorpershoek@baylibre.com>
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
// #define LOG_NDEBUG 0
#include <log/log.h>
#include <utils/Trace.h>

#include "LibcameraDevice.h"
#include "convert.h"

#include <aidl/android/hardware/camera/common/Status.h>
#include <regex>
#include <set>

namespace android {
namespace hardware {
namespace camera {
namespace device {
namespace implementation {

using ::aidl::android::hardware::camera::common::Status;
using ::android::hardware::camera::device::implementation::fromStatus;

LibcameraDevice::LibcameraDevice(
        sp<CameraModule> module, const std::string& cameraId,
        const SortedVector<std::pair<std::string, std::string>>& cameraDeviceNames)
    : mModule(module), mCameraId(cameraId) {
    mCameraIdInt = atoi(mCameraId.c_str());
    // Should not reach here as provider also validate ID
    if (mCameraIdInt < 0) {
        ALOGE("%s: Invalid camera id: %s", __FUNCTION__, mCameraId.c_str());
        mInitFail = true;
    } else if (mCameraIdInt >= mModule->getNumberOfCameras()) {
        ALOGI("%s: Adding a new camera id: %s", __FUNCTION__, mCameraId.c_str());
    }

    mDeviceVersion = mModule->getDeviceVersion(mCameraIdInt);
    if (mDeviceVersion < CAMERA_DEVICE_API_VERSION_3_2) {
        ALOGE("%s: Camera id %s does not support HAL3.2+",
                __FUNCTION__, mCameraId.c_str());
        mInitFail = true;
    }
}

LibcameraDevice::~LibcameraDevice() = default;

Status LibcameraDevice::initStatus() const {
    Mutex::Autolock _l(mLock);
    Status status = Status::OK;
    if (mInitFail) {
        status = Status::INTERNAL_ERROR;
    }
    return status;
}

ndk::ScopedAStatus LibcameraDevice::getCameraCharacteristics(CameraMetadata* _aidl_return) {
    if (_aidl_return == nullptr)
        return fromStatus(Status::ILLEGAL_ARGUMENT);

    Status status = initStatus();
    if (status != Status::OK) {
        ALOGE("%s: Invalid device status: %s", __FUNCTION__, toString(status).c_str());
        _aidl_return = nullptr;
        return fromStatus(status);
    }

    struct camera_info info;
    int ret = mModule->getCameraInfo(mCameraIdInt, &info);
    if (ret != OK) {
        ALOGE("%s: Could not retrieve camera info: %d", __FUNCTION__, ret);
        return fromStatus(getAidlStatus(ret));
    }
    convertToAidl(info.static_camera_characteristics, _aidl_return);
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraDevice::getPhysicalCameraCharacteristics(const std::string& in_physicalCameraId,
                                                                     CameraMetadata* _aidl_return) {
    Status status = initStatus();
    if (status != Status::OK) {
        ALOGE("%s: Invalid device status: %s", __FUNCTION__, toString(status).c_str());
        _aidl_return = nullptr;
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    // Require module 2.5+ version.
    if (mModule->getModuleApiVersion() < CAMERA_MODULE_API_VERSION_2_5) {
        ALOGE("%s: get_physical_camera_info() must be called "
              "on camera module 2.5 or newer", __FUNCTION__);
        _aidl_return = nullptr;
        return fromStatus(Status::INTERNAL_ERROR);
    }

    int id = atoi(in_physicalCameraId.c_str());
    camera_metadata_t* physicalInfo = nullptr;
    int ret = mModule->getPhysicalCameraInfo(id, &physicalInfo);
    if (ret != OK) {
        ALOGE("%s: Could not retrieve physical camera info: %d", __FUNCTION__, ret);
        _aidl_return = nullptr;
        return fromStatus(getAidlStatus(ret));
    }

    convertToAidl(physicalInfo, _aidl_return);
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraDevice::getResourceCost(CameraResourceCost* _aidl_return) {
    if (_aidl_return == nullptr)
        return fromStatus(Status::ILLEGAL_ARGUMENT);

    struct camera_info info;
    int ret = mModule->getCameraInfo(mCameraIdInt, &info);
    if (ret != OK) {
        ALOGE("%s: Could not retrieve camera info: %d", __FUNCTION__, ret);
        return fromStatus(getAidlStatus(ret));
    }

    _aidl_return->resourceCost = info.resource_cost;
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraDevice::isStreamCombinationSupported(const StreamConfiguration& in_streams,
                                                                 bool* _aidl_return) {
    // Require module 2.5+ version.
    if (mModule->getModuleApiVersion() < CAMERA_MODULE_API_VERSION_2_5) {
        ALOGE("%s: is_stream_combination_supported() must be called "
              "on camera module 2.5 or newer", __FUNCTION__);
        _aidl_return = nullptr;
        return fromStatus(Status::INTERNAL_ERROR);
    }

    camera_stream_combination_t streamComb{};
    streamComb.operation_mode = static_cast<uint32_t> (in_streams.operationMode);
    streamComb.num_streams = in_streams.streams.size();
    camera_stream_t *streamBuffer  = new camera_stream_t[streamComb.num_streams];

    size_t i = 0;
    for (const auto &it : in_streams.streams) {
        convertFromAidl(it, &streamBuffer[i]);
        i++;
    }
    streamComb.streams = streamBuffer;
    auto res = mModule->isStreamCombinationSupported(mCameraIdInt, &streamComb);
    Status status;
    switch (res) {
        case NO_ERROR:
            *_aidl_return = true;
            status = Status::OK;
            break;
        case BAD_VALUE:
            status = Status::OK;
            *_aidl_return = false;
            break;
        case INVALID_OPERATION:
            status = Status::OPERATION_NOT_SUPPORTED;
            *_aidl_return = false;
            break;
        default:
            ALOGE("%s: Unexpected error: %d", __FUNCTION__, res);
            status = Status::INTERNAL_ERROR;
            _aidl_return = nullptr;
    };
    delete [] streamBuffer;

    return fromStatus(status);
}

ndk::ScopedAStatus LibcameraDevice::open(const std::shared_ptr<ICameraDeviceCallback>& in_callback,
                                         std::shared_ptr<ICameraDeviceSession>* _aidl_return) {
    if (in_callback == nullptr) {
        ALOGE("%s: Cannot open camera, callback is null", __FUNCTION__);
        _aidl_return = nullptr;
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    Status status = initStatus();
    if (status != Status::OK) {
        // Provider will never pass initFailed device to client, so
        // this must be a disconnected camera
        ALOGE("%s: Cannot open camera. camera is disconnected", __FUNCTION__);
        _aidl_return = nullptr;
        return fromStatus(Status::CAMERA_DISCONNECTED);
    }

    Mutex::Autolock _l(mLock);

    /** Open HAL device */
    status_t res;
    camera3_device_t *device;

    ATRACE_BEGIN("camera3->open");
    res = mModule->open(mCameraId.c_str(), reinterpret_cast<hw_device_t**>(&device));
    ATRACE_END();
    if (res != OK) {
        ALOGE("%s: Cannot open camera %s: %d", __FUNCTION__, mCameraId.c_str(), res);
        _aidl_return = nullptr;
        return fromStatus(getAidlStatus(res));
    }

    /** Cross-check device version */
    if (device->common.version < CAMERA_DEVICE_API_VERSION_3_2) {
        ALOGE("%s: Could not open camera: "
                "Camera device should be at least %x, reports %x instead",
                __FUNCTION__,
                CAMERA_DEVICE_API_VERSION_3_2,
                device->common.version);
        device->common.close(&device->common);
        _aidl_return = nullptr;
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    struct camera_info info;
    res = mModule->getCameraInfo(mCameraIdInt, &info);
    if (res != OK) {
        ALOGE("%s: Could not open camera: getCameraInfo failed");
        device->common.close(&device->common);
        _aidl_return = nullptr;
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    *_aidl_return = nullptr; // session;
    return fromStatus(Status::OK);
}
ndk::ScopedAStatus LibcameraDevice::openInjectionSession(
        const std::shared_ptr<ICameraDeviceCallback>& in_callback,
        std::shared_ptr<ICameraInjectionSession>* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::setTorchMode(bool in_on) {
    if (!mModule->isSetTorchModeSupported())
        return fromStatus(Status::OPERATION_NOT_SUPPORTED);

    Status status = initStatus();
    if (status == Status::OK) {
        status = getAidlStatus(mModule->setTorchMode(mCameraId.c_str(), in_on));
    }

    return fromStatus(status);
}

ndk::ScopedAStatus LibcameraDevice::turnOnTorchWithStrengthLevel(int32_t in_torchStrength) {
    // not supported by hardware module
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::getTorchStrengthLevel(int32_t* _aidl_return) {
    // not supported by hardware module
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

}  // namespace implementation
}  // namespace device
}  // namespace camera
}  // namespace hardware
}  // namespace android
