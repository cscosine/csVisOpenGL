#pragma once

#include <Eigen/Geometry>
#include <memory>

namespace csVisOpenGL {
class UniformLineRenderer;
class ShaderFactory;
class Camera;

class PainterCameras {

  std::unique_ptr<UniformLineRenderer> renderer;

  static Eigen::Matrix3Xf _baselines;

public:
  PainterCameras();
  virtual ~PainterCameras();

  void initialize(ShaderFactory* shaderFactory);
  void draw(const Camera& camera);
  void draw(const Camera& camera, const Eigen::Matrix4f& m);

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setLineWidth(float f);

  void setPose(const Eigen::Isometry3f& p, float sizex, float sizey, float sizez);
  void setPoses(const std::vector<Eigen::Isometry3f>& p, float sizex, float sizey, float sizez);
};
} // namespace csVisOpenGL
