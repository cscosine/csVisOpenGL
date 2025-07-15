#pragma once
#include <csVisOpenGL/BackgroundRenderer.hpp>
#include <csVisOpenGL/BackgroundSolidRenderer.hpp>
#include <csVisOpenGL/MeshColorRenderer.hpp>
#include <csVisOpenGL/MeshRenderer.hpp>
#include <csVisOpenGL/PainterAxes.hpp>
#include <csVisOpenGL/PainterGrid.hpp>
#include <csVisOpenGL/PerVertexLineRenderer.hpp>
#include <csVisOpenGL/PerVertexMeshRenderer.hpp>
#include <csVisOpenGL/PerVertexPointRenderer.hpp>
#include <csVisOpenGL/SingleTextureRenderer.hpp>
#include <csVisOpenGL/UniformEllipsoidsRenderer.hpp>
#include <csVisOpenGL/UniformLineRenderer.hpp>
#include <csVisOpenGL/UniformMeshRenderer.hpp>
#include <csVisOpenGL/UniformPointRenderer.hpp>
#include <csVisOpenGL/Visualizer.hpp>

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

  void initialize(csVisOpenGL::ShaderFactory* shaderFactory) override;
  void update(const csVisOpenGL::Camera& camera) override;
  void paintBackground(const csVisOpenGL::Camera& camera) override;
  void paint(const csVisOpenGL::Camera& camera) override;
  void paintTransparent(const csVisOpenGL::Camera& camera) override;
  void paintQt(const csVisOpenGL::Camera& camera, QPainter& painter) override;

  bool bkgSolid;
};
