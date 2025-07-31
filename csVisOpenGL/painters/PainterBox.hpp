#pragma once

#include <Eigen/Core>
#include <memory>

class QOpenGLExtraFunctions;

namespace csVisOpenGL {
class ShaderFactory;
class Camera;
class PerVertexMeshRenderer;

class PainterBox {

public:
  PainterBox();
  virtual ~PainterBox();

  void initialize(ShaderFactory& shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions);
  void draw(const Camera& camera);

  void insertBox(const int i, const Eigen::Matrix3Xf& box, const Eigen::Vector3f& color);
  void resize(const int n);
  void apply();

  const int size() const;

  static Eigen::Matrix3Xf createBoxTriangles(const double width, const double length, const double height);

private:
  std::unique_ptr<PerVertexMeshRenderer> m_renderer;
  Eigen::Matrix3Xf m_boxes;
  Eigen::Matrix3Xf m_colors;
  int m_size;
};

} // namespace csVisOpenGL
