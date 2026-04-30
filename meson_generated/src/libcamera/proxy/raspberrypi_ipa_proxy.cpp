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


IPAProxyRPiThreaded::IPAProxyRPiThreaded(IPAModule *ipam, const CameraManager &cm)
	: IPAProxyRPi(ipam, cm), thread_("IPAProxyRPi")
{
	LOG(IPAProxy, Debug)
		<< "initializing raspberrypi proxy in thread: loading IPA from "
		<< ipam->path();

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


	ipa_->prepareIspComplete.connect(this, &IPAProxyRPiThreaded::prepareIspCompleteHandler);
	ipa_->processStatsComplete.connect(this, &IPAProxyRPiThreaded::processStatsCompleteHandler);
	ipa_->metadataReady.connect(this, &IPAProxyRPiThreaded::metadataReadyHandler);
	ipa_->setIspControls.connect(this, &IPAProxyRPiThreaded::setIspControlsHandler);
	ipa_->setDelayedControls.connect(this, &IPAProxyRPiThreaded::setDelayedControlsHandler);
	ipa_->setLensControls.connect(this, &IPAProxyRPiThreaded::setLensControlsHandler);
	ipa_->setCameraTimeout.connect(this, &IPAProxyRPiThreaded::setCameraTimeoutHandler);

	valid_ = true;
}

IPAProxyRPiThreaded::~IPAProxyRPiThreaded() = default;


int32_t IPAProxyRPiThreaded::init(
	const IPASettings &settings,
	const InitParams &params,
	InitResult *result)
{
	int32_t _ret = ipa_->init(settings, params, result);

	proxy_.moveToThread(&thread_);

	return _ret;
}

void IPAProxyRPiThreaded::start(
	const ControlList &controls,
	StartResult *result)
{
	state_ = ProxyRunning;
	thread_.start();

	return proxy_.invokeMethod(&ThreadProxy::start, ConnectionTypeBlocking, controls, result);
}

void IPAProxyRPiThreaded::stop()
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

int32_t IPAProxyRPiThreaded::configure(
	const IPACameraSensorInfo &sensorInfo,
	const ConfigParams &params,
	ConfigResult *result)
{
	return ipa_->configure(sensorInfo, params, result);

}

void IPAProxyRPiThreaded::mapBuffers(
	const std::vector<libcamera::IPABuffer> &buffers)
{
	return ipa_->mapBuffers(buffers);

}

void IPAProxyRPiThreaded::unmapBuffers(
	const std::vector<uint32_t> &ids)
{
	return ipa_->unmapBuffers(ids);

}

void IPAProxyRPiThreaded::prepareIsp(
	const PrepareParams &params)
{
	ASSERT(state_ == ProxyRunning);
	proxy_.invokeMethod(&ThreadProxy::prepareIsp, ConnectionTypeQueued, params);
}

void IPAProxyRPiThreaded::processStats(
	const ProcessParams &params)
{
	ASSERT(state_ == ProxyRunning);
	proxy_.invokeMethod(&ThreadProxy::processStats, ConnectionTypeQueued, params);
}



void IPAProxyRPiThreaded::prepareIspCompleteHandler(
	const BufferIds &buffers,
	const bool stitchSwapBuffers)
{
	ASSERT(state_ != ProxyStopped);
	prepareIspComplete.emit(buffers, stitchSwapBuffers);
}

void IPAProxyRPiThreaded::processStatsCompleteHandler(
	const BufferIds &buffers)
{
	ASSERT(state_ != ProxyStopped);
	processStatsComplete.emit(buffers);
}

void IPAProxyRPiThreaded::metadataReadyHandler(
	const ControlList &metadata)
{
	ASSERT(state_ != ProxyStopped);
	metadataReady.emit(metadata);
}

void IPAProxyRPiThreaded::setIspControlsHandler(
	const ControlList &controls)
{
	ASSERT(state_ != ProxyStopped);
	setIspControls.emit(controls);
}

void IPAProxyRPiThreaded::setDelayedControlsHandler(
	const ControlList &controls,
	const uint32_t delayContext)
{
	ASSERT(state_ != ProxyStopped);
	setDelayedControls.emit(controls, delayContext);
}

void IPAProxyRPiThreaded::setLensControlsHandler(
	const ControlList &controls)
{
	ASSERT(state_ != ProxyStopped);
	setLensControls.emit(controls);
}

void IPAProxyRPiThreaded::setCameraTimeoutHandler(
	const uint32_t maxFrameLengthMs)
{
	ASSERT(state_ != ProxyStopped);
	setCameraTimeout.emit(maxFrameLengthMs);
}


/* ========================================================================== */

