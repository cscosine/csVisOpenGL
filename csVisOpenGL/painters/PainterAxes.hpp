#pragma once

#include <Eigen/Geometry>
#include <memory>

namespace csVisOpenGL {
class PerVertexLineRenderer;
class ShaderFactory;
class Camera;

class PainterAxes {
  std::unique_ptr<PerVertexLineRenderer> renderer;

public:
  PainterAxes();
  virtual ~PainterAxes();

  void initialize(ShaderFactory* shaderFactory);
  void draw(const Camera& camera);

  void setPose(const Eigen::Isometry3f& p, float size);
  void setPoses(const std::vector<Eigen::Isometry3f>& p, const Eigen::VectorXf& size);
  void setPoses(const std::vector<Eigen::Isometry3f>& p, float size);
  void setPoses(const std::vector<Eigen::Isometry3f>& p, std::vector<float> size);

  void setLineWidth(float w);
};
} // namespace csVisOpenGL
