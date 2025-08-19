/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2020, Google Inc.
 *
 * Image Processing Algorithm proxy for raspberrypi
 *
 * This file is auto-generated. Do not edit.
 */

#include <libcamera/ipa/raspberrypi_ipa_proxy.h>

#include <memory>
#include <string>
#include <vector>

#include <libcamera/ipa/ipa_module_info.h>
#include <libcamera/ipa/raspberrypi_ipa_interface.h>
#include <libcamera/ipa/raspberrypi_ipa_serializer.h>

#include <libcamera/base/log.h>
#include <libcamera/base/thread.h>

#include "libcamera/internal/control_serializer.h"
#include "libcamera/internal/ipa_data_serializer.h"
#include "libcamera/internal/ipa_module.h"
#include "libcamera/internal/ipa_proxy.h"
#include "libcamera/internal/ipc_pipe.h"
#include "libcamera/internal/ipc_pipe_unixsocket.h"
#include "libcamera/internal/ipc_unixsocket.h"
#include "libcamera/internal/process.h"

namespace libcamera {

LOG_DECLARE_CATEGORY(IPAProxy)

namespace ipa {

namespace RPi {


IPAProxyRPi::IPAProxyRPi(IPAModule *ipam, bool isolate)
	: IPAProxy(ipam), isolate_(isolate),
	  controlSerializer_(ControlSerializer::Role::Proxy), seq_(0)
{
	LOG(IPAProxy, Debug)
		<< "initializing raspberrypi proxy: loading IPA from "
		<< ipam->path();

	if (isolate_) {
		const std::string proxyWorkerPath = resolvePath("raspberrypi_ipa_proxy");
		if (proxyWorkerPath.empty()) {
			LOG(IPAProxy, Error)
				<< "Failed to get proxy worker path";
			return;
		}

		auto ipc = std::make_unique<IPCPipeUnixSocket>(ipam->path().c_str(),
							       proxyWorkerPath.c_str());
		if (!ipc->isConnected()) {
			LOG(IPAProxy, Error) << "Failed to create IPCPipe";
			return;
		}

		ipc->recv.connect(this, &IPAProxyRPi::recvMessage);

		ipc_ = std::move(ipc);
		valid_ = true;
		return;
	}

	if (!ipam->load())
		return;

	IPAInterface *ipai = ipam->createInterface();
	if (!ipai) {
		LOG(IPAProxy, Error)
			<< "Failed to create IPA context for " << ipam->path();
		return;
	}

	ipa_ = std::unique_ptr<IPARPiInterface>(static_cast<IPARPiInterface *>(ipai));
	proxy_.setIPA(ipa_.get());


	ipa_->prepareIspComplete.connect(this, &IPAProxyRPi::prepareIspCompleteThread);
	ipa_->processStatsComplete.connect(this, &IPAProxyRPi::processStatsCompleteThread);
	ipa_->metadataReady.connect(this, &IPAProxyRPi::metadataReadyThread);
	ipa_->setIspControls.connect(this, &IPAProxyRPi::setIspControlsThread);
	ipa_->setDelayedControls.connect(this, &IPAProxyRPi::setDelayedControlsThread);
	ipa_->setLensControls.connect(this, &IPAProxyRPi::setLensControlsThread);
	ipa_->setCameraTimeout.connect(this, &IPAProxyRPi::setCameraTimeoutThread);

	valid_ = true;
}

IPAProxyRPi::~IPAProxyRPi()
{
	if (ipc_) {
		IPCMessage::Header header =
			{ static_cast<uint32_t>(_RPiCmd::Exit), seq_++ };
		IPCMessage msg(header);
		ipc_->sendAsync(msg);
	}
}


void IPAProxyRPi::recvMessage(const IPCMessage &data)
{
	size_t dataSize = data.data().size();
	_RPiEventCmd _cmd = static_cast<_RPiEventCmd>(data.header().cmd);

	switch (_cmd) {
	case _RPiEventCmd::PrepareIspComplete: {
		prepareIspCompleteIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::ProcessStatsComplete: {
		processStatsCompleteIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::MetadataReady: {
		metadataReadyIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetIspControls: {
		setIspControlsIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetDelayedControls: {
		setDelayedControlsIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetLensControls: {
		setLensControlsIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetCameraTimeout: {
		setCameraTimeoutIPC(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	default:
		LOG(IPAProxy, Error) << "Unknown command " << static_cast<uint32_t>(_cmd);
	}
}


int32_t IPAProxyRPi::init(
	const IPASettings &settings,
	const InitParams &params,
	InitResult *result)
{
	if (isolate_)
		return initIPC(settings, params, result);
	else
		return initThread(settings, params, result);
}

int32_t IPAProxyRPi::initThread(
	const IPASettings &settings,
	const InitParams &params,
	InitResult *result)
{
	int32_t _ret = ipa_->init(settings, params, result);

	proxy_.moveToThread(&thread_);

	return _ret;
}

int32_t IPAProxyRPi::initIPC(
	const IPASettings &settings,
	const InitParams &params,
	InitResult *result)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Init), seq_++ };
	IPCMessage _ipcInputBuf(_header);
	IPCMessage _ipcOutputBuf;


	std::vector<uint8_t> settingsBuf;
	std::tie(settingsBuf, std::ignore) =
		IPADataSerializer<IPASettings>::serialize(settings
);
	std::vector<uint8_t> paramsBuf;
	std::vector<SharedFD> paramsFds;
	std::tie(paramsBuf, paramsFds) =
		IPADataSerializer<InitParams>::serialize(params
);
	appendPOD<uint32_t>(_ipcInputBuf.data(), settingsBuf.size());
	appendPOD<uint32_t>(_ipcInputBuf.data(), paramsBuf.size());
	appendPOD<uint32_t>(_ipcInputBuf.data(), paramsFds.size());
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), settingsBuf.begin(), settingsBuf.end());
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), paramsBuf.begin(), paramsBuf.end());
	_ipcInputBuf.fds().insert(_ipcInputBuf.fds().end(), paramsFds.begin(), paramsFds.end());


	int _ret = ipc_->sendSync(_ipcInputBuf, &_ipcOutputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call init: " << _ret;
		return static_cast<int32_t>(_ret);
	}

	int32_t _retValue = IPADataSerializer<int32_t>::deserialize(_ipcOutputBuf.data(), 0);



	const size_t resultStart = 4;


	if (result) {
                *result =
                IPADataSerializer<InitResult>::deserialize(
                	_ipcOutputBuf.data().cbegin() + resultStart,
                	_ipcOutputBuf.data().cend(),
                	&controlSerializer_);
	}


	return _retValue;

}


void IPAProxyRPi::start(
	const ControlList &controls,
	StartResult *result)
{
	if (isolate_)
		return startIPC(controls, result);
	else
		return startThread(controls, result);
}

void IPAProxyRPi::startThread(
	const ControlList &controls,
	StartResult *result)
{
	state_ = ProxyRunning;
	thread_.start();

	return proxy_.invokeMethod(&ThreadProxy::start, ConnectionTypeBlocking, controls, result);
}

void IPAProxyRPi::startIPC(
	const ControlList &controls,
	StartResult *result)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Start), seq_++ };
	IPCMessage _ipcInputBuf(_header);
	IPCMessage _ipcOutputBuf;


