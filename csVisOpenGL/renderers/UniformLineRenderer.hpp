#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class UniformLineRenderer : public Renderer {

public:
  UniformLineRenderer();
  virtual ~UniformLineRenderer();

  void setLineWidth(float lineWidth) { this->m_lineWidth = lineWidth; }

  void setUniformColor(const Eigen::Vector4f &color);
  void setUniformColor(const Eigen::Vector3f &color);
  void setLines(const Eigen::Matrix3Xf &positions);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) override;

  GLfloat m_lineWidth;
};
} // namespace csVisOpenGL
