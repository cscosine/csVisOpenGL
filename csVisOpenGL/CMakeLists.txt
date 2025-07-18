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
renderers/BackgroundRenderer.hpp
renderers/BackgroundSolidRenderer.hpp
renderers/BoxRenderer.hpp
CameraController.hpp
Camera.hpp
Colors.hpp
ColorUtils.hpp
Frame.hpp
renderers/MeshColorRenderer.hpp
renderers/MeshRenderer.hpp
OrbitCameraController.hpp
painters/PainterAxes.hpp
painters/PainterCameras.hpp
painters/PainterCheckerBoard.hpp
painters/PainterGrid.hpp
renderers/PerVertexLineRenderer.hpp
renderers/PerVertexMeshRenderer.hpp
renderers/PerVertexPointRenderer.hpp
Renderer.hpp
ShaderFactory.hpp
Shader.hpp
renderers/SingleTextureRenderer.hpp
renderers/UniformEllipsoidsRenderer.hpp
renderers/UniformLineRenderer.hpp
renderers/UniformMeshRenderer.hpp
renderers/UniformPointRenderer.hpp
Vao.hpp
Visualizer.hpp
PRIVATE
renderers/BackgroundRenderer.cpp
renderers/BackgroundSolidRenderer.cpp
renderers/BoxRenderer.cpp
CameraController.cpp
Camera.cpp
Colors.cpp
ColorUtils.cpp
Frame.cpp
renderers/MeshColorRenderer.cpp
renderers/MeshRenderer.cpp
OrbitCameraController.cpp
painters/PainterAxes.cpp
painters/PainterCameras.cpp
painters/PainterCheckerBoard.cpp
painters/PainterGrid.cpp
renderers/PerVertexLineRenderer.cpp
renderers/PerVertexMeshRenderer.cpp
renderers/PerVertexPointRenderer.cpp
Renderer.cpp
Shader.cpp
ShaderFactory.cpp
renderers/SingleTextureRenderer.cpp
renderers/UniformEllipsoidsRenderer.cpp
renderers/UniformLineRenderer.cpp
renderers/UniformMeshRenderer.cpp
renderers/UniformPointRenderer.cpp
Vao.cpp
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
		csVisOpenGLLib::csVisOpenGLLib_csCMakeBaseInclude

)

cs_install_target(csVisOpenGL csVisOpenGL DEFAULT)

cs_finalize_target(csVisOpenGL)