	std::vector<uint8_t> controlsBuf;
	std::tie(controlsBuf, std::ignore) =
		IPADataSerializer<ControlList>::serialize(controls
, &controlSerializer_);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), controlsBuf.begin(), controlsBuf.end());


	int _ret = ipc_->sendSync(_ipcInputBuf, &_ipcOutputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call start: " << _ret;
		return;
	}



	const size_t resultStart = 0;


	if (result) {
                *result =
                IPADataSerializer<StartResult>::deserialize(
                	_ipcOutputBuf.data().cbegin() + resultStart,
                	_ipcOutputBuf.data().cend(),
                	&controlSerializer_);
	}

}


void IPAProxyRPi::stop()
{
	if (isolate_)
		return stopIPC();
	else
		return stopThread();
}

void IPAProxyRPi::stopThread()
{
	ASSERT(state_ != ProxyStopping);
	if (state_ != ProxyRunning)
		return;

	state_ = ProxyStopping;

	proxy_.invokeMethod(&ThreadProxy::stop, ConnectionTypeBlocking);

	thread_.exit();
	thread_.wait();

	Thread::current()->dispatchMessages(Message::Type::InvokeMessage, this);

	state_ = ProxyStopped;
}

void IPAProxyRPi::stopIPC()
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Stop), seq_++ };
	IPCMessage _ipcInputBuf(_header);




	int _ret = ipc_->sendSync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call stop: " << _ret;
		return;
	}
}


