#include "BoxRenderer.h"

#include "PerVertexMeshRenderer.h"

#include "Camera.h"
#include "ShaderFactory.h"

namespace csVisOpenGL {

const int BoxRenderer::_nColsBox = 6 * (3 * 2);

BoxRenderer::BoxRenderer()
    : _renderer(new PerVertexMeshRenderer()) {
  resize(0);
}

BoxRenderer::~BoxRenderer() {}

void BoxRenderer::initialize(ShaderFactory* shaderFactory) {
  _renderer->initialize(shaderFactory);
  resize(0);
  apply();
}

void BoxRenderer::draw(const Camera& camera) {
  _renderer->draw(camera);
}

Eigen::Matrix3Xf BoxRenderer::createBoxTriangles(const double width, const double length, const double height) {
  // box with center of mass in (x,y,z)=(0,0,0)

  Eigen::Matrix3Xf ret(3, _nColsBox);

  const double X = width;
  const double Y = length;
  const double Z = height;

  // low face from bottom-left to top-left in counter clockwise
  Eigen::Matrix3Xf lowVertices(3, 4);
  lowVertices.col(0) = Eigen::Vector3f(-X * 0.5, -Y * 0.5, -Z * 0.5);
  lowVertices.col(1) = Eigen::Vector3f(+X * 0.5, -Y * 0.5, -Z * 0.5);
  lowVertices.col(2) = Eigen::Vector3f(+X * 0.5, +Y * 0.5, -Z * 0.5);
  lowVertices.col(3) = Eigen::Vector3f(-X * 0.5, +Y * 0.5, -Z * 0.5);

  // high face from bottom-left to top-left in counter clockwise
  Eigen::Matrix3Xf highVertices(3, 4);
  highVertices.col(0) = Eigen::Vector3f(-X * 0.5, -Y * 0.5, +Z * 0.5);
  highVertices.col(1) = Eigen::Vector3f(+X * 0.5, -Y * 0.5, +Z * 0.5);
  highVertices.col(2) = Eigen::Vector3f(+X * 0.5, +Y * 0.5, +Z * 0.5);
  highVertices.col(3) = Eigen::Vector3f(-X * 0.5, +Y * 0.5, +Z * 0.5);

  // 1 frontal face
  Eigen::Matrix3Xf frontal(3, 3 * 2);
  frontal.col(0) = lowVertices.col(0);
  frontal.col(1) = lowVertices.col(1);
  frontal.col(2) = highVertices.col(0);
  frontal.col(3) = frontal.col(1);
  frontal.col(4) = highVertices.col(1);
  frontal.col(5) = frontal.col(2);

  // 2 back face
  Eigen::Matrix3Xf back(3, 3 * 2);
  back = frontal;
  back.row(1).setConstant(+Y * 0.5);

  // 3 lateral right face
  Eigen::Matrix3Xf lateralR(3, 3 * 2);
  lateralR.col(0) = lowVertices.col(1);
  lateralR.col(1) = lowVertices.col(2);
  lateralR.col(2) = highVertices.col(1);
  lateralR.col(3) = lateralR.col(1);
  lateralR.col(4) = highVertices.col(2);
  lateralR.col(5) = lateralR.col(2);

  // 4 lateral left face
  Eigen::Matrix3Xf lateralL(3, 3 * 2);
  lateralL = lateralR;
  lateralL.row(0).setConstant(-X * 0.5);

  // 5 low face
  Eigen::Matrix3Xf low(3, 3 * 2);
  low.col(0) = lowVertices.col(0);
  low.col(1) = lowVertices.col(1);
  low.col(2) = lowVertices.col(3);
  low.col(3) = low.col(1);
  low.col(4) = lowVertices.col(2);
  low.col(5) = low.col(2);

  // 6 high face
  Eigen::Matrix3Xf high(3, 3 * 2);
  high = low;
  high.row(2).setConstant(+Z * 0.5);

  // all triangles
  ret << frontal, back, lateralR, lateralL, low, high;

  return ret;
}

const int BoxRenderer::size() const {
  return _size;
}

void BoxRenderer::insertBox(const int i, const Eigen::Matrix3Xf& box, const Eigen::Vector3f& color) {
  assert(i < _size);
  _boxes.block(0, i * _nColsBox, 3, _nColsBox) = box;
  _colors.block(0, i * _nColsBox, 3, _nColsBox) = color.replicate(1, _nColsBox);
}

void BoxRenderer::resize(const int n) {
  _boxes = Eigen::Matrix3Xf(3, n * _nColsBox);
  _colors = Eigen::Matrix3Xf(3, n * _nColsBox);
  _size = n;
}

void BoxRenderer::apply() {
  _renderer->setTriangles(_boxes, _colors);
}

} // namespace csVisOpenGL
