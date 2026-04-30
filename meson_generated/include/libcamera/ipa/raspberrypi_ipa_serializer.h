/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2020, Google Inc.
 *
 * Image Processing Algorithm data serializer for raspberrypi
 *
 * This file is auto-generated. Do not edit.
 */

#pragma once

#include <tuple>
#include <vector>

#include <libcamera/ipa/raspberrypi_ipa_interface.h>
#include <libcamera/ipa/core_ipa_serializer.h>

#include "libcamera/internal/control_serializer.h"
#include "libcamera/internal/ipa_data_serializer.h"

namespace libcamera {

LOG_DECLARE_CATEGORY(IPADataSerializer)

template<>
class IPADataSerializer<ipa::RPi::SensorConfig>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::SensorConfig &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> sensorMetadata;
		std::tie(sensorMetadata, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.sensorMetadata);
		retData.insert(retData.end(), sensorMetadata.begin(), sensorMetadata.end());

		return {retData, {}};
	}

	static ipa::RPi::SensorConfig
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::SensorConfig>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::SensorConfig
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		ipa::RPi::SensorConfig ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorMetadata"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.sensorMetadata = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static ipa::RPi::SensorConfig
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::SensorConfig>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::SensorConfig
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::SensorConfig>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<ipa::RPi::InitParams>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::InitParams &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;
		std::vector<SharedFD> retFds;

		std::vector<uint8_t> lensPresent;
		std::tie(lensPresent, std::ignore) =
			IPADataSerializer<bool>::serialize(data.lensPresent);
		retData.insert(retData.end(), lensPresent.begin(), lensPresent.end());

		std::vector<uint8_t> sensorInfo;
		std::tie(sensorInfo, std::ignore) =
			IPADataSerializer<libcamera::IPACameraSensorInfo>::serialize(data.sensorInfo, cs);
		appendPOD<uint32_t>(retData, sensorInfo.size());
		retData.insert(retData.end(), sensorInfo.begin(), sensorInfo.end());

		std::vector<uint8_t> controllerMinFrameDurationUs;
		std::tie(controllerMinFrameDurationUs, std::ignore) =
			IPADataSerializer<float>::serialize(data.controllerMinFrameDurationUs);
		retData.insert(retData.end(), controllerMinFrameDurationUs.begin(), controllerMinFrameDurationUs.end());

		std::vector<uint8_t> fe;
		std::vector<SharedFD> feFds;
		std::tie(fe, feFds) =
			IPADataSerializer<SharedFD>::serialize(data.fe);
		retData.insert(retData.end(), fe.begin(), fe.end());
		retFds.insert(retFds.end(), feFds.begin(), feFds.end());

		std::vector<uint8_t> be;
		std::vector<SharedFD> beFds;
		std::tie(be, beFds) =
			IPADataSerializer<SharedFD>::serialize(data.be);
		retData.insert(retData.end(), be.begin(), be.end());
		retFds.insert(retFds.end(), beFds.begin(), beFds.end());

		return {retData, retFds};
	}

	static ipa::RPi::InitParams
	deserialize(std::vector<uint8_t> &data,
		    std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::InitParams>::deserialize(data.cbegin(), data.cend(), fds.cbegin(), fds.cend(), cs);
	}


	static ipa::RPi::InitParams
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    std::vector<SharedFD>::const_iterator fdsBegin,
		    std::vector<SharedFD>::const_iterator fdsEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		ipa::RPi::InitParams ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;
		std::vector<SharedFD>::const_iterator n = fdsBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);
		[[maybe_unused]] size_t fdsSize = std::distance(fdsBegin, fdsEnd);

		if (dataSize < 1) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "lensPresent"
				<< ": not enough data, expected "
				<< (1) << ", got " << (dataSize);
			return ret;
		}
		ret.lensPresent = IPADataSerializer<bool>::deserialize(m, m + 1);
		m += 1;
		dataSize -= 1;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorInfoSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sensorInfoSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sensorInfoSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorInfo"
				<< ": not enough data, expected "
				<< (sensorInfoSize) << ", got " << (dataSize);
			return ret;
		}
		ret.sensorInfo =
			IPADataSerializer<libcamera::IPACameraSensorInfo>::deserialize(m, m + sensorInfoSize, cs);
		m += sensorInfoSize;
		dataSize -= sensorInfoSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controllerMinFrameDurationUs"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.controllerMinFrameDurationUs = IPADataSerializer<float>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "fe"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.fe = IPADataSerializer<SharedFD>::deserialize(m, m + 4, n, n + 1, cs);
		m += 4;
		dataSize -= 4;
		n += ret.fe.isValid() ? 1 : 0;
		fdsSize -= ret.fe.isValid() ? 1 : 0;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "be"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.be = IPADataSerializer<SharedFD>::deserialize(m, m + 4, n, n + 1, cs);

		return ret;
	}
};

