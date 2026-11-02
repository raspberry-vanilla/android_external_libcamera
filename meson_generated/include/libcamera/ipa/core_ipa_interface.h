/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2020, Google Inc.
 *
 * libcamera core definitions for Image Processing Algorithms
 *
 * This file is auto-generated. Do not edit.
 */

#pragma once


#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <libcamera/controls.h>
#include <libcamera/framebuffer.h>
#include <libcamera/geometry.h>

#include <libcamera/ipa/ipa_interface.h>

namespace libcamera {





struct IPACameraSensorInfo
{
public:
#ifndef __DOXYGEN__
	IPACameraSensorInfo() = default;

	template<
		typename T1 = std::string,
		typename T2 = uint32_t,
		typename T3 = uint32_t,
		typename T4 = Rectangle,
		typename T5 = Rectangle,
		typename T6 = Rectangle,
		typename T7 = Size,
		typename T8 = uint64_t,
		typename T9 = uint32_t,
		typename T10 = uint32_t,
		typename T11 = uint32_t,
		typename T12 = uint32_t,
		std::enable_if_t<std::is_convertible_v<T1&&, std::string>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T2&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T3&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T4&&, Rectangle>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T5&&, Rectangle>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T6&&, Rectangle>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T7&&, Size>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T8&&, uint64_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T9&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T10&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T11&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T12&&, uint32_t>> * = nullptr
	>
	IPACameraSensorInfo(T1 &&_model, T2 &&_bitsPerPixel, T3 &&_cfaPattern, T4 &&_pixelArrayArea, T5 &&_activeArea, T6 &&_analogCrop, T7 &&_outputSize, T8 &&_pixelRate, T9 &&_minLineLength, T10 &&_maxLineLength, T11 &&_minFrameLength, T12 &&_maxFrameLength)
		: model(std::forward<T1>(_model))
		, bitsPerPixel(std::forward<T2>(_bitsPerPixel))
		, cfaPattern(std::forward<T3>(_cfaPattern))
		, pixelArrayArea(std::forward<T4>(_pixelArrayArea))
		, activeArea(std::forward<T5>(_activeArea))
		, analogCrop(std::forward<T6>(_analogCrop))
		, outputSize(std::forward<T7>(_outputSize))
		, pixelRate(std::forward<T8>(_pixelRate))
		, minLineLength(std::forward<T9>(_minLineLength))
		, maxLineLength(std::forward<T10>(_maxLineLength))
		, minFrameLength(std::forward<T11>(_minFrameLength))
		, maxFrameLength(std::forward<T12>(_maxFrameLength))
	{
	}
#endif


	std::string model;
	uint32_t bitsPerPixel{ 0 };
	uint32_t cfaPattern{ 0 };
	Rectangle pixelArrayArea;
	Rectangle activeArea;
	Rectangle analogCrop;
	Size outputSize;
	uint64_t pixelRate{ 0 };
	uint32_t minLineLength{ 0 };
	uint32_t maxLineLength{ 0 };
	uint32_t minFrameLength{ 0 };
	uint32_t maxFrameLength{ 0 };
};

struct IPABuffer
{
public:
#ifndef __DOXYGEN__
	IPABuffer() = default;

	template<
		typename T1 = uint32_t,
		typename T2 = std::vector<FrameBuffer::Plane>,
		std::enable_if_t<std::is_convertible_v<T1&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T2&&, std::vector<FrameBuffer::Plane>>> * = nullptr
	>
	IPABuffer(T1 &&_id, T2 &&_planes)
		: id(std::forward<T1>(_id))
		, planes(std::forward<T2>(_planes))
	{
	}
#endif


	uint32_t id{ 0 };
	std::vector<FrameBuffer::Plane> planes;
};

struct IPASettings
{
public:
#ifndef __DOXYGEN__
	IPASettings() = default;

	template<
		typename T1 = std::string,
		typename T2 = std::string,
		std::enable_if_t<std::is_convertible_v<T1&&, std::string>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T2&&, std::string>> * = nullptr
	>
	IPASettings(T1 &&_configurationFile, T2 &&_sensorModel)
		: configurationFile(std::forward<T1>(_configurationFile))
		, sensorModel(std::forward<T2>(_sensorModel))
	{
	}
#endif


	std::string configurationFile;
	std::string sensorModel;
};

struct IPAStream
{
public:
#ifndef __DOXYGEN__
	IPAStream() = default;

	template<
		typename T1 = uint32_t,
		typename T2 = Size,
		std::enable_if_t<std::is_convertible_v<T1&&, uint32_t>> * = nullptr,
		std::enable_if_t<std::is_convertible_v<T2&&, Size>> * = nullptr
	>
	IPAStream(T1 &&_pixelFormat, T2 &&_size)
		: pixelFormat(std::forward<T1>(_pixelFormat))
		, size(std::forward<T2>(_size))
	{
	}
#endif


	uint32_t pixelFormat{ 0 };
	Size size;
};


} /* namespace libcamera */