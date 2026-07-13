/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2020, Google Inc.
 *
 * Data serializer for core libcamera definitions for IPA
 *
 * This file is auto-generated. Do not edit.
 */

#pragma once

#include <tuple>
#include <vector>

#include <libcamera/ipa/core_ipa_interface.h>

#include "libcamera/internal/control_serializer.h"
#include "libcamera/internal/ipa_data_serializer.h"

namespace libcamera {

LOG_DECLARE_CATEGORY(IPADataSerializer)

template<>
class IPADataSerializer<Point>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::Point &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> x;
		std::tie(x, std::ignore) =
			IPADataSerializer<int32_t>::serialize(data.x);
		retData.insert(retData.end(), x.begin(), x.end());

		std::vector<uint8_t> y;
		std::tie(y, std::ignore) =
			IPADataSerializer<int32_t>::serialize(data.y);
		retData.insert(retData.end(), y.begin(), y.end());

		return {retData, {}};
	}

	static libcamera::Point
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Point>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::Point
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::Point ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "x"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.x = IPADataSerializer<int32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "y"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.y = IPADataSerializer<int32_t>::deserialize(m, m + 4);

		return ret;
	}

	static libcamera::Point
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Point>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::Point
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Point>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<Size>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::Size &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> width;
		std::tie(width, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.width);
		retData.insert(retData.end(), width.begin(), width.end());

		std::vector<uint8_t> height;
		std::tie(height, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.height);
		retData.insert(retData.end(), height.begin(), height.end());

		return {retData, {}};
	}

	static libcamera::Size
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Size>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::Size
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::Size ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "width"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.width = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "height"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.height = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static libcamera::Size
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Size>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::Size
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Size>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<SizeRange>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::SizeRange &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> min;
		std::tie(min, std::ignore) =
			IPADataSerializer<libcamera::Size>::serialize(data.min, cs);
		appendPOD<uint32_t>(retData, min.size());
		retData.insert(retData.end(), min.begin(), min.end());

		std::vector<uint8_t> max;
		std::tie(max, std::ignore) =
			IPADataSerializer<libcamera::Size>::serialize(data.max, cs);
		appendPOD<uint32_t>(retData, max.size());
		retData.insert(retData.end(), max.begin(), max.end());

		std::vector<uint8_t> hStep;
		std::tie(hStep, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.hStep);
		retData.insert(retData.end(), hStep.begin(), hStep.end());

		std::vector<uint8_t> vStep;
		std::tie(vStep, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.vStep);
		retData.insert(retData.end(), vStep.begin(), vStep.end());

		return {retData, {}};
	}

	static libcamera::SizeRange
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::SizeRange>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::SizeRange
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::SizeRange ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "minSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t minSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < minSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "min"
				<< ": not enough data, expected "
				<< (minSize) << ", got " << (dataSize);
			return ret;
		}
		ret.min =
			IPADataSerializer<libcamera::Size>::deserialize(m, m + minSize, cs);
		m += minSize;
		dataSize -= minSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "maxSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t maxSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < maxSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "max"
				<< ": not enough data, expected "
				<< (maxSize) << ", got " << (dataSize);
			return ret;
		}
		ret.max =
			IPADataSerializer<libcamera::Size>::deserialize(m, m + maxSize, cs);
		m += maxSize;
		dataSize -= maxSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "hStep"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.hStep = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "vStep"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.vStep = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static libcamera::SizeRange
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::SizeRange>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::SizeRange
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::SizeRange>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<Rectangle>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::Rectangle &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> x;
		std::tie(x, std::ignore) =
			IPADataSerializer<int32_t>::serialize(data.x);
		retData.insert(retData.end(), x.begin(), x.end());

		std::vector<uint8_t> y;
		std::tie(y, std::ignore) =
			IPADataSerializer<int32_t>::serialize(data.y);
		retData.insert(retData.end(), y.begin(), y.end());

		std::vector<uint8_t> width;
		std::tie(width, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.width);
		retData.insert(retData.end(), width.begin(), width.end());

		std::vector<uint8_t> height;
		std::tie(height, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.height);
		retData.insert(retData.end(), height.begin(), height.end());

		return {retData, {}};
	}

	static libcamera::Rectangle
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Rectangle>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::Rectangle
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::Rectangle ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "x"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.x = IPADataSerializer<int32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "y"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.y = IPADataSerializer<int32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "width"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.width = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "height"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.height = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static libcamera::Rectangle
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Rectangle>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::Rectangle
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::Rectangle>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<IPACameraSensorInfo>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::IPACameraSensorInfo &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> model;
		std::tie(model, std::ignore) =
			IPADataSerializer<std::string>::serialize(data.model);
		appendPOD<uint32_t>(retData, model.size());
		retData.insert(retData.end(), model.begin(), model.end());

		std::vector<uint8_t> bitsPerPixel;
		std::tie(bitsPerPixel, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.bitsPerPixel);
		retData.insert(retData.end(), bitsPerPixel.begin(), bitsPerPixel.end());

		std::vector<uint8_t> cfaPattern;
		std::tie(cfaPattern, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.cfaPattern);
		retData.insert(retData.end(), cfaPattern.begin(), cfaPattern.end());

		std::vector<uint8_t> activeAreaSize;
		std::tie(activeAreaSize, std::ignore) =
			IPADataSerializer<libcamera::Size>::serialize(data.activeAreaSize, cs);
		appendPOD<uint32_t>(retData, activeAreaSize.size());
		retData.insert(retData.end(), activeAreaSize.begin(), activeAreaSize.end());

		std::vector<uint8_t> analogCrop;
		std::tie(analogCrop, std::ignore) =
			IPADataSerializer<libcamera::Rectangle>::serialize(data.analogCrop, cs);
		appendPOD<uint32_t>(retData, analogCrop.size());
		retData.insert(retData.end(), analogCrop.begin(), analogCrop.end());

		std::vector<uint8_t> outputSize;
		std::tie(outputSize, std::ignore) =
			IPADataSerializer<libcamera::Size>::serialize(data.outputSize, cs);
		appendPOD<uint32_t>(retData, outputSize.size());
		retData.insert(retData.end(), outputSize.begin(), outputSize.end());

		std::vector<uint8_t> pixelRate;
		std::tie(pixelRate, std::ignore) =
			IPADataSerializer<uint64_t>::serialize(data.pixelRate);
		retData.insert(retData.end(), pixelRate.begin(), pixelRate.end());

		std::vector<uint8_t> minLineLength;
		std::tie(minLineLength, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.minLineLength);
		retData.insert(retData.end(), minLineLength.begin(), minLineLength.end());

		std::vector<uint8_t> maxLineLength;
		std::tie(maxLineLength, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.maxLineLength);
		retData.insert(retData.end(), maxLineLength.begin(), maxLineLength.end());

		std::vector<uint8_t> minFrameLength;
		std::tie(minFrameLength, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.minFrameLength);
		retData.insert(retData.end(), minFrameLength.begin(), minFrameLength.end());

		std::vector<uint8_t> maxFrameLength;
		std::tie(maxFrameLength, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.maxFrameLength);
		retData.insert(retData.end(), maxFrameLength.begin(), maxFrameLength.end());

		return {retData, {}};
	}

	static libcamera::IPACameraSensorInfo
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPACameraSensorInfo>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::IPACameraSensorInfo
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::IPACameraSensorInfo ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "modelSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t modelSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < modelSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "model"
				<< ": not enough data, expected "
				<< (modelSize) << ", got " << (dataSize);
			return ret;
		}
		ret.model =
			IPADataSerializer<std::string>::deserialize(m, m + modelSize);
		m += modelSize;
		dataSize -= modelSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "bitsPerPixel"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.bitsPerPixel = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "cfaPattern"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.cfaPattern = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "activeAreaSizeSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t activeAreaSizeSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < activeAreaSizeSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "activeAreaSize"
				<< ": not enough data, expected "
				<< (activeAreaSizeSize) << ", got " << (dataSize);
			return ret;
		}
		ret.activeAreaSize =
			IPADataSerializer<libcamera::Size>::deserialize(m, m + activeAreaSizeSize, cs);
		m += activeAreaSizeSize;
		dataSize -= activeAreaSizeSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "analogCropSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t analogCropSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < analogCropSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "analogCrop"
				<< ": not enough data, expected "
				<< (analogCropSize) << ", got " << (dataSize);
			return ret;
		}
		ret.analogCrop =
			IPADataSerializer<libcamera::Rectangle>::deserialize(m, m + analogCropSize, cs);
		m += analogCropSize;
		dataSize -= analogCropSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "outputSizeSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t outputSizeSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < outputSizeSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "outputSize"
				<< ": not enough data, expected "
				<< (outputSizeSize) << ", got " << (dataSize);
			return ret;
		}
		ret.outputSize =
			IPADataSerializer<libcamera::Size>::deserialize(m, m + outputSizeSize, cs);
		m += outputSizeSize;
		dataSize -= outputSizeSize;


		if (dataSize < 8) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "pixelRate"
				<< ": not enough data, expected "
				<< (8) << ", got " << (dataSize);
			return ret;
		}
		ret.pixelRate = IPADataSerializer<uint64_t>::deserialize(m, m + 8);
		m += 8;
		dataSize -= 8;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "minLineLength"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.minLineLength = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "maxLineLength"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.maxLineLength = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "minFrameLength"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.minFrameLength = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "maxFrameLength"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.maxFrameLength = IPADataSerializer<uint32_t>::deserialize(m, m + 4);

		return ret;
	}

	static libcamera::IPACameraSensorInfo
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPACameraSensorInfo>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::IPACameraSensorInfo
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPACameraSensorInfo>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<IPABuffer>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::IPABuffer &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;
		std::vector<SharedFD> retFds;

		std::vector<uint8_t> id;
		std::tie(id, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.id);
		retData.insert(retData.end(), id.begin(), id.end());

		std::vector<uint8_t> planes;
		std::vector<SharedFD> planesFds;
		std::tie(planes, planesFds) =
			IPADataSerializer<std::vector<FrameBuffer::Plane>>::serialize(data.planes, cs);
		appendPOD<uint32_t>(retData, planes.size());
		appendPOD<uint32_t>(retData, planesFds.size());
		retData.insert(retData.end(), planes.begin(), planes.end());
		retFds.insert(retFds.end(), planesFds.begin(), planesFds.end());

		return {retData, retFds};
	}

	static libcamera::IPABuffer
	deserialize(std::vector<uint8_t> &data,
		    std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPABuffer>::deserialize(data.cbegin(), data.cend(), fds.cbegin(), fds.cend(), cs);
	}


	static libcamera::IPABuffer
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    std::vector<SharedFD>::const_iterator fdsBegin,
		    std::vector<SharedFD>::const_iterator fdsEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::IPABuffer ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;
		std::vector<SharedFD>::const_iterator n = fdsBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);
		[[maybe_unused]] size_t fdsSize = std::distance(fdsBegin, fdsEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "id"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.id = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "planesSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t planesSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "planesFdsSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t planesFdsSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (fdsSize < planesFdsSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "planes"
				<< ": not enough fds, expected "
				<< (planesFdsSize) << ", got " << (fdsSize);
			return ret;
		}
		if (dataSize < planesSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "planes"
				<< ": not enough data, expected "
				<< (planesSize) << ", got " << (dataSize);
			return ret;
		}
		ret.planes =
			IPADataSerializer<std::vector<FrameBuffer::Plane>>::deserialize(m, m + planesSize, n, n + planesFdsSize, cs);

		return ret;
	}
};

