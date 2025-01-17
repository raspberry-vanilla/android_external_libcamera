/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ANDROID_HARDWARE_CAMERA_PROVIDER_V2_5_LEGACYCAMERAPROVIDER_H
#define ANDROID_HARDWARE_CAMERA_PROVIDER_V2_5_LEGACYCAMERAPROVIDER_H

#include <SimpleThread.h>
#include <aidl/android/hardware/camera/common/CameraDeviceStatus.h>
#include <aidl/android/hardware/camera/common/VendorTagSection.h>
#include <aidl/android/hardware/camera/device/ICameraDevice.h>
#include <aidl/android/hardware/camera/provider/BnCameraProvider.h>
#include <aidl/android/hardware/camera/provider/CameraIdAndStreamCombination.h>
#include <aidl/android/hardware/camera/provider/ConcurrentCameraIdCombination.h>
#include <aidl/android/hardware/camera/provider/ICameraProviderCallback.h>
#include <hardware/camera_common.h>
#include <CameraModule.h>
#include <VendorTagDescriptor.h>
#include <poll.h>
#include <utils/Mutex.h>
#include <utils/Thread.h>
#include <utils/SortedVector.h>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <map>

namespace android {
namespace hardware {
namespace camera {
namespace provider {
namespace implementation {

using ::aidl::android::hardware::camera::common::CameraDeviceStatus;
using ::aidl::android::hardware::camera::common::VendorTagSection;
using ::aidl::android::hardware::camera::device::ICameraDevice;
using ::aidl::android::hardware::camera::provider::BnCameraProvider;
using ::aidl::android::hardware::camera::provider::CameraIdAndStreamCombination;
using ::aidl::android::hardware::camera::provider::ConcurrentCameraIdCombination;
using ::aidl::android::hardware::camera::provider::ICameraProviderCallback;
using ::android::hardware::camera::common::helper::SimpleThread;
using ::android::hardware::camera::common::helper::CameraModule;

/**
 * This implementation supports cameras implemented via the legacy libhardware
 * camera HAL definitions.
 */
class LibcameraProvider : public BnCameraProvider,
                          public camera_module_callbacks_t {
public:
    LibcameraProvider();
    ~LibcameraProvider() override;

    // Caller must use this method to check if CameraProvider ctor failed
    bool isInitFailed() { return mInitFailed; }

    ndk::ScopedAStatus setCallback(
            const std::shared_ptr<ICameraProviderCallback>& in_callback) override;
    ndk::ScopedAStatus getVendorTags(std::vector<VendorTagSection>* _aidl_return) override;
    ndk::ScopedAStatus getCameraIdList(std::vector<std::string>* _aidl_return) override;
    ndk::ScopedAStatus getCameraDeviceInterface(
            const std::string& in_cameraDeviceName,
            std::shared_ptr<ICameraDevice>* _aidl_return) override;
    ndk::ScopedAStatus notifyDeviceStateChange(int64_t in_deviceState) override;
    ndk::ScopedAStatus getConcurrentCameraIds(
            std::vector<ConcurrentCameraIdCombination>* _aidl_return) override;
    ndk::ScopedAStatus isConcurrentStreamCombinationSupported(
            const std::vector<CameraIdAndStreamCombination>& in_configs,
            bool* _aidl_return) override;

protected:
    Mutex mCbLock;
    std::shared_ptr<ICameraProviderCallback> mCallbacks = nullptr;

    sp<CameraModule> mModule;

    int mNumberOfLegacyCameras;
    std::map<std::string, CameraDeviceStatus> mCameraStatusMap; // camera id -> status
    std::map<std::string, bool> mOpenLegacySupported; // camera id -> open_legacy HAL1.0 supported
    SortedVector<std::string> mCameraIds; // the "0"/"1" libcamera camera Ids
    // (cameraId string, aidl device name) pairs
    SortedVector<std::pair<std::string, std::string>> mCameraDeviceNames;

    int mPreferredHal3MinorVersion;

    // Must be queried before using any APIs.
    // APIs will only work when this returns true
    bool mInitFailed;
    bool initialize();

    int checkCameraVersion(int id, camera_info info);

    // create HIDL device name from camera ID and legacy device version
    std::string getAidlDeviceName(std::string cameraId, int deviceVersion);

    // static callback forwarding methods
    static void sCameraDeviceStatusChange(
        const struct camera_module_callbacks* callbacks,
        int camera_id,
        int new_status);
    static void sTorchModeStatusChange(
        const struct camera_module_callbacks* callbacks,
        const char* camera_id,
        int new_status);

    void addDeviceNames(int camera_id, CameraDeviceStatus status = CameraDeviceStatus::PRESENT,
                        bool cam_new = false);
    void removeDeviceNames(int camera_id);

};

}  // namespace implementation
}  // namespace provider
}  // namespace camera
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_CAMERA_PROVIDER_V2_5_LEGACYCAMERAPROVIDER_H
