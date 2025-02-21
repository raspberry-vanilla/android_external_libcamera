/*
 * Copyright 2017 The Android Open Source Project
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

#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "LibcameraProvider.h"

using ::android::hardware::camera::provider::implementation::LibcameraProvider;

namespace {
// Default recommended RPC thread count for camera provider implementations
const int HWBINDER_THREAD_COUNT = 6;
}  // namespace

int main()
{
    ALOGI("LibcameraProvider libcamera service is starting.");

    ABinderProcess_setThreadPoolMaxThreadCount(HWBINDER_THREAD_COUNT);

    std::shared_ptr<LibcameraProvider> provider = ndk::SharedRefBase::make<LibcameraProvider>();
    const std::string serviceName = std::string(LibcameraProvider::descriptor) + "/libcamera/0";

    binder_exception_t ret =
            AServiceManager_addService(provider->asBinder().get(), serviceName.c_str());
    LOG_ALWAYS_FATAL_IF(ret != EX_NONE,
                        "Error while registering libcamera camera provider service: %d", ret);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
