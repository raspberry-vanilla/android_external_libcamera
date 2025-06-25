/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2020, Google Inc.
 *
 * Image Processing Algorithm proxy for raspberrypi
 *
 * This file is auto-generated. Do not edit.
 */

#pragma once

#include <libcamera/ipa/ipa_interface.h>
#include <libcamera/ipa/raspberrypi_ipa_interface.h>

#include <libcamera/base/object.h>
#include <libcamera/base/thread.h>

#include "libcamera/internal/control_serializer.h"
#include "libcamera/internal/ipa_proxy.h"
#include "libcamera/internal/ipc_pipe.h"
#include "libcamera/internal/ipc_pipe_unixsocket.h"
#include "libcamera/internal/ipc_unixsocket.h"

namespace libcamera {

namespace ipa {

namespace RPi {


class IPAProxyRPi : public IPAProxy, public IPARPiInterface, public Object
{
public:
	IPAProxyRPi(IPAModule *ipam, bool isolate);
	~IPAProxyRPi();


        int32_t init(
        	const IPASettings &settings,
        	const InitParams &params,
        	InitResult *result) override;

        void start(
        	const ControlList &controls,
        	StartResult *result) override;

        void stop() override;

        int32_t configure(
        	const IPACameraSensorInfo &sensorInfo,
        	const ConfigParams &params,
        	ConfigResult *result) override;

        void mapBuffers(
        	const std::vector<libcamera::IPABuffer> &buffers) override;

        void unmapBuffers(
        	const std::vector<uint32_t> &ids) override;

        void prepareIsp(
        	const PrepareParams &params) override;

        void processStats(
        	const ProcessParams &params) override;


private:
	void recvMessage(const IPCMessage &data);


        int32_t initThread(
        	const IPASettings &settings,
        	const InitParams &params,
        	InitResult *result);
        int32_t initIPC(
        	const IPASettings &settings,
        	const InitParams &params,
        	InitResult *result);

        void startThread(
        	const ControlList &controls,
        	StartResult *result);
        void startIPC(
        	const ControlList &controls,
        	StartResult *result);

        void stopThread();
        void stopIPC();

        int32_t configureThread(
        	const IPACameraSensorInfo &sensorInfo,
        	const ConfigParams &params,
        	ConfigResult *result);
        int32_t configureIPC(
        	const IPACameraSensorInfo &sensorInfo,
        	const ConfigParams &params,
        	ConfigResult *result);

        void mapBuffersThread(
        	const std::vector<libcamera::IPABuffer> &buffers);
        void mapBuffersIPC(
        	const std::vector<libcamera::IPABuffer> &buffers);

        void unmapBuffersThread(
        	const std::vector<uint32_t> &ids);
        void unmapBuffersIPC(
        	const std::vector<uint32_t> &ids);

        void prepareIspThread(
        	const PrepareParams &params);
        void prepareIspIPC(
        	const PrepareParams &params);

        void processStatsThread(
        	const ProcessParams &params);
        void processStatsIPC(
        	const ProcessParams &params);


        void prepareIspCompleteThread(
        	const BufferIds &buffers,
        	const bool stitchSwapBuffers);
	void prepareIspCompleteIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

        void processStatsCompleteThread(
        	const BufferIds &buffers);
	void processStatsCompleteIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

        void metadataReadyThread(
        	const ControlList &metadata);
	void metadataReadyIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

        void setIspControlsThread(
        	const ControlList &controls);
	void setIspControlsIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

        void setDelayedControlsThread(
        	const ControlList &controls,
        	const uint32_t delayContext);
	void setDelayedControlsIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

        void setLensControlsThread(
        	const ControlList &controls);
	void setLensControlsIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

        void setCameraTimeoutThread(
        	const uint32_t maxFrameLengthMs);
	void setCameraTimeoutIPC(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);


	/* Helper class to invoke async functions in another thread. */
	class ThreadProxy : public Object
	{
	public:
		ThreadProxy()
			: ipa_(nullptr)
		{
		}

		void setIPA(IPARPiInterface *ipa)
		{
			ipa_ = ipa;
		}

		void stop()
		{
			ipa_->stop();
		}

		void start(
                	const ControlList &controls,
                	StartResult *result)
		{
			ipa_->start(controls, result);
		}
		void prepareIsp(
                	const PrepareParams &params)
		{
			ipa_->prepareIsp(params);
		}
		void processStats(
                	const ProcessParams &params)
		{
			ipa_->processStats(params);
		}

	private:
		IPARPiInterface *ipa_;
	};

	Thread thread_;
	ThreadProxy proxy_;
	std::unique_ptr<IPARPiInterface> ipa_;

	const bool isolate_;

	std::unique_ptr<IPCPipeUnixSocket> ipc_;

	ControlSerializer controlSerializer_;


	uint32_t seq_;
};

} /* namespace RPi */

} /* namespace ipa */

} /* namespace libcamera */
