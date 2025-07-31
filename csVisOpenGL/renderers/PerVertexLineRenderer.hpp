#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class PerVertexLineRenderer : public Renderer {

public:
  PerVertexLineRenderer();
  virtual ~PerVertexLineRenderer();

  void setLineWidth(float lineWidth) {
    this->m_lineWidth = lineWidth;
  }

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setLines(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors);
  void setLines(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;

  GLfloat m_lineWidth;
};
} // namespace csVisOpenGL
