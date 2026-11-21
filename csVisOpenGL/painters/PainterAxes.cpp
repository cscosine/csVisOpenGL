#include <csVisOpenGL/painters/PainterAxes.hpp>

#include <csVisOpenGL/renderers/PerVertexLineRenderer.hpp>

namespace csVisOpenGL {

PainterAxes::PainterAxes() : m_renderer(std::make_unique<PerVertexLineRenderer>()) {}
PainterAxes::~PainterAxes() {}

void PainterAxes::initialize(ShaderFactory &shaderFactory,
                             std::shared_ptr<QOpenGLExtraFunctions> const &glExtraFunctions) {
  m_renderer->initialize(shaderFactory, glExtraFunctions);
  m_renderer->setLines(Eigen::Matrix3Xf(3, 0), Eigen::Matrix3Xf(3, 0));
}

void PainterAxes::draw(const Camera &camera) { m_renderer->draw(camera); }

void PainterAxes::setPose(const Eigen::Isometry3f &p, float size) {
  Eigen::Matrix3Xf lines = Eigen::Matrix3Xf::Zero(3, 6);
  Eigen::Matrix3Xf colors = Eigen::Matrix3Xf::Zero(3, 6);

  lines.col(0) = Eigen::Vector3f::Zero();
  lines.col(1) = p.matrix().col(0).head<3>() * size;
  lines.col(2) = Eigen::Vector3f::Zero();
  lines.col(3) = p.matrix().col(1).head<3>() * size;
  lines.col(4) = Eigen::Vector3f::Zero();
  lines.col(5) = p.matrix().col(2).head<3>() * size;

  lines.colwise() += p.translation();

  colors.col(0) = Eigen::Vector3f::UnitX();
  colors.col(1) = Eigen::Vector3f::UnitX();
  colors.col(2) = Eigen::Vector3f::UnitY();
  colors.col(3) = Eigen::Vector3f::UnitY();
  colors.col(4) = Eigen::Vector3f::UnitZ();
  colors.col(5) = Eigen::Vector3f::UnitZ();

  m_renderer->setLines(lines, colors);
}

void PainterAxes::setPoses(const std::vector<Eigen::Isometry3f> &ps, const Eigen::VectorXf &size) {
  Eigen::Matrix3Xf all_lines = Eigen::Matrix3Xf::Zero(3, 6 * ps.size());
  Eigen::Matrix3Xf all_colors = Eigen::Matrix3Xf::Zero(3, 6 * ps.size());

  int col = 0;
  for (int i = 0; i < ps.size(); i++) {
    const auto &p = ps[i];
    // x
    all_colors.col(col) = Eigen::Vector3f::UnitX();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitX();
    all_lines.col(col++) = p.translation() + p.matrix().col(0).head<3>() * size(i);
    // y
    all_colors.col(col) = Eigen::Vector3f::UnitY();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitY();
    all_lines.col(col++) = p.translation() + p.matrix().col(1).head<3>() * size(i);
    // z
    all_colors.col(col) = Eigen::Vector3f::UnitZ();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitZ();
    all_lines.col(col++) = p.translation() + p.matrix().col(2).head<3>() * size(i);
  }

  m_renderer->setLines(all_lines, all_colors);
}

void PainterAxes::setPoses(const std::vector<Eigen::Isometry3f> &ps, float size) {
  Eigen::Matrix3Xf all_lines = Eigen::Matrix3Xf::Zero(3, 6 * ps.size());
  Eigen::Matrix3Xf all_colors = Eigen::Matrix3Xf::Zero(3, 6 * ps.size());

  int col = 0;
  for (int i = 0; i < ps.size(); i++) {
    const auto &p = ps[i];
    // x
    all_colors.col(col) = Eigen::Vector3f::UnitX();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitX();
    all_lines.col(col++) = p.translation() + p.matrix().col(0).head<3>() * size;
    // y
    all_colors.col(col) = Eigen::Vector3f::UnitY();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitY();
    all_lines.col(col++) = p.translation() + p.matrix().col(1).head<3>() * size;
    // z
    all_colors.col(col) = Eigen::Vector3f::UnitZ();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitZ();
    all_lines.col(col++) = p.translation() + p.matrix().col(2).head<3>() * size;
  }

  m_renderer->setLines(all_lines, all_colors);
}

void PainterAxes::setPoses(const std::vector<Eigen::Isometry3f> &ps, std::vector<float> size) {
  Eigen::Matrix3Xf all_lines = Eigen::Matrix3Xf::Zero(3, 6 * ps.size());
  Eigen::Matrix3Xf all_colors = Eigen::Matrix3Xf::Zero(3, 6 * ps.size());

  int col = 0;
  for (int i = 0; i < ps.size(); i++) {
    const auto &p = ps[i];
    // x
    all_colors.col(col) = Eigen::Vector3f::UnitX();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitX();
    all_lines.col(col++) = p.translation() + p.matrix().col(0).head<3>() * size[i];
    // y
    all_colors.col(col) = Eigen::Vector3f::UnitY();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitY();
    all_lines.col(col++) = p.translation() + p.matrix().col(1).head<3>() * size[i];
    // z
    all_colors.col(col) = Eigen::Vector3f::UnitZ();
    all_lines.col(col++) = p.translation() + Eigen::Vector3f::Zero();
    all_colors.col(col) = Eigen::Vector3f::UnitZ();
    all_lines.col(col++) = p.translation() + p.matrix().col(2).head<3>() * size[i];
  }

  m_renderer->setLines(all_lines, all_colors);
}

void PainterAxes::setLineWidth(float f) { m_renderer->setLineWidth(f); }

} // namespace csVisOpenGL
