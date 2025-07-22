#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class UniformMeshRenderer : public Renderer {
  bool cull;

public:
  UniformMeshRenderer();
  virtual ~UniformMeshRenderer();

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setTriangles(const Eigen::Matrix3Xf& positions);

  void setCullFaceEnabled(bool set) {
    this->cull = set;
  }

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};
} // namespace csVisOpenGL
