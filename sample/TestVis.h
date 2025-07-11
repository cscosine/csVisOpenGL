#pragma once
#include "csVisOpenGL/BackgroundRenderer.h"
#include "csVisOpenGL/BackgroundSolidRenderer.h"
#include "csVisOpenGL/MeshColorRenderer.h"
#include "csVisOpenGL/MeshRenderer.h"
#include "csVisOpenGL/PainterAxes.h"
#include "csVisOpenGL/PainterGrid.h"
#include "csVisOpenGL/PerVertexLineRenderer.h"
#include "csVisOpenGL/PerVertexMeshRenderer.h"
#include "csVisOpenGL/PerVertexPointRenderer.h"
#include "csVisOpenGL/SingleTextureRenderer.h"
#include "csVisOpenGL/UniformEllipsoidsRenderer.h"
#include "csVisOpenGL/UniformLineRenderer.h"
#include "csVisOpenGL/UniformMeshRenderer.h"
#include "csVisOpenGL/UniformPointRenderer.h"
#include <csVisOpenGL/Visualizer.h>

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
