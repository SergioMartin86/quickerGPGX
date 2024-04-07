# Loading upstream (base) and new cores
subdir('base')
subdir('new')

# Do not build any targets if this is a subproject
if meson.is_subproject() == false

# Common application flags
commonCompileArgs = [ '-Wall', '-Wfatal-errors' ]

# Grabbing jaffarCommon dependency

jaffarCommonSubproject = subproject('jaffarCommon')
jaffarCommonDependency = jaffarCommonSubproject.get_variable('jaffarCommonDependency')

# Building tester tool for QuickerSnes9x

newGPGXTester = executable('newGPGXTester',
  'tester.cpp',
  cpp_args            : [ commonCompileArgs ], 
  dependencies        : [ newGPGXDependency, jaffarCommonDependency ],
)

# Building tester tool for the original snes9x

baseGPGXTester = executable('baseGPGXTester',
  'tester.cpp',
  cpp_args            : [ commonCompileArgs ],
  dependencies        : [ baseGPGXDependency, jaffarCommonDependency ],
)

# Building tests
subdir('tests')

endif # If not subproject
