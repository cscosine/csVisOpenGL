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
		BackgroundRenderer.h
		BackgroundSolidRenderer.h
		BoxRenderer.h
		CameraController.h
		Camera.h
		Colors.h
		ColorUtils.h
		Frame.h
		MeshColorRenderer.h
		MeshRenderer.h
		OrbitCameraController.h
		PainterAxes.h
		PainterCameras.h
		PainterCheckerBoard.h
		PainterGrid.h
		PerVertexLineRenderer.h
		PerVertexMeshRenderer.h
		PerVertexPointRenderer.h
		Renderer.h
		ShaderFactory.h
		Shader.h
		SingleTextureRenderer.h
		UniformEllipsoidsRenderer.h
		UniformLineRenderer.h
		UniformMeshRenderer.h
		UniformPointRenderer.h
		Utils.h
		Vao.h
		Visualizer.h
	PRIVATE
		BackgroundRenderer.cpp
		BackgroundSolidRenderer.cpp
		BoxRenderer.cpp
		CameraController.cpp
		Camera.cpp
		Colors.cpp
		ColorUtils.cpp
		Frame.cpp
		MeshColorRenderer.cpp
		MeshRenderer.cpp
		OrbitCameraController.cpp
		PainterAxes.cpp
		PainterCameras.cpp
		PainterCheckerBoard.cpp
		PainterGrid.cpp
		PerVertexLineRenderer.cpp
		PerVertexMeshRenderer.cpp
		PerVertexPointRenderer.cpp
		Renderer.cpp
		Shader.cpp
		ShaderFactory.cpp
		SingleTextureRenderer.cpp
		UniformEllipsoidsRenderer.cpp
		UniformLineRenderer.cpp
		UniformMeshRenderer.cpp
		UniformPointRenderer.cpp
		Utils.cpp
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

cs_install_target(csVisOpenGL csVisOpenGLExport DEFAULT)

cs_finalize_target(csVisOpenGL)
