/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * Properties ID list
 *
 * This file is auto-generated. Do not edit.
 */

#pragma once

#include <array>
#include <map>
#include <stdint.h>
#include <string>
#include <string_view>

#include <libcamera/controls.h>

namespace libcamera {

namespace properties {

extern const ControlIdMap properties;


enum {
	LOCATION = 1,
	ROTATION = 2,
	MODEL = 3,
	UNIT_CELL_SIZE = 4,
	PIXEL_ARRAY_AREA = 5,
	PIXEL_ARRAY_READABLE_AREA = 6,
	PIXEL_ARRAY_ACTIVE_AREAS = 7,
	SCALER_CROP_MAXIMUM = 8,
	SENSOR_SENSITIVITY = 9,
	SYSTEM_DEVICES = 10,
	PIPELINE_HANDLER = 11,
};


enum LocationEnum {
	LocationFront = 0,
	LocationBack = 1,
	LocationExternal = 2,
};
extern const std::array<const ControlValue, 3> LocationValues;
extern const std::map<std::string, int32_t> LocationNameValueMap;
extern const Control<int32_t> Location;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_LOCATION
extern const Control<int32_t> Rotation;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_ROTATION
extern const Control<std::string_view> Model;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_MODEL
extern const Control<Size> UnitCellSize;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_UNIT_CELL_SIZE
extern const Control<Rectangle> PixelArrayArea;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_PIXEL_ARRAY_AREA
extern const Control<Rectangle> PixelArrayReadableArea;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_PIXEL_ARRAY_READABLE_AREA
extern const Control<std::span<const Rectangle>> PixelArrayActiveAreas;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_PIXEL_ARRAY_ACTIVE_AREAS
extern const Control<Rectangle> ScalerCropMaximum;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_SCALER_CROP_MAXIMUM
extern const Control<float> SensorSensitivity;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_SENSOR_SENSITIVITY
extern const Control<std::span<const int64_t>> SystemDevices;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_SYSTEM_DEVICES
extern const Control<std::string_view> PipelineHandler;
#define LIBCAMERA_HAS_LIBCAMERA_VENDOR_PROPERTIES_PIPELINE_HANDLER

namespace draft {

#define LIBCAMERA_HAS_DRAFT_VENDOR_PROPERTIES


enum {
	COLOR_FILTER_ARRANGEMENT = 10001,
};


enum ColorFilterArrangementEnum {
	ColorFilterArrangementRGGB = 0,
	ColorFilterArrangementGRBG = 1,
	ColorFilterArrangementGBRG = 2,
	ColorFilterArrangementBGGR = 3,
	ColorFilterArrangementRGB = 4,
	ColorFilterArrangementMONO = 5,
};
extern const std::array<const ControlValue, 6> ColorFilterArrangementValues;
extern const std::map<std::string, int32_t> ColorFilterArrangementNameValueMap;
extern const Control<int32_t> ColorFilterArrangement;
#define LIBCAMERA_HAS_DRAFT_VENDOR_PROPERTIES_COLOR_FILTER_ARRANGEMENT

} /* namespace draft */

} /* namespace properties */

} /* namespace libcamera */