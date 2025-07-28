#pragma once

#include <Eigen/Geometry>
#include <memory>

class QOpenGLExtraFunctions;

namespace csVisOpenGL {
class UniformMeshRenderer;
class PainterAxes;
class ShaderFactory;
class Camera;

class PainterCheckerBoard {

  std::unique_ptr<UniformMeshRenderer> blackSquares;
  std::unique_ptr<UniformMeshRenderer> whiteSquares;
  std::unique_ptr<PainterAxes> axes;

  bool paintChecherboard;
  bool paintAxes;

public:
  PainterCheckerBoard();
  virtual ~PainterCheckerBoard();

  void initialize(ShaderFactory& shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions);
  void draw(const Camera& camera);

  void setCheckerBoard(int w, int h, float squareSize, const Eigen::Isometry3f& pose, float xBase, float yBase, float axesScale);
};
} // namespace csVisOpenGL
