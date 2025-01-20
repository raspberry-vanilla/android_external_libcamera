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
        const SortedVector<std::pair<std::string, std::string>>& cameraDeviceNames) {}

LibcameraDevice::~LibcameraDevice() = default;

ndk::ScopedAStatus LibcameraDevice::getCameraCharacteristics(CameraMetadata* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::getPhysicalCameraCharacteristics(const std::string& in_physicalCameraId,
                                                                     CameraMetadata* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::getResourceCost(CameraResourceCost* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::isStreamCombinationSupported(const StreamConfiguration& in_streams,
                                                                 bool* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::open(const std::shared_ptr<ICameraDeviceCallback>& in_callback,
                                         std::shared_ptr<ICameraDeviceSession>* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}
ndk::ScopedAStatus LibcameraDevice::openInjectionSession(
        const std::shared_ptr<ICameraDeviceCallback>& in_callback,
        std::shared_ptr<ICameraInjectionSession>* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::setTorchMode(bool in_on) {
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::turnOnTorchWithStrengthLevel(int32_t in_torchStrength) {
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

ndk::ScopedAStatus LibcameraDevice::getTorchStrengthLevel(int32_t* _aidl_return) {
    _aidl_return = nullptr;
    return fromStatus(Status::ILLEGAL_ARGUMENT);
}

}  // namespace implementation
}  // namespace device
}  // namespace camera
}  // namespace hardware
}  // namespace android
