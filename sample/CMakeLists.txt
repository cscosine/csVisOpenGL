project(csVisOpenGLSample LANGUAGES CXX)


# prepare application
cs_add_executable(csVisOpenGLSample)

cs_find_package(csVisOpenGLSample PRIVATE Qt6 COMPONENTS Widgets REQUIRED CONFIG)

cs_target_sources(csVisOpenGLSample
	PRIVATE
	TestVis.hpp
	Window.hpp
	Window.cpp
	TestVis.cpp
	main.cpp
	Window.ui
)
set_target_properties(csVisOpenGLSample PROPERTIES AUTOMOC ON)
set_target_properties(csVisOpenGLSample PROPERTIES AUTOUIC ON)
set_target_properties(csVisOpenGLSample PROPERTIES AUTORCC ON)

cs_target_link_libraries(csVisOpenGLSample
	PUBLIC
		csVisOpenGL::csVisOpenGL
		csVisOpenGL::renderers
		csVisOpenGL::painters
)

# a sample: built, not installed; its deployment (cs_deployment): a folder that runs on another machine, made by the
# install (Release), with the Qt plugins of a GUI application (cs_deployment_qt_gui, offscreen too); CI runs it
# (--smoke)
cs_deployment(sample TARGETS csVisOpenGLSample)
cs_deployment_qt_gui(sample OFFSCREEN)
