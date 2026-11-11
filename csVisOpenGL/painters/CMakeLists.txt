project(painters LANGUAGES CXX)

# prepare standard library
cs_add_library(painters STATIC)
cs_add_library(csVisOpenGL::painters ALIAS painters)


cs_target_sources(painters
	PRIVATE_INSTALL
PainterBox.hpp
PainterAxes.hpp
PainterCameras.hpp
PainterCheckerBoard.hpp
PainterGrid.hpp
PRIVATE
PainterBox.cpp
PainterAxes.cpp
PainterCameras.cpp
PainterCheckerBoard.cpp
PainterGrid.cpp
		)


cs_target_link_libraries(painters
	PUBLIC
		csVisOpenGL::csVisOpenGL
		csVisOpenGL::renderers
)

cs_install_target(painters COMPONENT painters DEFAULT)
