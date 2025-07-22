#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class UniformPointRenderer : public Renderer {
  GLfloat pointSize;
  bool smoothPoints;

public:
  UniformPointRenderer();
  virtual ~UniformPointRenderer();

  void setPointSize(float pointSize) {
    this->pointSize = pointSize;
  }
  void setSmoothPoints(bool set) {
    this->smoothPoints = set;
  }

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setPoints(const Eigen::Matrix3Xf& positions);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};
} // namespace csVisOpenGL