int32_t IPAProxyRPi::configure(
	const IPACameraSensorInfo &sensorInfo,
	const ConfigParams &params,
	ConfigResult *result)
{
	if (isolate_)
		return configureIPC(sensorInfo, params, result);
	else
		return configureThread(sensorInfo, params, result);
}

int32_t IPAProxyRPi::configureThread(
	const IPACameraSensorInfo &sensorInfo,
	const ConfigParams &params,
	ConfigResult *result)
{
	return ipa_->configure(sensorInfo, params, result);

}

int32_t IPAProxyRPi::configureIPC(
	const IPACameraSensorInfo &sensorInfo,
	const ConfigParams &params,
	ConfigResult *result)
{
	controlSerializer_.reset();
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Configure), seq_++ };
	IPCMessage _ipcInputBuf(_header);
	IPCMessage _ipcOutputBuf;


	std::vector<uint8_t> sensorInfoBuf;
	std::tie(sensorInfoBuf, std::ignore) =
		IPADataSerializer<IPACameraSensorInfo>::serialize(sensorInfo
);
	std::vector<uint8_t> paramsBuf;
	std::vector<SharedFD> paramsFds;
	std::tie(paramsBuf, paramsFds) =
		IPADataSerializer<ConfigParams>::serialize(params
, &controlSerializer_);
	appendPOD<uint32_t>(_ipcInputBuf.data(), sensorInfoBuf.size());
	appendPOD<uint32_t>(_ipcInputBuf.data(), paramsBuf.size());
	appendPOD<uint32_t>(_ipcInputBuf.data(), paramsFds.size());
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), sensorInfoBuf.begin(), sensorInfoBuf.end());
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), paramsBuf.begin(), paramsBuf.end());
	_ipcInputBuf.fds().insert(_ipcInputBuf.fds().end(), paramsFds.begin(), paramsFds.end());


	int _ret = ipc_->sendSync(_ipcInputBuf, &_ipcOutputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call configure: " << _ret;
		return static_cast<int32_t>(_ret);
	}

	int32_t _retValue = IPADataSerializer<int32_t>::deserialize(_ipcOutputBuf.data(), 0);



	const size_t resultStart = 4;


	if (result) {
                *result =
                IPADataSerializer<ConfigResult>::deserialize(
                	_ipcOutputBuf.data().cbegin() + resultStart,
                	_ipcOutputBuf.data().cend(),
                	&controlSerializer_);
	}


	return _retValue;

}


void IPAProxyRPi::mapBuffers(
	const std::vector<libcamera::IPABuffer> &buffers)
{
	if (isolate_)
		return mapBuffersIPC(buffers);
	else
		return mapBuffersThread(buffers);
}

void IPAProxyRPi::mapBuffersThread(
	const std::vector<libcamera::IPABuffer> &buffers)
{
	return ipa_->mapBuffers(buffers);

}

