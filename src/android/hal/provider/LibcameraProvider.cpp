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

#define LOG_TAG "LibcameraProvider"
//#define LOG_NDEBUG 0
#include <android/log.h>

#include "LibcameraProvider.h"
#include <aidl/android/hardware/camera/common/Status.h>

#include <LibcameraDevice.h>

#include <convert.h>
#include <cutils/properties.h>
#include <regex>
#include <string>
#include <utils/Trace.h>

namespace android {
namespace hardware {
namespace camera {
namespace provider {
namespace implementation {

namespace {
// "device@<version>/libcamera/<id>"
const std::regex kDeviceNameRE("device@([0-9]+\\.[0-9]+)/libcamera/(.+)");
const char *kHAL3_4 = "3.4";
const char *kHAL3_5 = "3.5";
const int kMaxCameraDeviceNameLen = 128;
const int kMaxCameraIdLen = 16;

bool matchDeviceName(const std::string& deviceName, std::string* deviceVersion,
                     std::string* cameraId) {
    std::smatch sm;
    if (std::regex_match(deviceName, sm, kDeviceNameRE)) {
        if (deviceVersion != nullptr) {
            *deviceVersion = sm[1];
        }
        if (cameraId != nullptr) {
            *cameraId = sm[2];
        }
        return true;
    }
    return false;
}

} // anonymous namespace

using ::aidl::android::hardware::camera::common::Status;
using ::aidl::android::hardware::camera::common::CameraDeviceStatus;
using ::aidl::android::hardware::camera::common::TorchModeStatus;
using ::android::hardware::camera::device::implementation::LibcameraDevice;
using ::android::hardware::camera::device::implementation::fromStatus;

void LibcameraProvider::addDeviceNames(int camera_id, CameraDeviceStatus status, bool cam_new)
{
    char cameraId[kMaxCameraIdLen];
    snprintf(cameraId, sizeof(cameraId), "%d", camera_id);
    std::string cameraIdStr(cameraId);

    mCameraIds.add(cameraIdStr);

    // initialize mCameraDeviceNames and mOpenLegacySupported
    mOpenLegacySupported[cameraIdStr] = false;
    int deviceVersion = mModule->getDeviceVersion(camera_id);
    auto deviceNamePair = std::make_pair(cameraIdStr,
                                         getAidlDeviceName(cameraIdStr, deviceVersion));
    mCameraDeviceNames.add(deviceNamePair);
    if (cam_new) {
        mCallbacks->cameraDeviceStatusChange(deviceNamePair.second, status);
    }
    if (deviceVersion >= CAMERA_DEVICE_API_VERSION_3_2 &&
            mModule->isOpenLegacyDefined()) {
        // try open_legacy to see if it actually works
        struct hw_device_t* halDev = nullptr;
        int ret = mModule->openLegacy(cameraId, CAMERA_DEVICE_API_VERSION_1_0, &halDev);
        if (ret == 0) {
            mOpenLegacySupported[cameraIdStr] = true;
            halDev->close(halDev);
            deviceNamePair = std::make_pair(cameraIdStr,
                            getAidlDeviceName(cameraIdStr, CAMERA_DEVICE_API_VERSION_1_0));
            mCameraDeviceNames.add(deviceNamePair);
            if (cam_new) {
                mCallbacks->cameraDeviceStatusChange(deviceNamePair.second, status);
            }
        } else if (ret == -EBUSY || ret == -EUSERS) {
            // Looks like this provider instance is not initialized during
            // system startup and there are other camera users already.
            // Not a good sign but not fatal.
            ALOGW("%s: open_legacy try failed!", __FUNCTION__);
        }
    }
}

void LibcameraProvider::removeDeviceNames(int camera_id)
{
    std::string cameraIdStr = std::to_string(camera_id);

    mCameraIds.remove(cameraIdStr);

    int deviceVersion = mModule->getDeviceVersion(camera_id);
    auto deviceNamePair = std::make_pair(cameraIdStr,
                                         getAidlDeviceName(cameraIdStr, deviceVersion));
    mCameraDeviceNames.remove(deviceNamePair);
    mCallbacks->cameraDeviceStatusChange(deviceNamePair.second, CameraDeviceStatus::NOT_PRESENT);
    if (deviceVersion >= CAMERA_DEVICE_API_VERSION_3_2 &&
        mModule->isOpenLegacyDefined() && mOpenLegacySupported[cameraIdStr]) {

        deviceNamePair = std::make_pair(cameraIdStr,
                            getAidlDeviceName(cameraIdStr, CAMERA_DEVICE_API_VERSION_1_0));
        mCameraDeviceNames.remove(deviceNamePair);
        mCallbacks->cameraDeviceStatusChange(deviceNamePair.second,
                                             CameraDeviceStatus::NOT_PRESENT);
    }

    mModule->removeCamera(camera_id);
}

/**
 * static callback forwarding methods from HAL to instance
 */
void LibcameraProvider::sCameraDeviceStatusChange(
        const struct camera_module_callbacks* callbacks,
        int camera_id,
        int new_status) {
    LibcameraProvider* cp = const_cast<LibcameraProvider*>(
            static_cast<const LibcameraProvider*>(callbacks));
    if (cp == nullptr) {
        ALOGE("%s: callback ops is null", __FUNCTION__);
        return;
    }

    Mutex::Autolock _l(cp->mCbLock);
    char cameraId[kMaxCameraIdLen];
    snprintf(cameraId, sizeof(cameraId), "%d", camera_id);
    std::string cameraIdStr(cameraId);
    cp->mCameraStatusMap[cameraIdStr] = (CameraDeviceStatus) new_status;

    if (cp->mCallbacks == nullptr) {
        // For camera connected before mCallbacks is set, the corresponding
        // addDeviceNames() would be called later in setCallbacks().
        return;
    }

    bool found = false;
    CameraDeviceStatus status = (CameraDeviceStatus)new_status;
    for (auto const& deviceNamePair : cp->mCameraDeviceNames) {
        if (cameraIdStr.compare(deviceNamePair.first) == 0) {
            cp->mCallbacks->cameraDeviceStatusChange(deviceNamePair.second, status);
            found = true;
        }
    }

    switch (status) {
        case CameraDeviceStatus::PRESENT:
        case CameraDeviceStatus::ENUMERATING:
            if (!found) {
                cp->addDeviceNames(camera_id, status, true);
            }
            break;
        case CameraDeviceStatus::NOT_PRESENT:
            if (found) {
                cp->removeDeviceNames(camera_id);
            }
    }
}

void LibcameraProvider::sTorchModeStatusChange(
        const struct camera_module_callbacks* callbacks,
        const char* camera_id,
        int new_status) {
    LibcameraProvider* cp = const_cast<LibcameraProvider*>(
            static_cast<const LibcameraProvider*>(callbacks));

    if (cp == nullptr) {
        ALOGE("%s: callback ops is null", __FUNCTION__);
        return;
    }

    Mutex::Autolock _l(cp->mCbLock);
    if (cp->mCallbacks != nullptr) {
        std::string cameraIdStr(camera_id);
        TorchModeStatus status = (TorchModeStatus) new_status;
        for (auto const& deviceNamePair : cp->mCameraDeviceNames) {
            if (cameraIdStr.compare(deviceNamePair.first) == 0) {
                cp->mCallbacks->torchModeStatusChange(
                        deviceNamePair.second, status);
            }
        }
    }
}

std::string LibcameraProvider::getAidlDeviceName(
        std::string cameraId, int deviceVersion) {
    // Maybe consider create a version check method and SortedVec to speed up?
    if (deviceVersion != CAMERA_DEVICE_API_VERSION_1_0 &&
            deviceVersion != CAMERA_DEVICE_API_VERSION_3_2 &&
            deviceVersion != CAMERA_DEVICE_API_VERSION_3_3 &&
            deviceVersion != CAMERA_DEVICE_API_VERSION_3_4 &&
            deviceVersion != CAMERA_DEVICE_API_VERSION_3_5 &&
            deviceVersion != CAMERA_DEVICE_API_VERSION_3_6) {
        ALOGE("%s: Invalid device HAL version: %d", __FUNCTION__, deviceVersion);
        return "";
    }

    // AIDL version 1
    char deviceName[kMaxCameraDeviceNameLen];
    snprintf(deviceName, sizeof(deviceName), "device@1.0/libcamera/%s", cameraId.c_str());
    return deviceName;
}

LibcameraProvider::LibcameraProvider() :
        BnCameraProvider(),
        camera_module_callbacks_t({sCameraDeviceStatusChange,
                                   sTorchModeStatusChange}) {
    mInitFailed = initialize();
}

LibcameraProvider::~LibcameraProvider() {}

bool LibcameraProvider::initialize() {
    camera_module_t *rawModule;
    int err = hw_get_module(CAMERA_HARDWARE_MODULE_ID,
            (const hw_module_t **)&rawModule);
    if (err < 0) {
        ALOGE("Could not load camera HAL module: %d (%s)", err, strerror(-err));
        return true;
    }

    mModule = new CameraModule(rawModule);
    err = mModule->init();
    if (err != OK) {
        ALOGE("Could not initialize camera HAL module: %d (%s)", err, strerror(-err));
        mModule.clear();
        return true;
    }
    ALOGI("Loaded \"%s\" camera module", mModule->getModuleName());

    // Setup callback now because we are going to try openLegacy next
    err = mModule->setCallbacks(this);
    if (err != OK) {
        ALOGE("Could not set camera module callback: %d (%s)", err, strerror(-err));
        mModule.clear();
        return true;
    }

    mPreferredHal3MinorVersion =
        property_get_int32("ro.vendor.camera.wrapper.hal3TrebleMinorVersion", 3);
    ALOGV("Preferred HAL 3 minor version is %d", mPreferredHal3MinorVersion);
    switch(mPreferredHal3MinorVersion) {
        case 2:
        case 3:
            // OK
            break;
        default:
            ALOGW("Unknown minor camera device HAL version %d in property "
                    "'camera.wrapper.hal3TrebleMinorVersion', defaulting to 3",
                    mPreferredHal3MinorVersion);
            mPreferredHal3MinorVersion = 3;
    }

    mNumberOfLegacyCameras = mModule->getNumberOfCameras();
    for (int i = 0; i < mNumberOfLegacyCameras; i++) {
        struct camera_info info;
        auto rc = mModule->getCameraInfo(i, &info);
        if (rc != NO_ERROR) {
            ALOGE("%s: Camera info query failed!", __func__);
            mModule.clear();
            return true;
        }

        if (checkCameraVersion(i, info) != OK) {
            ALOGE("%s: Camera version check failed!", __func__);
            mModule.clear();
            return true;
        }

        char cameraId[kMaxCameraIdLen];
        snprintf(cameraId, sizeof(cameraId), "%d", i);
        std::string cameraIdStr(cameraId);
        mCameraStatusMap[cameraIdStr] = CameraDeviceStatus::PRESENT;

        addDeviceNames(i);
    }

    return false; // mInitFailed
}

/**
 * Check that the device HAL version is still in supported.
 */
int LibcameraProvider::checkCameraVersion(int id, camera_info info) {
    if (mModule == nullptr) {
        return NO_INIT;
    }

    // device_version undefined in CAMERA_MODULE_API_VERSION_1_0,
    // All CAMERA_MODULE_API_VERSION_1_0 devices are backward-compatible
    uint16_t moduleVersion = mModule->getModuleApiVersion();
    if (moduleVersion >= CAMERA_MODULE_API_VERSION_2_0) {
        // Verify the device version is in the supported range
        switch (info.device_version) {
            case CAMERA_DEVICE_API_VERSION_1_0:
            case CAMERA_DEVICE_API_VERSION_3_2:
            case CAMERA_DEVICE_API_VERSION_3_3:
            case CAMERA_DEVICE_API_VERSION_3_4:
            case CAMERA_DEVICE_API_VERSION_3_5:
                // in support
                break;
            case CAMERA_DEVICE_API_VERSION_3_6:
                /**
                 * ICameraDevice@3.5 contains APIs from both
                 * CAMERA_DEVICE_API_VERSION_3_6 and CAMERA_MODULE_API_VERSION_2_5
                 * so we require HALs to uprev both for simplified supported combinations.
                 * HAL can still opt in individual new APIs indepedently.
                 */
                if (moduleVersion < CAMERA_MODULE_API_VERSION_2_5) {
                    ALOGE("%s: Device %d has unsupported version combination:"
                            "HAL version %x and module version %x",
                            __FUNCTION__, id, info.device_version, moduleVersion);
                    return NO_INIT;
                }
                break;
            case CAMERA_DEVICE_API_VERSION_2_0:
            case CAMERA_DEVICE_API_VERSION_2_1:
            case CAMERA_DEVICE_API_VERSION_3_0:
            case CAMERA_DEVICE_API_VERSION_3_1:
                // no longer supported
            default:
                ALOGE("%s: Device %d has HAL version %x, which is not supported",
                        __FUNCTION__, id, info.device_version);
                return NO_INIT;
        }
    }

    return OK;
}

ndk::ScopedAStatus LibcameraProvider::setCallback(
        const std::shared_ptr<ICameraProviderCallback>& in_callback) {
    Mutex::Autolock _l(mCbLock);
    mCallbacks = in_callback;
    if (in_callback == nullptr)
        return fromStatus(Status::ILLEGAL_ARGUMENT);

    // Add and report all presenting external cameras.
    for (auto const& statusPair : mCameraStatusMap) {
        int id = std::stoi(statusPair.first);
        auto status = static_cast<CameraDeviceStatus>(statusPair.second);
        if (id >= mNumberOfLegacyCameras && status != CameraDeviceStatus::NOT_PRESENT) {
            addDeviceNames(id, status, true);
        }
    }

    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraProvider::getVendorTags(
        std::vector<VendorTagSection>* _aidl_return) {
    if (_aidl_return == nullptr) {
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }
    // libcamera does not implement the .get_vendor_tag_ops
    // returning empty vector.
    *_aidl_return = {};
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraProvider::getCameraIdList(std::vector<std::string>* _aidl_return) {
    if (_aidl_return == nullptr) {
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    for (auto const& deviceNamePair : mCameraDeviceNames) {
        if (std::stoi(deviceNamePair.first) >= mNumberOfLegacyCameras) {
            // External camera devices must be reported through the device status change callback,
            // not in this list.
            continue;
        }
        if (mCameraStatusMap[deviceNamePair.first] == CameraDeviceStatus::PRESENT) {
            _aidl_return->push_back(deviceNamePair.second);
        }
    }

    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraProvider::getCameraDeviceInterface(
        const std::string& in_cameraDeviceName,
        std::shared_ptr<ICameraDevice>* _aidl_return) {
    if (_aidl_return == nullptr) {
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    std::string deviceVersion, cameraId;
    bool match = matchDeviceName(in_cameraDeviceName, &deviceVersion, &cameraId);

    if (!match) {
        *_aidl_return = nullptr;
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    ssize_t index = mCameraDeviceNames.indexOf(std::make_pair(cameraId, in_cameraDeviceName));
    if (index == NAME_NOT_FOUND) { // Either an illegal name or a device version mismatch
        Status status = Status::OK;
        ssize_t idx = mCameraIds.indexOf(cameraId);
        if (idx == NAME_NOT_FOUND) {
            ALOGE("%s: cannot find camera %s!", __FUNCTION__, cameraId.c_str());
            status = Status::ILLEGAL_ARGUMENT;
        } else { // invalid version
            ALOGE("%s: camera device %s does not support version %s!",
                    __FUNCTION__, cameraId.c_str(), deviceVersion.c_str());
            status = Status::OPERATION_NOT_SUPPORTED;
        }
        *_aidl_return = nullptr;
        return fromStatus(status);
    }

    if (mCameraStatusMap.count(cameraId) == 0 ||
            mCameraStatusMap[cameraId] != CameraDeviceStatus::PRESENT) {
        *_aidl_return = nullptr;
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }

    std::shared_ptr<LibcameraDevice> device =
        ndk::SharedRefBase::make<LibcameraDevice>(mModule, cameraId, mCameraDeviceNames);

    if (device == nullptr) {
        ALOGE("%s: cannot allocate camera device for id %s", __FUNCTION__, in_cameraDeviceName.c_str());
        *_aidl_return = nullptr;
        return fromStatus(Status::INTERNAL_ERROR);
    }

    if (device->isInitFailed()) {
        ALOGE("%s: camera device %s init failed!", __FUNCTION__, in_cameraDeviceName.c_str());
        *_aidl_return = nullptr;
        return fromStatus(Status::INTERNAL_ERROR);
    }

    *_aidl_return = device;
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraProvider::notifyDeviceStateChange(
        int64_t in_deviceState) {
    ALOGD("%s: New device state: 0x%" PRIx64, __FUNCTION__, in_deviceState);
    uint64_t state = static_cast<uint64_t>(in_deviceState);
    mModule->notifyDeviceStateChange(state);
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraProvider::getConcurrentCameraIds(
        std::vector<ConcurrentCameraIdCombination>* _aidl_return) {
    if (_aidl_return == nullptr) {
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }
    // No concurrent camera combinations are supported
    *_aidl_return = {};
    return fromStatus(Status::OK);
}

ndk::ScopedAStatus LibcameraProvider::isConcurrentStreamCombinationSupported(
        const std::vector<CameraIdAndStreamCombination>&, bool* _aidl_return) {
    if (_aidl_return == nullptr) {
        return fromStatus(Status::ILLEGAL_ARGUMENT);
    }
    // No concurrent stream combinations are supported
    *_aidl_return = false;
    return fromStatus(Status::OK);
}

} // namespace implementation
}  // namespace provider
}  // namespace camera
}  // namespace hardware
}  // namespace android
