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

#pragma once

#include <aidl/android/hardware/camera/common/Status.h>
#include <aidl/android/hardware/camera/device/BnCameraDevice.h>

#include <CameraModule.h>

#include "LibcameraDeviceSession.h"

#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace android {
namespace hardware {
namespace camera {
namespace device {
namespace implementation {

using ::aidl::android::hardware::camera::common::CameraResourceCost;
using ::aidl::android::hardware::camera::common::Status;
using ::aidl::android::hardware::camera::device::BnCameraDevice;
using ::aidl::android::hardware::camera::device::CameraMetadata;
using ::aidl::android::hardware::camera::device::ICameraDeviceCallback;
using ::aidl::android::hardware::camera::device::ICameraDeviceSession;
using ::aidl::android::hardware::camera::device::ICameraInjectionSession;
using ::aidl::android::hardware::camera::device::StreamConfiguration;

using ::android::hardware::camera::common::helper::CameraModule;

using ::android::sp;

class LibcameraDevice : public BnCameraDevice {
public:
    // Called by provider HAL.
    // Provider HAL must ensure the uniqueness of CameraDevice object per cameraId, or there could
    // be multiple CameraDevice trying to access the same physical camera.  Also, provider will have
    // to keep track of all CameraDevice objects in order to notify CameraDevice when the underlying
    // camera is detached.
    LibcameraDevice(sp<CameraModule> module,
                 const std::string& cameraId,
                 const SortedVector<std::pair<std::string, std::string>>& cameraDeviceNames);
    virtual ~LibcameraDevice();

    ndk::ScopedAStatus getCameraCharacteristics(CameraMetadata* _aidl_return) override;
    ndk::ScopedAStatus getPhysicalCameraCharacteristics(const std::string& in_physicalCameraId,
                                                        CameraMetadata* _aidl_return) override;
    ndk::ScopedAStatus getResourceCost(CameraResourceCost* _aidl_return) override;
    ndk::ScopedAStatus isStreamCombinationSupported(const StreamConfiguration& in_streams,
                                                    bool* _aidl_return) override;
    ndk::ScopedAStatus open(const std::shared_ptr<ICameraDeviceCallback>& in_callback,
                            std::shared_ptr<ICameraDeviceSession>* _aidl_return) override;
    ndk::ScopedAStatus openInjectionSession(
            const std::shared_ptr<ICameraDeviceCallback>& in_callback,
            std::shared_ptr<ICameraInjectionSession>* _aidl_return) override;
    ndk::ScopedAStatus setTorchMode(bool in_on) override;
    ndk::ScopedAStatus turnOnTorchWithStrengthLevel(int32_t in_torchStrength) override;
    ndk::ScopedAStatus getTorchStrengthLevel(int32_t* _aidl_return) override;

    // Caller must use this method to check if CameraDevice ctor failed
    bool isInitFailed() { return mInitFail; }

private:
    sp<CameraModule> mModule;
    std::string mCameraId;
    // const after ctor
    int mCameraIdInt;
    int mDeviceVersion;
    bool mInitFail = false;

    // gating access to mSession
    mutable Mutex mLock;

    Status initStatus() const;

    std::shared_ptr<LibcameraDeviceSession> mSession = nullptr;

    virtual std::shared_ptr<LibcameraDeviceSession> createSession(
            camera3_device_t* device,
            const camera_metadata_t* deviceInfo,
            const std::shared_ptr<ICameraDeviceCallback>& cb);
};

}  // namespace implementation
}  // namespace device
}  // namespace camera
}  // namespace hardware
}  // namespace android