void IPAProxyRPi::mapBuffersIPC(
	const std::vector<libcamera::IPABuffer> &buffers)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::MapBuffers), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> buffersBuf;
	std::vector<SharedFD> buffersFds;
	std::tie(buffersBuf, buffersFds) =
		IPADataSerializer<std::vector<libcamera::IPABuffer>>::serialize(buffers
);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), buffersBuf.begin(), buffersBuf.end());
	_ipcInputBuf.fds().insert(_ipcInputBuf.fds().end(), buffersFds.begin(), buffersFds.end());


	int _ret = ipc_->sendSync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call mapBuffers: " << _ret;
		return;
	}
}


void IPAProxyRPi::unmapBuffers(
	const std::vector<uint32_t> &ids)
{
	if (isolate_)
		return unmapBuffersIPC(ids);
	else
		return unmapBuffersThread(ids);
}

void IPAProxyRPi::unmapBuffersThread(
	const std::vector<uint32_t> &ids)
{
	return ipa_->unmapBuffers(ids);

}

void IPAProxyRPi::unmapBuffersIPC(
	const std::vector<uint32_t> &ids)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::UnmapBuffers), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> idsBuf;
	std::tie(idsBuf, std::ignore) =
		IPADataSerializer<std::vector<uint32_t>>::serialize(ids
);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), idsBuf.begin(), idsBuf.end());


	int _ret = ipc_->sendSync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call unmapBuffers: " << _ret;
		return;
	}
}


void IPAProxyRPi::prepareIsp(
	const PrepareParams &params)
{
	if (isolate_)
		return prepareIspIPC(params);
	else
		return prepareIspThread(params);
}

void IPAProxyRPi::prepareIspThread(
	const PrepareParams &params)
{
	ASSERT(state_ == ProxyRunning);
	proxy_.invokeMethod(&ThreadProxy::prepareIsp, ConnectionTypeQueued, params);
}

void IPAProxyRPi::prepareIspIPC(
	const PrepareParams &params)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::PrepareIsp), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> paramsBuf;
	std::tie(paramsBuf, std::ignore) =
		IPADataSerializer<PrepareParams>::serialize(params
, &controlSerializer_);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), paramsBuf.begin(), paramsBuf.end());


	int _ret = ipc_->sendAsync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call prepareIsp: " << _ret;
		return;
	}
}


void IPAProxyRPi::processStats(
	const ProcessParams &params)
{
	if (isolate_)
		return processStatsIPC(params);
	else
		return processStatsThread(params);
}

void IPAProxyRPi::processStatsThread(
	const ProcessParams &params)
{
	ASSERT(state_ == ProxyRunning);
	proxy_.invokeMethod(&ThreadProxy::processStats, ConnectionTypeQueued, params);
}

void IPAProxyRPi::processStatsIPC(
	const ProcessParams &params)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::ProcessStats), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> paramsBuf;
	std::tie(paramsBuf, std::ignore) =
		IPADataSerializer<ProcessParams>::serialize(params
);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), paramsBuf.begin(), paramsBuf.end());


	int _ret = ipc_->sendAsync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call processStats: " << _ret;
		return;
	}
}




void IPAProxyRPi::prepareIspCompleteThread(
	const BufferIds &buffers,
	const bool stitchSwapBuffers)
{
	ASSERT(state_ != ProxyStopped);
	prepareIspComplete.emit(buffers, stitchSwapBuffers);
}

void IPAProxyRPi::prepareIspCompleteIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{

	[[maybe_unused]] const size_t buffersBufSize = readPOD<uint32_t>(data, 0, data + dataSize);
	[[maybe_unused]] const size_t stitchSwapBuffersBufSize = readPOD<uint32_t>(data, 4, data + dataSize);

	const size_t buffersStart = 8;
	const size_t stitchSwapBuffersStart = buffersStart + buffersBufSize;


	BufferIds buffers =
        IPADataSerializer<BufferIds>::deserialize(
        	data + buffersStart,
        	data + buffersStart + buffersBufSize);

	bool stitchSwapBuffers =
        IPADataSerializer<bool>::deserialize(
        	data + stitchSwapBuffersStart,
        	data + stitchSwapBuffersStart + stitchSwapBuffersBufSize);

	prepareIspComplete.emit(buffers, stitchSwapBuffers);
}

