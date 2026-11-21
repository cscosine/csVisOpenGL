project(renderers LANGUAGES CXX)

# prepare standard library
cs_add_library(renderers STATIC)
cs_add_library(csVisOpenGL::renderers ALIAS renderers)

cs_target_sources(
  renderers
  PRIVATE_INSTALL
    BackgroundRenderer.hpp
    BackgroundSolidRenderer.hpp
    MeshColorRenderer.hpp
    MeshRenderer.hpp
    PerVertexLineRenderer.hpp
    PerVertexMeshRenderer.hpp
    PerVertexPointRenderer.hpp
    SingleTextureRenderer.hpp
    UniformEllipsoidsRenderer.hpp
    UniformLineRenderer.hpp
    UniformMeshRenderer.hpp
    UniformPointRenderer.hpp
  PRIVATE
    BackgroundRenderer.cpp
    BackgroundSolidRenderer.cpp
    MeshColorRenderer.cpp
    MeshRenderer.cpp
    PerVertexLineRenderer.cpp
    PerVertexMeshRenderer.cpp
    PerVertexPointRenderer.cpp
    SingleTextureRenderer.cpp
    UniformEllipsoidsRenderer.cpp
    UniformLineRenderer.cpp
    UniformMeshRenderer.cpp
    UniformPointRenderer.cpp
)

cs_target_link_libraries(renderers PUBLIC csVisOpenGL::csVisOpenGL)

cs_install_target(renderers COMPONENT renderers DEFAULT)