IPAProxyRPiIsolated::IPAProxyRPiIsolated(IPAModule *ipam, const CameraManager &cm)
	: IPAProxyRPi(ipam, cm),
	  controlSerializer_(ControlSerializer::Role::Proxy), seq_(0)
{
	LOG(IPAProxy, Debug)
		<< "initializing raspberrypi proxy in isolation: loading IPA from "
		<< ipam->path();

	const std::string proxyWorkerPath = resolvePath("raspberrypi_ipa_proxy");
	if (proxyWorkerPath.empty()) {
		LOG(IPAProxy, Error) << "Failed to get proxy worker path";
		return;
	}

	auto ipc = std::make_unique<IPCPipeUnixSocket>(ipam->path().c_str(),
						       proxyWorkerPath.c_str());
	if (!ipc->isConnected()) {
		LOG(IPAProxy, Error) << "Failed to create IPCPipe";
		return;
	}

	ipc->recv.connect(this, &IPAProxyRPiIsolated::recvMessage);

	ipc_ = std::move(ipc);
	valid_ = true;
}

IPAProxyRPiIsolated::~IPAProxyRPiIsolated()
{
	if (ipc_) {
		IPCMessage::Header header =
			{ static_cast<uint32_t>(_RPiCmd::Exit), seq_++ };
		IPCMessage msg(header);
		ipc_->sendAsync(msg);
	}
}


int32_t IPAProxyRPiIsolated::init(
	const IPASettings &settings,
	const InitParams &params,
	InitResult *result)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Init), seq_++ };
	IPCMessage _ipcInputBuf(_header);
	IPCMessage _ipcOutputBuf;


	std::vector<uint8_t> settingsBuf;
	std::tie(settingsBuf, std::ignore) =
		IPADataSerializer<libcamera::IPASettings>::serialize(settings);
	std::vector<uint8_t> paramsBuf;
	std::vector<SharedFD> paramsFds;
	std::tie(paramsBuf, paramsFds) =
		IPADataSerializer<ipa::RPi::InitParams>::serialize(params);
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
                IPADataSerializer<ipa::RPi::InitResult>::deserialize(
                	_ipcOutputBuf.data().cbegin() + resultStart,
                	_ipcOutputBuf.data().cend(),
                	&controlSerializer_);
	}


	return _retValue;

}


void IPAProxyRPiIsolated::start(
	const ControlList &controls,
	StartResult *result)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Start), seq_++ };
	IPCMessage _ipcInputBuf(_header);
	IPCMessage _ipcOutputBuf;


	std::vector<uint8_t> controlsBuf;
	std::tie(controlsBuf, std::ignore) =
		IPADataSerializer<libcamera::ControlList>::serialize(controls, &controlSerializer_);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), controlsBuf.begin(), controlsBuf.end());


	int _ret = ipc_->sendSync(_ipcInputBuf, &_ipcOutputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call start: " << _ret;
		return;
	}



	const size_t resultStart = 0;


	if (result) {
                *result =
                IPADataSerializer<ipa::RPi::StartResult>::deserialize(
                	_ipcOutputBuf.data().cbegin() + resultStart,
                	_ipcOutputBuf.data().cend(),
                	&controlSerializer_);
	}

}


void IPAProxyRPiIsolated::stop()
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::Stop), seq_++ };
	IPCMessage _ipcInputBuf(_header);




	int _ret = ipc_->sendSync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call stop: " << _ret;
		return;
	}
}


int32_t IPAProxyRPiIsolated::configure(
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
		IPADataSerializer<libcamera::IPACameraSensorInfo>::serialize(sensorInfo);
	std::vector<uint8_t> paramsBuf;
	std::vector<SharedFD> paramsFds;
	std::tie(paramsBuf, paramsFds) =
		IPADataSerializer<ipa::RPi::ConfigParams>::serialize(params, &controlSerializer_);
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
                IPADataSerializer<ipa::RPi::ConfigResult>::deserialize(
                	_ipcOutputBuf.data().cbegin() + resultStart,
                	_ipcOutputBuf.data().cend(),
                	&controlSerializer_);
	}


	return _retValue;

}


void IPAProxyRPiIsolated::mapBuffers(
	const std::vector<libcamera::IPABuffer> &buffers)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::MapBuffers), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> buffersBuf;
	std::vector<SharedFD> buffersFds;
	std::tie(buffersBuf, buffersFds) =
		IPADataSerializer<std::vector<libcamera::IPABuffer>>::serialize(buffers);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), buffersBuf.begin(), buffersBuf.end());
	_ipcInputBuf.fds().insert(_ipcInputBuf.fds().end(), buffersFds.begin(), buffersFds.end());


	int _ret = ipc_->sendSync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call mapBuffers: " << _ret;
		return;
	}
}


void IPAProxyRPiIsolated::unmapBuffers(
	const std::vector<uint32_t> &ids)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::UnmapBuffers), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> idsBuf;
	std::tie(idsBuf, std::ignore) =
		IPADataSerializer<std::vector<uint32_t>>::serialize(ids);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), idsBuf.begin(), idsBuf.end());


	int _ret = ipc_->sendSync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call unmapBuffers: " << _ret;
		return;
	}
}


void IPAProxyRPiIsolated::prepareIsp(
	const PrepareParams &params)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::PrepareIsp), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> paramsBuf;
	std::tie(paramsBuf, std::ignore) =
		IPADataSerializer<ipa::RPi::PrepareParams>::serialize(params, &controlSerializer_);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), paramsBuf.begin(), paramsBuf.end());


	int _ret = ipc_->sendAsync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call prepareIsp: " << _ret;
		return;
	}
}