template<>
class IPADataSerializer<IPASettings>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::IPASettings &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> configurationFile;
		std::tie(configurationFile, std::ignore) =
			IPADataSerializer<std::string>::serialize(data.configurationFile);
		appendPOD<uint32_t>(retData, configurationFile.size());
		retData.insert(retData.end(), configurationFile.begin(), configurationFile.end());

		std::vector<uint8_t> sensorModel;
		std::tie(sensorModel, std::ignore) =
			IPADataSerializer<std::string>::serialize(data.sensorModel);
		appendPOD<uint32_t>(retData, sensorModel.size());
		retData.insert(retData.end(), sensorModel.begin(), sensorModel.end());

		return {retData, {}};
	}

	static libcamera::IPASettings
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPASettings>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::IPASettings
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::IPASettings ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "configurationFileSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t configurationFileSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < configurationFileSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "configurationFile"
				<< ": not enough data, expected "
				<< (configurationFileSize) << ", got " << (dataSize);
			return ret;
		}
		ret.configurationFile =
			IPADataSerializer<std::string>::deserialize(m, m + configurationFileSize);
		m += configurationFileSize;
		dataSize -= configurationFileSize;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorModelSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sensorModelSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sensorModelSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sensorModel"
				<< ": not enough data, expected "
				<< (sensorModelSize) << ", got " << (dataSize);
			return ret;
		}
		ret.sensorModel =
			IPADataSerializer<std::string>::deserialize(m, m + sensorModelSize);

		return ret;
	}

	static libcamera::IPASettings
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPASettings>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::IPASettings
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPASettings>::deserialize(dataBegin, dataEnd, cs);
	}
};

