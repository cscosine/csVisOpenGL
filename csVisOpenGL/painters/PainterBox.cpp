#include <cassert>
#include <csVisOpenGL/painters/PainterBox.hpp>

#include <csVisOpenGL/renderers/PerVertexMeshRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/ShaderFactory.hpp>

namespace csVisOpenGL {

static constexpr int N_COLS_BOX{6 * (3 * 2)};

PainterBox::PainterBox() : m_renderer(new PerVertexMeshRenderer()) { resize(0); }

PainterBox::~PainterBox() {}

void PainterBox::initialize(ShaderFactory &shaderFactory,
                            std::shared_ptr<QOpenGLExtraFunctions> const &glExtraFunctions) {
  m_renderer->initialize(shaderFactory, glExtraFunctions);
  resize(0);
  apply();
}

void PainterBox::draw(const Camera &camera) { m_renderer->draw(camera); }

Eigen::Matrix3Xf PainterBox::createBoxTriangles(const double width, const double length, const double height) {
  // box with center in (x,y,z)=(0,0,0)

  Eigen::Matrix3Xf ret(3, N_COLS_BOX);

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

const int PainterBox::size() const { return m_size; }

void PainterBox::insertBox(const int i, const Eigen::Matrix3Xf &box, const Eigen::Vector3f &color) {
  assert(i < m_size);
  m_boxes.block(0, i * N_COLS_BOX, 3, N_COLS_BOX) = box;
  m_colors.block(0, i * N_COLS_BOX, 3, N_COLS_BOX) = color.replicate(1, N_COLS_BOX);
}

void PainterBox::resize(const int n) {
  m_boxes = Eigen::Matrix3Xf(3, n * N_COLS_BOX);
  m_colors = Eigen::Matrix3Xf(3, n * N_COLS_BOX);
  m_size = n;
}

void PainterBox::apply() { m_renderer->setTriangles(m_boxes, m_colors); }

} // namespace csVisOpenGL
