# SPDX-License-Identifier: CC0-1.0

libcamera_internal_sources += files([
    'config_parser.cpp',
    'image_frame_generator.cpp',
    'test_pattern_generator.cpp',
    'virtual.cpp',
])

libjpeg = dependency('libjpeg', required : false)

libcamera_deps += [libyuv_dep]
if libjpeg.found()
  libcamera_deps += [libjpeg]
endif

subdir('data')
