#pragma once

#include "Renderer.h"

namespace csVisOpenGL {
class PerVertexMeshRenderer : public Renderer {
  bool cull;

public:
  PerVertexMeshRenderer();
  virtual ~PerVertexMeshRenderer();

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);

  void setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors);
  void setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors);

  void setCullFaceEnabled(bool set) {
    this->cull = set;
  }

protected:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};
} // namespace csVisOpenGL
