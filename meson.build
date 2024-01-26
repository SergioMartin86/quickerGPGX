project('quickerGPGX','c','cpp',
  version: '1.0.0',
  license: 'GPL-3.0-only',
  default_options : ['cpp_std=c++20', 'default_library=shared', 'buildtype=release']
)

# Loading dependencies
subdir('source')

# Common application flags
commonCompileArgs = [ '-Wall', '-Wfatal-errors' ]

# Building playback tool

if get_option('buildPlayer') == true
quickerGPGXPlayerSrc = [
 'source/sdl/config.c',
 'source/sdl/error.c',
 'source/sdl/fileio.c',
 'source/sdl/unzip.c',
 'source/sdl/main.c'
]

  executable('player',
    'source/player.cpp',
    quickerGPGXPlayerSrc,
    cpp_args            : [ commonCompileArgs, '-DNCURSES' ],
    dependencies        : [ gpgxDependency,
                            toolDependency,
                            dependency('sdl2'),
                            dependency('SDL2_image'),
                          ],
    include_directories : include_directories(['source']),
    link_args           : [ '-lncurses' ],
  )
endif

# Building tester tool for QuickerGPGX

#quickerGPGXTester = executable('quickerGPGXTester',
#  'source/tester.cpp',
#  cpp_args            : [ commonCompileArgs, '-DPREVENT_RENDERING' ], 
#  dependencies        : [ quickerGPGXDependency, toolDependency ],
#  include_directories : include_directories(['../extern/json'])
#)

# Building tester tool for the original gpgx

#gpgxTester = executable('gpgxTester',
#  'source/tester.cpp',
#  cpp_args            : [ commonCompileArgs, '-DPREVENT_RENDERING'  ],
#  dependencies        : [ gpgxDependency, toolDependency ],
#  include_directories : include_directories(['../extern/json'])
#)

# Building tests
#subdir('tests')