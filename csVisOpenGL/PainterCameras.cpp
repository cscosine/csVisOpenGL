#include <csVisOpenGL/PainterCameras.hpp>
#include <csVisOpenGL/UniformLineRenderer.hpp>

namespace csVisOpenGL {

Eigen::Matrix3Xf PainterCameras::_baselines;

PainterCameras::PainterCameras()
    : renderer(new UniformLineRenderer()) {}
PainterCameras::~PainterCameras() {}

void PainterCameras::initialize(ShaderFactory* shaderFactory) {
  renderer->initialize(shaderFactory);
  renderer->setLines(Eigen::Matrix3Xf());

  if (_baselines.cols() == 0) {
    int c = 0;

    float sizex = 0.5f;
    float sizey = 0.5f;
    float sizez = 0.5f;

    _baselines.resize(3, 16);
    _baselines.col(c++) = Eigen::Vector3f(+sizex, +sizey, sizez);
    _baselines.col(c++) = Eigen::Vector3f(-sizex, +sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(-sizex, +sizey, sizez);
    _baselines.col(c++) = Eigen::Vector3f(-sizex, -sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(-sizex, -sizey, sizez);
    _baselines.col(c++) = Eigen::Vector3f(+sizex, -sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(+sizex, -sizey, sizez);
    _baselines.col(c++) = Eigen::Vector3f(+sizex, +sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    _baselines.col(c++) = Eigen::Vector3f(+sizex, +sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    _baselines.col(c++) = Eigen::Vector3f(+sizex, -sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    _baselines.col(c++) = Eigen::Vector3f(-sizex, -sizey, sizez);

    _baselines.col(c++) = Eigen::Vector3f(0, 0, 0);
    _baselines.col(c++) = Eigen::Vector3f(-sizex, +sizey, sizez);
  }
}

void PainterCameras::setUniformColor(const Eigen::Vector4f& color) {
  renderer->setUniformColor(color);
}

void PainterCameras::setUniformColor(const Eigen::Vector3f& color) {
  renderer->setUniformColor(color);
}

void PainterCameras::setLineWidth(float f) {
  renderer->setLineWidth(f);
}

void PainterCameras::draw(const Camera& camera) {
  renderer->draw(camera);
}

void PainterCameras::draw(const Camera& camera, const Eigen::Matrix4f& m) {
  renderer->draw(camera, m);
}

void PainterCameras::setPose(const Eigen::Isometry3f& p, float sizex, float sizey, float sizez) {
  Eigen::Matrix3Xf baselines = Eigen::Vector3f(sizex, sizey, sizez).asDiagonal() * _baselines;

  renderer->setLines(p * baselines);
}

void PainterCameras::setPoses(const std::vector<Eigen::Isometry3f>& ps, float sizex, float sizey, float sizez) {
  Eigen::Matrix3Xf all_lines = Eigen::Matrix3Xf::Zero(3, _baselines.cols() * ps.size());

  int col = 0;
  for (int i = 0; i < ps.size(); i++) {
    const auto& p = ps[i];

    all_lines.block(0, _baselines.cols() * i, 3, _baselines.cols()) =
        p * Eigen::Vector3f(sizex, sizey, sizez).asDiagonal() * _baselines;
  }

  renderer->setLines(all_lines);
}

} // namespace csVisOpenGL
