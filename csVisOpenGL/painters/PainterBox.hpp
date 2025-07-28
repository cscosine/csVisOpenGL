#pragma once

#include <Eigen/Core>
#include <memory>

class QOpenGLExtraFunctions;

namespace csVisOpenGL {
class ShaderFactory;
class Camera;
class PerVertexMeshRenderer;

class PainterBox {

  std::unique_ptr<PerVertexMeshRenderer> _renderer;
  Eigen::Matrix3Xf _boxes;
  Eigen::Matrix3Xf _colors;
  int _size;
  const static int _nColsBox;

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
};

} // namespace csVisOpenGL
