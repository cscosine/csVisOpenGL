project(csVisOpenGLSample LANGUAGES CXX)

# prepare application
cs_add_executable(csVisOpenGLSample)

cs_target_sources(csVisOpenGLSample
	PRIVATE
	TestVis.h
	Window.h
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
)

cs_install_target(csVisOpenGLSample csVisOpenGLSample)

cs_finalize_target(csVisOpenGLSample)
