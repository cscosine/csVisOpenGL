cmake_minimum_required(VERSION 3.31)

# before project(): the oldest macOS the binaries run on (Qt 6.12)
include(csCMakeSetup)
cs_macos_deployment_target(14.4)

project(csVisOpenGLSol VERSION 1.0.0 LANGUAGES CXX)

# add the csCMake system
include(csCMake)

cs_initialize_project(
  NAMESPACE csVisOpenGL
  CXX_STANDARD 17
  # the default configurations supported by this project
  DEFAULT_CONFIG Debug Release
  DEFAULT_CONFIG_MAPPED RelWithDebInfo
  MAPPED_TO "RelWithDebInfo,Release,"
  # a custom configuration, based on Debug (imported targets: Debug, else Release)
  CUSTOM_CONFIG Paranoid
  BASED_ON "Debug,Release,"
  POSTFIX "dp"
)

# add libs, executables, etc
add_subdirectory(csVisOpenGL)
add_subdirectory(sample)