template<>
class IPADataSerializer<ipa::RPi::InitResult>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::InitResult &data,
		  ControlSerializer *cs)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> sensorConfig;
		std::tie(sensorConfig, std::ignore) =
			IPADataSerializer<ipa::RPi::SensorConfig>::serialize(data.sensorConfig, cs);
		appendPOD<uint32_t>(retData, sensorConfig.size());
		retData.insert(retData.end(), sensorConfig.begin(), sensorConfig.end());

		if (data.controlInfo.size() > 0) {
			std::vector<uint8_t> controlInfo;
			std::tie(controlInfo, std::ignore) =
				IPADataSerializer<ControlInfoMap>::serialize(data.controlInfo, cs);
			appendPOD<uint32_t>(retData, controlInfo.size());
			retData.insert(retData.end(), controlInfo.begin(), controlInfo.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		return {retData, {}};
	}

	static ipa::RPi::InitResult
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs)
	{
		return IPADataSerializer<ipa::RPi::InitResult>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::InitResult
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    ControlSerializer *cs)
	{
		ipa::RPi::InitResult ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorConfigSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sensorConfigSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sensorConfigSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorConfig"
				<< ": not enough data, expected "
				<< (sensorConfigSize) << ", got " << (dataSize);
			return ret;
		}
		ret.sensorConfig =
			IPADataSerializer<ipa::RPi::SensorConfig>::deserialize(m, m + sensorConfigSize, cs);
		m += sensorConfigSize;
		dataSize -= sensorConfigSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controlInfoSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t controlInfoSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < controlInfoSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controlInfo"
				<< ": not enough data, expected "
				<< (controlInfoSize) << ", got " << (dataSize);
			return ret;
		}
		if (controlInfoSize > 0)
			ret.controlInfo =
				IPADataSerializer<ControlInfoMap>::deserialize(m, m + controlInfoSize, cs);

		return ret;
	}

	static ipa::RPi::InitResult
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::InitResult>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::InitResult
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::InitResult>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<ipa::RPi::BufferIds>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::BufferIds &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> bayer;
		std::tie(bayer, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.bayer);
		retData.insert(retData.end(), bayer.begin(), bayer.end());

		std::vector<uint8_t> embedded;
		std::tie(embedded, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.embedded);
		retData.insert(retData.end(), embedded.begin(), embedded.end());

		std::vector<uint8_t> stats;
		std::tie(stats, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.stats);
		retData.insert(retData.end(), stats.begin(), stats.end());

		return {retData, {}};
	}

	static ipa::RPi::BufferIds
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::BufferIds>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::BufferIds
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		ipa::RPi::BufferIds ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "bayer"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.bayer = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "embedded"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.embedded = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "stats"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.stats = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static ipa::RPi::BufferIds
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::BufferIds>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::BufferIds
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::BufferIds>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<ipa::RPi::ConfigParams>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::ConfigParams &data,
		  ControlSerializer *cs)
	{
		std::vector<uint8_t> retData;
		std::vector<SharedFD> retFds;

		std::vector<uint8_t> transform;
		std::tie(transform, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.transform);
		retData.insert(retData.end(), transform.begin(), transform.end());

		if (data.sensorControls.size() > 0) {
			std::vector<uint8_t> sensorControls;
			std::tie(sensorControls, std::ignore) =
				IPADataSerializer<ControlInfoMap>::serialize(data.sensorControls, cs);
			appendPOD<uint32_t>(retData, sensorControls.size());
			retData.insert(retData.end(), sensorControls.begin(), sensorControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		if (data.ispControls.size() > 0) {
			std::vector<uint8_t> ispControls;
			std::tie(ispControls, std::ignore) =
				IPADataSerializer<ControlInfoMap>::serialize(data.ispControls, cs);
			appendPOD<uint32_t>(retData, ispControls.size());
			retData.insert(retData.end(), ispControls.begin(), ispControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		if (data.lensControls.size() > 0) {
			std::vector<uint8_t> lensControls;
			std::tie(lensControls, std::ignore) =
				IPADataSerializer<ControlInfoMap>::serialize(data.lensControls, cs);
			appendPOD<uint32_t>(retData, lensControls.size());
			retData.insert(retData.end(), lensControls.begin(), lensControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		std::vector<uint8_t> lsTableHandle;
		std::vector<SharedFD> lsTableHandleFds;
		std::tie(lsTableHandle, lsTableHandleFds) =
			IPADataSerializer<SharedFD>::serialize(data.lsTableHandle);
		retData.insert(retData.end(), lsTableHandle.begin(), lsTableHandle.end());
		retFds.insert(retFds.end(), lsTableHandleFds.begin(), lsTableHandleFds.end());

		return {retData, retFds};
	}

	static ipa::RPi::ConfigParams
	deserialize(std::vector<uint8_t> &data,
		    std::vector<SharedFD> &fds,
		    ControlSerializer *cs)
	{
		return IPADataSerializer<ipa::RPi::ConfigParams>::deserialize(data.cbegin(), data.cend(), fds.cbegin(), fds.cend(), cs);
	}


	static ipa::RPi::ConfigParams
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    std::vector<SharedFD>::const_iterator fdsBegin,
		    std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs)
	{
		ipa::RPi::ConfigParams ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;
		std::vector<SharedFD>::const_iterator n = fdsBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);
		[[maybe_unused]] size_t fdsSize = std::distance(fdsBegin, fdsEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "transform"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.transform = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sensorControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sensorControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorControls"
				<< ": not enough data, expected "
				<< (sensorControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (sensorControlsSize > 0)
			ret.sensorControls =
				IPADataSerializer<ControlInfoMap>::deserialize(m, m + sensorControlsSize, cs);
		m += sensorControlsSize;
		dataSize -= sensorControlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "ispControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t ispControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < ispControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "ispControls"
				<< ": not enough data, expected "
				<< (ispControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (ispControlsSize > 0)
			ret.ispControls =
				IPADataSerializer<ControlInfoMap>::deserialize(m, m + ispControlsSize, cs);
		m += ispControlsSize;
		dataSize -= ispControlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "lensControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t lensControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < lensControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "lensControls"
				<< ": not enough data, expected "
				<< (lensControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (lensControlsSize > 0)
			ret.lensControls =
				IPADataSerializer<ControlInfoMap>::deserialize(m, m + lensControlsSize, cs);
		m += lensControlsSize;
		dataSize -= lensControlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "lsTableHandle"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.lsTableHandle = IPADataSerializer<SharedFD>::deserialize(m, m + 4, n, n + 1, cs);

		return ret;
	}
};

template<>
class IPADataSerializer<ipa::RPi::ConfigResult>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::ConfigResult &data,
		  ControlSerializer *cs)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> modeSensitivity;
		std::tie(modeSensitivity, std::ignore) =
			IPADataSerializer<float>::serialize(data.modeSensitivity);
		retData.insert(retData.end(), modeSensitivity.begin(), modeSensitivity.end());

		if (data.controlInfo.size() > 0) {
			std::vector<uint8_t> controlInfo;
			std::tie(controlInfo, std::ignore) =
				IPADataSerializer<ControlInfoMap>::serialize(data.controlInfo, cs);
			appendPOD<uint32_t>(retData, controlInfo.size());
			retData.insert(retData.end(), controlInfo.begin(), controlInfo.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		if (data.sensorControls.size() > 0) {
			std::vector<uint8_t> sensorControls;
			std::tie(sensorControls, std::ignore) =
				IPADataSerializer<ControlList>::serialize(data.sensorControls, cs);
			appendPOD<uint32_t>(retData, sensorControls.size());
			retData.insert(retData.end(), sensorControls.begin(), sensorControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		if (data.lensControls.size() > 0) {
			std::vector<uint8_t> lensControls;
			std::tie(lensControls, std::ignore) =
				IPADataSerializer<ControlList>::serialize(data.lensControls, cs);
			appendPOD<uint32_t>(retData, lensControls.size());
			retData.insert(retData.end(), lensControls.begin(), lensControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		return {retData, {}};
	}

	static ipa::RPi::ConfigResult
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs)
	{
		return IPADataSerializer<ipa::RPi::ConfigResult>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::ConfigResult
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    ControlSerializer *cs)
	{
		ipa::RPi::ConfigResult ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "modeSensitivity"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.modeSensitivity = IPADataSerializer<float>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controlInfoSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t controlInfoSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < controlInfoSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controlInfo"
				<< ": not enough data, expected "
				<< (controlInfoSize) << ", got " << (dataSize);
			return ret;
		}
		if (controlInfoSize > 0)
			ret.controlInfo =
				IPADataSerializer<ControlInfoMap>::deserialize(m, m + controlInfoSize, cs);
		m += controlInfoSize;
		dataSize -= controlInfoSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sensorControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sensorControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorControls"
				<< ": not enough data, expected "
				<< (sensorControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (sensorControlsSize > 0)
			ret.sensorControls =
				IPADataSerializer<ControlList>::deserialize(m, m + sensorControlsSize, cs);
		m += sensorControlsSize;
		dataSize -= sensorControlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "lensControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t lensControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < lensControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "lensControls"
				<< ": not enough data, expected "
				<< (lensControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (lensControlsSize > 0)
			ret.lensControls =
				IPADataSerializer<ControlList>::deserialize(m, m + lensControlsSize, cs);

		return ret;
	}

	static ipa::RPi::ConfigResult
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::ConfigResult>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::ConfigResult
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::ConfigResult>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<ipa::RPi::StartResult>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::StartResult &data,
		  ControlSerializer *cs)
	{
		std::vector<uint8_t> retData;

		if (data.controls.size() > 0) {
			std::vector<uint8_t> controls;
			std::tie(controls, std::ignore) =
				IPADataSerializer<ControlList>::serialize(data.controls, cs);
			appendPOD<uint32_t>(retData, controls.size());
			retData.insert(retData.end(), controls.begin(), controls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		std::vector<uint8_t> startupFrameCount;
		std::tie(startupFrameCount, std::ignore) =
			IPADataSerializer<int32_t>::serialize(data.startupFrameCount);
		retData.insert(retData.end(), startupFrameCount.begin(), startupFrameCount.end());

		std::vector<uint8_t> invalidFrameCount;
		std::tie(invalidFrameCount, std::ignore) =
			IPADataSerializer<int32_t>::serialize(data.invalidFrameCount);
		retData.insert(retData.end(), invalidFrameCount.begin(), invalidFrameCount.end());

		return {retData, {}};
	}

	static ipa::RPi::StartResult
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs)
	{
		return IPADataSerializer<ipa::RPi::StartResult>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::StartResult
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    ControlSerializer *cs)
	{
		ipa::RPi::StartResult ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t controlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < controlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "controls"
				<< ": not enough data, expected "
				<< (controlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (controlsSize > 0)
			ret.controls =
				IPADataSerializer<ControlList>::deserialize(m, m + controlsSize, cs);
		m += controlsSize;
		dataSize -= controlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "startupFrameCount"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.startupFrameCount = IPADataSerializer<int32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "invalidFrameCount"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.invalidFrameCount = IPADataSerializer<int32_t>::deserialize(m, m + 4);

		return ret;
	}

	static ipa::RPi::StartResult
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::StartResult>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::StartResult
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::StartResult>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<ipa::RPi::PrepareParams>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::PrepareParams &data,
		  ControlSerializer *cs)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> buffers;
		std::tie(buffers, std::ignore) =
			IPADataSerializer<ipa::RPi::BufferIds>::serialize(data.buffers, cs);
		appendPOD<uint32_t>(retData, buffers.size());
		retData.insert(retData.end(), buffers.begin(), buffers.end());

		if (data.sensorControls.size() > 0) {
			std::vector<uint8_t> sensorControls;
			std::tie(sensorControls, std::ignore) =
				IPADataSerializer<ControlList>::serialize(data.sensorControls, cs);
			appendPOD<uint32_t>(retData, sensorControls.size());
			retData.insert(retData.end(), sensorControls.begin(), sensorControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		if (data.requestControls.size() > 0) {
			std::vector<uint8_t> requestControls;
			std::tie(requestControls, std::ignore) =
				IPADataSerializer<ControlList>::serialize(data.requestControls, cs);
			appendPOD<uint32_t>(retData, requestControls.size());
			retData.insert(retData.end(), requestControls.begin(), requestControls.end());
		} else {
			appendPOD<uint32_t>(retData, 0);
		}

		std::vector<uint8_t> ipaContext;
		std::tie(ipaContext, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.ipaContext);
		retData.insert(retData.end(), ipaContext.begin(), ipaContext.end());

		std::vector<uint8_t> delayContext;
		std::tie(delayContext, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.delayContext);
		retData.insert(retData.end(), delayContext.begin(), delayContext.end());

		return {retData, {}};
	}

	static ipa::RPi::PrepareParams
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs)
	{
		return IPADataSerializer<ipa::RPi::PrepareParams>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::PrepareParams
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    ControlSerializer *cs)
	{
		ipa::RPi::PrepareParams ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "buffersSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t buffersSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < buffersSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "buffers"
				<< ": not enough data, expected "
				<< (buffersSize) << ", got " << (dataSize);
			return ret;
		}
		ret.buffers =
			IPADataSerializer<ipa::RPi::BufferIds>::deserialize(m, m + buffersSize, cs);
		m += buffersSize;
		dataSize -= buffersSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sensorControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sensorControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorControls"
				<< ": not enough data, expected "
				<< (sensorControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (sensorControlsSize > 0)
			ret.sensorControls =
				IPADataSerializer<ControlList>::deserialize(m, m + sensorControlsSize, cs);
		m += sensorControlsSize;
		dataSize -= sensorControlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "requestControlsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t requestControlsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < requestControlsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "requestControls"
				<< ": not enough data, expected "
				<< (requestControlsSize) << ", got " << (dataSize);
			return ret;
		}
		if (requestControlsSize > 0)
			ret.requestControls =
				IPADataSerializer<ControlList>::deserialize(m, m + requestControlsSize, cs);
		m += requestControlsSize;
		dataSize -= requestControlsSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "ipaContext"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.ipaContext = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "delayContext"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.delayContext = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static ipa::RPi::PrepareParams
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::PrepareParams>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::PrepareParams
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::PrepareParams>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<ipa::RPi::ProcessParams>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const ipa::RPi::ProcessParams &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> buffers;
		std::tie(buffers, std::ignore) =
			IPADataSerializer<ipa::RPi::BufferIds>::serialize(data.buffers, cs);
		appendPOD<uint32_t>(retData, buffers.size());
		retData.insert(retData.end(), buffers.begin(), buffers.end());

		std::vector<uint8_t> ipaContext;
		std::tie(ipaContext, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.ipaContext);
		retData.insert(retData.end(), ipaContext.begin(), ipaContext.end());

		return {retData, {}};
	}

	static ipa::RPi::ProcessParams
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::ProcessParams>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static ipa::RPi::ProcessParams
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		ipa::RPi::ProcessParams ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "buffersSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t buffersSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < buffersSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "buffers"
				<< ": not enough data, expected "
				<< (buffersSize) << ", got " << (dataSize);
			return ret;
		}
		ret.buffers =
			IPADataSerializer<ipa::RPi::BufferIds>::deserialize(m, m + buffersSize, cs);
		m += buffersSize;
		dataSize -= buffersSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "ipaContext"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.ipaContext = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static ipa::RPi::ProcessParams
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::ProcessParams>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static ipa::RPi::ProcessParams
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<ipa::RPi::ProcessParams>::deserialize(dataBegin, dataEnd, cs);
	}
};


} /* namespace libcamera */