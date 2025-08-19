/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2020, Google Inc.
 *
 * Image Processing Algorithm interface for raspberrypi
 *
 * This file is auto-generated. Do not edit.
 */

#pragma once


#include <string>
#include <vector>

#include <libcamera/base/flags.h>
#include <libcamera/base/signal.h>

#include <libcamera/controls.h>
#include <libcamera/framebuffer.h>
#include <libcamera/geometry.h>

#include <libcamera/ipa/core_ipa_interface.h>
#include <libcamera/ipa/ipa_interface.h>

namespace libcamera {

namespace ipa {

namespace RPi {



const uint32_t MaxLsGridSize = 0x8000;


enum class _RPiCmd {
	Exit = 0,
	Init = 1,
	Start = 2,
	Stop = 3,
	Configure = 4,
	MapBuffers = 5,
	UnmapBuffers = 6,
	PrepareIsp = 7,
	ProcessStats = 8,
};

enum class _RPiEventCmd {
	PrepareIspComplete = 1,
	ProcessStatsComplete = 2,
	MetadataReady = 3,
	SetIspControls = 4,
	SetDelayedControls = 5,
	SetLensControls = 6,
	SetCameraTimeout = 7,
};


struct SensorConfig
{
public:
#ifndef __DOXYGEN__
	SensorConfig()
		: sensorMetadata(0)
	{
	}

	SensorConfig(uint32_t _sensorMetadata)
		: sensorMetadata(_sensorMetadata)
	{
	}
#endif


	uint32_t sensorMetadata;
};

struct InitParams
{
public:
#ifndef __DOXYGEN__
	InitParams()
		: lensPresent(0), fe(-1), be(-1)
	{
	}

	InitParams(bool _lensPresent, const IPACameraSensorInfo &_sensorInfo, const SharedFD &_fe, const SharedFD &_be)
		: lensPresent(_lensPresent), sensorInfo(_sensorInfo), fe(_fe), be(_be)
	{
	}
#endif


	bool lensPresent;
	IPACameraSensorInfo sensorInfo;
	SharedFD fe;
	SharedFD be;
};

struct InitResult
{
public:
#ifndef __DOXYGEN__
	InitResult()
	{
	}

	InitResult(const SensorConfig &_sensorConfig, const ControlInfoMap &_controlInfo)
		: sensorConfig(_sensorConfig), controlInfo(_controlInfo)
	{
	}
#endif


	SensorConfig sensorConfig;
	ControlInfoMap controlInfo;
};

struct BufferIds
{
public:
#ifndef __DOXYGEN__
	BufferIds()
		: bayer(0), embedded(0), stats(0)
	{
	}

	BufferIds(uint32_t _bayer, uint32_t _embedded, uint32_t _stats)
		: bayer(_bayer), embedded(_embedded), stats(_stats)
	{
	}
#endif


	uint32_t bayer;
	uint32_t embedded;
	uint32_t stats;
};

struct ConfigParams
{
public:
#ifndef __DOXYGEN__
	ConfigParams()
		: transform(0), lsTableHandle(-1)
	{
	}

	ConfigParams(uint32_t _transform, const ControlInfoMap &_sensorControls, const ControlInfoMap &_ispControls, const ControlInfoMap &_lensControls, const SharedFD &_lsTableHandle)
		: transform(_transform), sensorControls(_sensorControls), ispControls(_ispControls), lensControls(_lensControls), lsTableHandle(_lsTableHandle)
	{
	}
#endif


	uint32_t transform;
	ControlInfoMap sensorControls;
	ControlInfoMap ispControls;
	ControlInfoMap lensControls;
	SharedFD lsTableHandle;
};

struct ConfigResult
{
public:
#ifndef __DOXYGEN__
	ConfigResult()
		: modeSensitivity(0)
	{
	}

	ConfigResult(float _modeSensitivity, const ControlInfoMap &_controlInfo, const ControlList &_sensorControls, const ControlList &_lensControls)
		: modeSensitivity(_modeSensitivity), controlInfo(_controlInfo), sensorControls(_sensorControls), lensControls(_lensControls)
	{
	}
#endif


	float modeSensitivity;
	ControlInfoMap controlInfo;
	ControlList sensorControls;
	ControlList lensControls;
};

struct StartResult
{
public:
#ifndef __DOXYGEN__
	StartResult()
		: startupFrameCount(0), invalidFrameCount(0)
	{
	}

	StartResult(const ControlList &_controls, int32_t _startupFrameCount, int32_t _invalidFrameCount)
		: controls(_controls), startupFrameCount(_startupFrameCount), invalidFrameCount(_invalidFrameCount)
	{
	}
#endif


	ControlList controls;
	int32_t startupFrameCount;
	int32_t invalidFrameCount;
};

struct PrepareParams
{
public:
#ifndef __DOXYGEN__
	PrepareParams()
		: ipaContext(0), delayContext(0)
	{
	}

	PrepareParams(const BufferIds &_buffers, const ControlList &_sensorControls, const ControlList &_requestControls, uint32_t _ipaContext, uint32_t _delayContext)
		: buffers(_buffers), sensorControls(_sensorControls), requestControls(_requestControls), ipaContext(_ipaContext), delayContext(_delayContext)
	{
	}
#endif


	BufferIds buffers;
	ControlList sensorControls;
	ControlList requestControls;
	uint32_t ipaContext;
	uint32_t delayContext;
};

struct ProcessParams
{
public:
#ifndef __DOXYGEN__
	ProcessParams()
		: ipaContext(0)
	{
	}

	ProcessParams(const BufferIds &_buffers, uint32_t _ipaContext)
		: buffers(_buffers), ipaContext(_ipaContext)
	{
	}
#endif


	BufferIds buffers;
	uint32_t ipaContext;
};

class IPARPiInterface : public IPAInterface
{
public:

	virtual int32_t init(
		const IPASettings &settings,
		const InitParams &params,
		InitResult *result) = 0;

	virtual void start(
		const ControlList &controls,
		StartResult *result) = 0;

	virtual void stop() = 0;

	virtual int32_t configure(
		const IPACameraSensorInfo &sensorInfo,
		const ConfigParams &params,
		ConfigResult *result) = 0;

	virtual void mapBuffers(
		const std::vector<libcamera::IPABuffer> &buffers) = 0;

	virtual void unmapBuffers(
		const std::vector<uint32_t> &ids) = 0;

	virtual void prepareIsp(
		const PrepareParams &params) = 0;

	virtual void processStats(
		const ProcessParams &params) = 0;

	Signal<const BufferIds &, bool> prepareIspComplete;

	Signal<const BufferIds &> processStatsComplete;

	Signal<const ControlList &> metadataReady;

	Signal<const ControlList &> setIspControls;

	Signal<const ControlList &, uint32_t> setDelayedControls;

	Signal<const ControlList &> setLensControls;

	Signal<uint32_t> setCameraTimeout;
};

} /* namespace RPi */

} /* namespace ipa */

} /* namespace libcamera */