void IPAProxyRPiIsolated::processStats(
	const ProcessParams &params)
{
	IPCMessage::Header _header = { static_cast<uint32_t>(_RPiCmd::ProcessStats), seq_++ };
	IPCMessage _ipcInputBuf(_header);


	std::vector<uint8_t> paramsBuf;
	std::tie(paramsBuf, std::ignore) =
		IPADataSerializer<ipa::RPi::ProcessParams>::serialize(params);
	_ipcInputBuf.data().insert(_ipcInputBuf.data().end(), paramsBuf.begin(), paramsBuf.end());


	int _ret = ipc_->sendAsync(_ipcInputBuf);
	if (_ret < 0) {
		LOG(IPAProxy, Error) << "Failed to call processStats: " << _ret;
		return;
	}
}



void IPAProxyRPiIsolated::recvMessage(const IPCMessage &data)
{
	size_t dataSize = data.data().size();
	_RPiEventCmd _cmd = static_cast<_RPiEventCmd>(data.header().cmd);

	switch (_cmd) {
	case _RPiEventCmd::PrepareIspComplete: {
		prepareIspCompleteHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::ProcessStatsComplete: {
		processStatsCompleteHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::MetadataReady: {
		metadataReadyHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetIspControls: {
		setIspControlsHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetDelayedControls: {
		setDelayedControlsHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetLensControls: {
		setLensControlsHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	case _RPiEventCmd::SetCameraTimeout: {
		setCameraTimeoutHandler(data.data().cbegin(), dataSize, data.fds());
		break;
	}
	default:
		LOG(IPAProxy, Error) << "Unknown command " << static_cast<uint32_t>(_cmd);
	}
}


void IPAProxyRPiIsolated::prepareIspCompleteHandler(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{

	[[maybe_unused]] const size_t buffersBufSize = readPOD<uint32_t>(data, 0, data + dataSize);
	[[maybe_unused]] const size_t stitchSwapBuffersBufSize = readPOD<uint32_t>(data, 4, data + dataSize);

	const size_t buffersStart = 8;
	const size_t stitchSwapBuffersStart = buffersStart + buffersBufSize;


	BufferIds buffers =
        IPADataSerializer<ipa::RPi::BufferIds>::deserialize(
        	data + buffersStart,
        	data + buffersStart + buffersBufSize);

	bool stitchSwapBuffers =
        IPADataSerializer<bool>::deserialize(
        	data + stitchSwapBuffersStart,
        	data + stitchSwapBuffersStart + stitchSwapBuffersBufSize);

	prepareIspComplete.emit(buffers, stitchSwapBuffers);
}

void IPAProxyRPiIsolated::processStatsCompleteHandler(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t buffersStart = 0;


	BufferIds buffers =
        IPADataSerializer<ipa::RPi::BufferIds>::deserialize(
        	data + buffersStart,
        	data + dataSize);

	processStatsComplete.emit(buffers);
}

void IPAProxyRPiIsolated::metadataReadyHandler(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t metadataStart = 0;


	ControlList metadata =
        IPADataSerializer<libcamera::ControlList>::deserialize(
        	data + metadataStart,
        	data + dataSize,
        	&controlSerializer_);

	metadataReady.emit(metadata);
}

void IPAProxyRPiIsolated::setIspControlsHandler(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t controlsStart = 0;


	ControlList controls =
        IPADataSerializer<libcamera::ControlList>::deserialize(
        	data + controlsStart,
        	data + dataSize,
        	&controlSerializer_);

	setIspControls.emit(controls);
}

void IPAProxyRPiIsolated::setDelayedControlsHandler(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{

	[[maybe_unused]] const size_t controlsBufSize = readPOD<uint32_t>(data, 0, data + dataSize);
	[[maybe_unused]] const size_t delayContextBufSize = readPOD<uint32_t>(data, 4, data + dataSize);

	const size_t controlsStart = 8;
	const size_t delayContextStart = controlsStart + controlsBufSize;


	ControlList controls =
        IPADataSerializer<libcamera::ControlList>::deserialize(
        	data + controlsStart,
        	data + controlsStart + controlsBufSize,
        	&controlSerializer_);

	uint32_t delayContext =
        IPADataSerializer<uint32_t>::deserialize(
        	data + delayContextStart,
        	data + delayContextStart + delayContextBufSize);

	setDelayedControls.emit(controls, delayContext);
}

void IPAProxyRPiIsolated::setLensControlsHandler(
	[[maybe_unused]] std::vector<uint8_t>::const_iterator data,
	[[maybe_unused]] size_t dataSize,
	[[maybe_unused]] const std::vector<SharedFD> &fds)
{


	const size_t controlsStart = 0;


	ControlList controls =
        IPADataSerializer<libcamera::ControlList>::deserialize(
        	data + controlsStart,
        	data + dataSize,
        	&controlSerializer_);

	setLensControls.emit(controls);
}

void IPAProxyRPiIsolated::setCameraTimeoutHandler(
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