#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class UniformMeshRenderer : public Renderer {

public:
  UniformMeshRenderer();
  virtual ~UniformMeshRenderer();

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setTriangles(const Eigen::Matrix3Xf& positions);

  void setCullFaceEnabled(bool set) {
    this->m_cull = set;
  }

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;

  bool m_cull;
};
} // namespace csVisOpenGL
