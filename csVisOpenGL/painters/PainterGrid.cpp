#include <csVisOpenGL/painters/PainterGrid.hpp>

#include <csVisOpenGL/renderers/UniformLineRenderer.hpp>

namespace csVisOpenGL {

PainterGrid::PainterGrid()
    : primaryRenderer(new UniformLineRenderer())
    , secondaryRenderer(new UniformLineRenderer()) {}

PainterGrid::~PainterGrid() {}

void PainterGrid::initialize(ShaderFactory* shaderFactory) {
  Eigen::Vector3f darkGray = Eigen::Vector3f::Ones() * 0.35f;
  Eigen::Vector3f lightGray = Eigen::Vector3f::Ones() * 0.65f;

  primaryRenderer->initialize(shaderFactory);
  primaryRenderer->setUniformColor(lightGray);
  primaryRenderer->setLineWidth(2);
  secondaryRenderer->initialize(shaderFactory);
  secondaryRenderer->setUniformColor(darkGray);

  this->setGrid(Eigen::Isometry3f::Identity(), Eigen::Vector2f(10, 10), Eigen::Vector2i(9, 9));
}

void PainterGrid::draw(const Camera& camera) {
  primaryRenderer->draw(camera);
  secondaryRenderer->draw(camera);
}

void PainterGrid::setGrid(const Eigen::Isometry3f& pose, const Eigen::Vector2f& size, const Eigen::Vector2i& steps) {

  // Get bounds
  int xSteps = steps(0);
  int ySteps = steps(1);
  Eigen::Matrix3Xf secondaryLines = Eigen::Matrix3Xf::Zero(3, xSteps * 2 + ySteps * 2);
  Eigen::Matrix3Xf primaryLines = Eigen::Matrix3Xf::Zero(3, 12);

  float dX = size(0) / (xSteps + 1);
  float maxX = size(0) / 2;
  float minX = -maxX;
  float centerX = minX + (maxX - minX) / 2;
  float dY = size(1) / (ySteps + 1);
  float maxY = size(1) / 2;
  float minY = -maxY;
  float centerY = minY + (maxY - minY) / 2;

  // Create primary lines
  primaryLines.col(0) = Eigen::Vector3f(minX, minY, 0);
  primaryLines.col(1) = Eigen::Vector3f(minX, maxY, 0);
  primaryLines.col(2) = Eigen::Vector3f(minX, maxY, 0);
  primaryLines.col(3) = Eigen::Vector3f(maxX, maxY, 0);
  primaryLines.col(4) = Eigen::Vector3f(maxX, maxY, 0);
  primaryLines.col(5) = Eigen::Vector3f(maxX, minY, 0);
  primaryLines.col(6) = Eigen::Vector3f(maxX, minY, 0);
  primaryLines.col(7) = Eigen::Vector3f(minX, minY, 0);
  primaryLines.col(8) = Eigen::Vector3f(maxX, centerY, 0);
  primaryLines.col(9) = Eigen::Vector3f(minX, centerY, 0);
  primaryLines.col(10) = Eigen::Vector3f(centerX, maxY, 0);
  primaryLines.col(11) = Eigen::Vector3f(centerX, minY, 0);

  primaryRenderer->setLines(pose * primaryLines);

  // Create secondary lines
  int idx = 0;
  for (int i = 0; i < xSteps; i++) {
    secondaryLines.col(idx++) = Eigen::Vector3f(minX + (i + 1) * dX, minY, 0);
    secondaryLines.col(idx++) = Eigen::Vector3f(minX + (i + 1) * dX, maxY, 0);
  }
  for (int i = 0; i < ySteps; i++) {
    secondaryLines.col(idx++) = Eigen::Vector3f(minX, minY + (i + 1) * dY, 0);
    secondaryLines.col(idx++) = Eigen::Vector3f(maxX, minY + (i + 1) * dY, 0);
  }
  secondaryRenderer->setLines(pose * secondaryLines);
}
} // namespace csVisOpenGL
