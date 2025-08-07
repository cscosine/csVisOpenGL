project(csVisOpenGL LANGUAGES CXX)


# prepare custom project asserts, note: use standard find_package
find_package(csCore REQUIRED COMPONENTS assert CONFIG)

list(APPEND CMAKE_MODULE_PATH "${csCore_BASE_DIR}/cmake/csCore/assert/")
include(csAssert)

# set defaults for Paranoid additional configuration
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELEASE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELWITHDEBINFO Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT SAFE_MATH Paranoid ON)

csAssertGenerateProject(csVisOpenGL csVisOpenGL) #generate target csVisOpenGL::csVisOpenGLAssert
list(POP_BACK CMAKE_MODULE_PATH)

# prepare standard library
cs_add_library(csVisOpenGL STATIC)
cs_add_library(csVisOpenGL::csVisOpenGL ALIAS csVisOpenGL)

cs_find_package(csVisOpenGL PUBLIC Eigen3 REQUIRED CONFIG)
cs_find_package(csVisOpenGL PUBLIC csCore COMPONENTS core eigenUtils REQUIRED CONFIG)
cs_find_package(csVisOpenGL PUBLIC Qt6 COMPONENTS Core Gui OpenGL Widgets OpenGLWidgets REQUIRED CONFIG)
cs_find_package(csVisOpenGL PUBLIC OpenGL REQUIRED)

cs_target_sources(csVisOpenGL
	PRIVATE_INSTALL
CameraControllerSignals.hpp
CameraControllerSlots.hpp
Camera.hpp
Colors.hpp
ColorUtils.hpp
Frame.hpp
FrameSlotsInterface.hpp
FrameSignals.hpp
OrbitCameraController.hpp
Renderer.hpp
ShaderFactory.hpp
Shader.hpp
Vao.hpp
VisualizerInterface.hpp
TexturesManager.hpp
PRIVATE
Camera.cpp
Colors.cpp
ColorUtils.cpp
Frame.cpp
FrameSlotsInterface.cpp
FrameSignals.cpp
OrbitCameraController.cpp
Renderer.cpp
Shader.cpp
ShaderFactory.cpp
TexturesManager.cpp
Vao.cpp
CameraControllerSignals.cpp
CameraControllerSlots.cpp
# resources
vis.qrc
		)

set_target_properties(csVisOpenGL PROPERTIES AUTOMOC ON)
set_target_properties(csVisOpenGL PROPERTIES AUTOUIC ON)
set_target_properties(csVisOpenGL PROPERTIES AUTORCC ON)


cs_target_link_libraries(csVisOpenGL
	PUBLIC
		Eigen3::Eigen
		Qt6::Core
		Qt6::Gui
		Qt6::Widgets
		Qt6::OpenGL
		Qt6::OpenGLWidgets
		OpenGL::GL
		csVisOpenGL::csVisOpenGLAssert
		csVisOpenGLSol::csVisOpenGLSol_csCMakeBaseInclude

)

cs_install_target(csVisOpenGL csVisOpenGL DEFAULT)

add_subdirectory(renderers)
add_subdirectory(painters)

cs_finalize_target(csVisOpenGL)
