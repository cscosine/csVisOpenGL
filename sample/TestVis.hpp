#pragma once
#include <csVisOpenGL/Visualizer.hpp>
#include <csVisOpenGL/painters/PainterAxes.hpp>
#include <csVisOpenGL/painters/PainterGrid.hpp>
#include <csVisOpenGL/renderers/BackgroundRenderer.hpp>
#include <csVisOpenGL/renderers/BackgroundSolidRenderer.hpp>
#include <csVisOpenGL/renderers/MeshColorRenderer.hpp>
#include <csVisOpenGL/renderers/MeshRenderer.hpp>
#include <csVisOpenGL/renderers/PerVertexLineRenderer.hpp>
#include <csVisOpenGL/renderers/PerVertexMeshRenderer.hpp>
#include <csVisOpenGL/renderers/PerVertexPointRenderer.hpp>
#include <csVisOpenGL/renderers/SingleTextureRenderer.hpp>
#include <csVisOpenGL/renderers/UniformEllipsoidsRenderer.hpp>
#include <csVisOpenGL/renderers/UniformLineRenderer.hpp>
#include <csVisOpenGL/renderers/UniformMeshRenderer.hpp>
#include <csVisOpenGL/renderers/UniformPointRenderer.hpp>

class TestcsVisOpenGL : public csVisOpenGL::Visualizer {
  csVisOpenGL::UniformPointRenderer uniformPointRenderer;
  csVisOpenGL::PerVertexPointRenderer perVertexPointRenderer;
  csVisOpenGL::UniformLineRenderer uniformLineRenderer;
  csVisOpenGL::PerVertexLineRenderer perVertexLineRenderer;

  csVisOpenGL::UniformMeshRenderer uniformMeshRenderer;
  csVisOpenGL::PerVertexMeshRenderer perVertexMeshRendered;

  csVisOpenGL::MeshRenderer meshRenderer;
  csVisOpenGL::MeshColorRenderer meshColorRenderer;

  csVisOpenGL::PainterAxes axes;
  csVisOpenGL::PainterGrid grid;

  csVisOpenGL::BackgroundRenderer bkgRenderer;
  csVisOpenGL::BackgroundSolidRenderer bkgSolidRenderer;

  csVisOpenGL::UniformEllipsoidsRenderer3D unifEllipsRenderer3d;
  csVisOpenGL::UniformEllipsoidsRenderer2D unifEllipsRenderer2d;

  csVisOpenGL::SingleTextureRenderer singleTextureRenderer;

public:
  TestcsVisOpenGL();
  virtual ~TestcsVisOpenGL();

  void initialize(csVisOpenGL::ShaderFactory* shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions) override;
  void update(const csVisOpenGL::Camera& camera) override;
  void paintBackground(const csVisOpenGL::Camera& camera) override;
  void paint(const csVisOpenGL::Camera& camera) override;
  void paintTransparent(const csVisOpenGL::Camera& camera) override;
  void paintQt(const csVisOpenGL::Camera& camera, QPainter& painter) override;

  bool bkgSolid;
};
