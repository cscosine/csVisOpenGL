#include <csVisOpenGL/painters/PainterCameras.hpp>
#include <csVisOpenGL/renderers/UniformLineRenderer.hpp>

namespace csVisOpenGL {

PainterCameras::PainterCameras()
    : m_renderer(new UniformLineRenderer()) {}
PainterCameras::~PainterCameras() {}

void PainterCameras::initialize(ShaderFactory& shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions) {
  m_renderer->initialize(shaderFactory, glExtraFunctions);
  m_renderer->setLines(Eigen::Matrix3Xf());

  if (m_baselines.cols() == 0) {
    int c = 0;

    float sizex = 0.5f;
    float sizey = 0.5f;
    float sizez = 0.5f;

    m_baselines.resize(3, 16);
    m_baselines.col(c++) = Eigen::Vector3f(+sizex, +sizey, sizez);
    m_baselines.col(c++) = Eigen::Vector3f(-sizex, +sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(-sizex, +sizey, sizez);
    m_baselines.col(c++) = Eigen::Vector3f(-sizex, -sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(-sizex, -sizey, sizez);
    m_baselines.col(c++) = Eigen::Vector3f(+sizex, -sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(+sizex, -sizey, sizez);
    m_baselines.col(c++) = Eigen::Vector3f(+sizex, +sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    m_baselines.col(c++) = Eigen::Vector3f(+sizex, +sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    m_baselines.col(c++) = Eigen::Vector3f(+sizex, -sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    m_baselines.col(c++) = Eigen::Vector3f(-sizex, -sizey, sizez);

    m_baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    m_baselines.col(c++) = Eigen::Vector3f(-sizex, +sizey, sizez);
  }
}

void PainterCameras::setUniformColor(const Eigen::Vector4f& color) {
  m_renderer->setUniformColor(color);
}

void PainterCameras::setUniformColor(const Eigen::Vector3f& color) {
  m_renderer->setUniformColor(color);
}

void PainterCameras::setLineWidth(float f) {
  m_renderer->setLineWidth(f);
}

void PainterCameras::draw(const Camera& camera) {
  m_renderer->draw(camera);
}

void PainterCameras::draw(const Camera& camera, const Eigen::Matrix4f& m) {
  m_renderer->draw(camera, m);
}

void PainterCameras::setPose(const Eigen::Isometry3f& p, float sizex, float sizey, float sizez) {
  Eigen::Matrix3Xf baselines = Eigen::Vector3f(sizex, sizey, sizez).asDiagonal() * m_baselines;

  m_renderer->setLines(p * baselines);
}

void PainterCameras::setPoses(const std::vector<Eigen::Isometry3f>& ps, float sizex, float sizey, float sizez) {
  Eigen::Matrix3Xf all_lines = Eigen::Matrix3Xf::Zero(3, m_baselines.cols() * ps.size());

  int col = 0;
  for (int i = 0; i < ps.size(); i++) {
    const auto& p = ps[i];

    all_lines.block(0, m_baselines.cols() * i, 3, m_baselines.cols()) =
        p * Eigen::Vector3f(sizex, sizey, sizez).asDiagonal() * m_baselines;
  }

  m_renderer->setLines(all_lines);
}

} // namespace csVisOpenGL
