# SPDX-License-Identifier: CC0-1.0

libcamera_sources += files([
    'pisp.cpp',
])

libpisp_dep = dependency('libpisp', fallback : ['libpisp', 'libpisp_dep'])

libcamera_deps += [libpisp_dep]

subdir('data')