void IPAProxyRPi::processStatsCompleteThread(
	const BufferIds &buffers)
{
	ASSERT(state_ != ProxyStopped);
	processStatsComplete.emit(buffers);
}

void IPAProxyRPi::processStatsCompleteIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t buffersStart = 0;


	BufferIds buffers =
        IPADataSerializer<BufferIds>::deserialize(
        	data + buffersStart,
        	data + dataSize);

	processStatsComplete.emit(buffers);
}

void IPAProxyRPi::metadataReadyThread(
	const ControlList &metadata)
{
	ASSERT(state_ != ProxyStopped);
	metadataReady.emit(metadata);
}

void IPAProxyRPi::metadataReadyIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t metadataStart = 0;


	ControlList metadata =
        IPADataSerializer<ControlList>::deserialize(
        	data + metadataStart,
        	data + dataSize,
        	&controlSerializer_);

	metadataReady.emit(metadata);
}

void IPAProxyRPi::setIspControlsThread(
	const ControlList &controls)
{
	ASSERT(state_ != ProxyStopped);
	setIspControls.emit(controls);
}

void IPAProxyRPi::setIspControlsIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t controlsStart = 0;


	ControlList controls =
        IPADataSerializer<ControlList>::deserialize(
        	data + controlsStart,
        	data + dataSize,
        	&controlSerializer_);

	setIspControls.emit(controls);
}

void IPAProxyRPi::setDelayedControlsThread(
	const ControlList &controls,
	const uint32_t delayContext)
{
	ASSERT(state_ != ProxyStopped);
	setDelayedControls.emit(controls, delayContext);
}

void IPAProxyRPi::setDelayedControlsIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{

	[[maybe_unused]] const size_t controlsBufSize = readPOD<uint32_t>(data, 0, data + dataSize);
	[[maybe_unused]] const size_t delayContextBufSize = readPOD<uint32_t>(data, 4, data + dataSize);

	const size_t controlsStart = 8;
	const size_t delayContextStart = controlsStart + controlsBufSize;


	ControlList controls =
        IPADataSerializer<ControlList>::deserialize(
        	data + controlsStart,
        	data + controlsStart + controlsBufSize,
        	&controlSerializer_);

	uint32_t delayContext =
        IPADataSerializer<uint32_t>::deserialize(
        	data + delayContextStart,
        	data + delayContextStart + delayContextBufSize);

	setDelayedControls.emit(controls, delayContext);
}

void IPAProxyRPi::setLensControlsThread(
	const ControlList &controls)
{
	ASSERT(state_ != ProxyStopped);
	setLensControls.emit(controls);
}

void IPAProxyRPi::setLensControlsIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t controlsStart = 0;


	ControlList controls =
        IPADataSerializer<ControlList>::deserialize(
        	data + controlsStart,
        	data + dataSize,
        	&controlSerializer_);

	setLensControls.emit(controls);
}

void IPAProxyRPi::setCameraTimeoutThread(
	const uint32_t maxFrameLengthMs)
{
	ASSERT(state_ != ProxyStopped);
	setCameraTimeout.emit(maxFrameLengthMs);
}

void IPAProxyRPi::setCameraTimeoutIPC(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t maxFrameLengthMsStart = 0;


	uint32_t maxFrameLengthMs =
        IPADataSerializer<uint32_t>::deserialize(
        	data + maxFrameLengthMsStart,
        	data + dataSize);

	setCameraTimeout.emit(maxFrameLengthMs);
}


} /* namespace RPi */

} /* namespace ipa */

} /* namespace libcamera */