template<>
class IPADataSerializer<IPAStream>
{
public:
	static std::tuple<std::vector<uint8_t>, std::vector<SharedFD>>
	serialize(const libcamera::IPAStream &data,
		  [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		std::vector<uint8_t> retData;

		std::vector<uint8_t> pixelFormat;
		std::tie(pixelFormat, std::ignore) =
			IPADataSerializer<uint32_t>::serialize(data.pixelFormat);
		retData.insert(retData.end(), pixelFormat.begin(), pixelFormat.end());

		std::vector<uint8_t> size;
		std::tie(size, std::ignore) =
			IPADataSerializer<libcamera::Size>::serialize(data.size, cs);
		appendPOD<uint32_t>(retData, size.size());
		retData.insert(retData.end(), size.begin(), size.end());

		return {retData, {}};
	}

	static libcamera::IPAStream
	deserialize(std::vector<uint8_t> &data,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPAStream>::deserialize(data.cbegin(), data.cend(), cs);
	}


	static libcamera::IPAStream
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] ControlSerializer *cs = nullptr)
	{
		libcamera::IPAStream ret;
		std::vector<uint8_t>::const_iterator m = dataBegin;

		size_t dataSize = std::distance(dataBegin, dataEnd);

		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "pixelFormat"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		ret.pixelFormat = IPADataSerializer<uint32_t>::deserialize(m, m + 4);
		m += 4;
		dataSize -= 4;


		if (dataSize < 4) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "sizeSize"
				<< ": not enough data, expected "
				<< (4) << ", got " << (dataSize);
			return ret;
		}
		const size_t sizeSize = readPOD<uint32_t>(m, 0, dataEnd);
		m += 4;
		dataSize -= 4;
		if (dataSize < sizeSize) {
			LOG(IPADataSerializer, Error)
				<< "Failed to deserialize " << "size"
				<< ": not enough data, expected "
				<< (sizeSize) << ", got " << (dataSize);
			return ret;
		}
		ret.size =
			IPADataSerializer<libcamera::Size>::deserialize(m, m + sizeSize, cs);

		return ret;
	}

	static libcamera::IPAStream
	deserialize(std::vector<uint8_t> &data,
		    [[maybe_unused]] std::vector<SharedFD> &fds,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPAStream>::deserialize(data.cbegin(), data.cend(), cs);
	}

	static libcamera::IPAStream
	deserialize(std::vector<uint8_t>::const_iterator dataBegin,
		    std::vector<uint8_t>::const_iterator dataEnd,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsBegin,
		    [[maybe_unused]] std::vector<SharedFD>::const_iterator fdsEnd,
		    ControlSerializer *cs = nullptr)
	{
		return IPADataSerializer<libcamera::IPAStream>::deserialize(dataBegin, dataEnd, cs);
	}
};


} /* namespace libcamera */