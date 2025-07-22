#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class PerVertexPointRenderer : public Renderer {
  GLfloat pointSize;
  bool smoothPoints;

public:
  PerVertexPointRenderer();
  virtual ~PerVertexPointRenderer();

  void setPointSize(float pointSize) {
    this->pointSize = pointSize;
  }
  void setSmoothPoints(bool set) {
    this->smoothPoints = set;
  }

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setPoints(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors);
  void setPoints(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors);
  void setColors(const Eigen::Matrix3Xf& colors);
  void setColors(const Eigen::Matrix4Xf& colors);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};
} // namespace csVisOpenGL
