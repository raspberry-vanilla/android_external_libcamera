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


class IPAProxyRPiThreaded;
class IPAProxyRPiIsolated;

class IPAProxyRPi : public IPAProxy, public IPARPiInterface, public Object
{
public:
	using Threaded = IPAProxyRPiThreaded;
	using Isolated = IPAProxyRPiIsolated;

protected:
	using IPAProxy::IPAProxy;
};

class IPAProxyRPiThreaded : public IPAProxyRPi
{
public:
	IPAProxyRPiThreaded(IPAModule *ipam, const CameraManager &cm);
	~IPAProxyRPiThreaded();


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

        void prepareIspCompleteHandler(
        	const BufferIds &buffers,
        	const bool stitchSwapBuffers);

        void processStatsCompleteHandler(
        	const BufferIds &buffers);

        void metadataReadyHandler(
        	const ControlList &metadata);

        void setIspControlsHandler(
        	const ControlList &controls);

        void setDelayedControlsHandler(
        	const ControlList &controls,
        	const uint32_t delayContext);

        void setLensControlsHandler(
        	const ControlList &controls);

        void setCameraTimeoutHandler(
        	const uint32_t maxFrameLengthMs);


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
};

class IPAProxyRPiIsolated : public IPAProxyRPi
{
public:
	IPAProxyRPiIsolated(IPAModule *ipam, const CameraManager &cm);
	~IPAProxyRPiIsolated();


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


	void prepareIspCompleteHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

	void processStatsCompleteHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

	void metadataReadyHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

	void setIspControlsHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

	void setDelayedControlsHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

	void setLensControlsHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);

	void setCameraTimeoutHandler(
		std::vector<uint8_t>::const_iterator data,
		size_t dataSize,
		const std::vector<SharedFD> &fds);


	std::unique_ptr<IPCPipeUnixSocket> ipc_;

	ControlSerializer controlSerializer_;


	uint32_t seq_;
};

} /* namespace RPi */

} /* namespace ipa */

} /* namespace libcamera */