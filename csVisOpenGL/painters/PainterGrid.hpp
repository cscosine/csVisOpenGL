#pragma once

#include <Eigen/Geometry>
#include <memory>

class QOpenGLExtraFunctions;

namespace csVisOpenGL {
class UniformLineRenderer;
class ShaderFactory;
class Camera;

class PainterGrid {

public:
  PainterGrid();
  virtual ~PainterGrid();

  void initialize(ShaderFactory &shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const &glExtraFunctions);
  void draw(const Camera &camera);

  void setGrid(const Eigen::Isometry3f &p, const Eigen::Vector2f &size, const Eigen::Vector2i &steps);

private:
  std::unique_ptr<UniformLineRenderer> m_primaryRenderer;
  std::unique_ptr<UniformLineRenderer> m_secondaryRenderer;
};
} // namespace csVisOpenGL
