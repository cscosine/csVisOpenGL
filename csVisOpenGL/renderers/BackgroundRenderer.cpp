#include <csVisOpenGL/renderers/BackgroundRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/ShaderFactory.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

BackgroundRenderer::BackgroundRenderer()
    : Renderer(ShaderFactory::ShaderNames::Background) {}

void BackgroundRenderer::setDefaultColors() {
  setColors(Eigen::Vector3f(31 / 255.0f, 88 / 255.0f, 125 / 255.0f), Eigen::Vector3f(42 / 255.0f, 117 / 255.0f, 169 / 255.0f),
            Eigen::Vector3f(20 / 255.0f, 40 / 255.0f, 80 / 255.0f));
}

void BackgroundRenderer::setColors(const Eigen::Vector4f& zc, const Eigen::Vector4f& hc, const Eigen::Vector4f& nc) {
  shader->bind();
  shader->getProgram().setUniformValue("zenithColor", zc.x(), zc.y(), zc.z(), zc.w());
  shader->getProgram().setUniformValue("horizonColor", hc.x(), hc.y(), hc.z(), hc.w());
  shader->getProgram().setUniformValue("nadirColor", nc.x(), nc.y(), nc.z(), nc.w());
  shader->release();
}

void BackgroundRenderer::setColors(const Eigen::Vector3f& zc, const Eigen::Vector3f& hc, const Eigen::Vector3f& nc) {
  shader->bind();
  shader->getProgram().setUniformValue("zenithColor", zc.x(), zc.y(), zc.z(), 1.0);
  shader->getProgram().setUniformValue("horizonColor", hc.x(), hc.y(), hc.z(), 1.0);
  shader->getProgram().setUniformValue("nadirColor", nc.x(), nc.y(), nc.z(), 1.0);
  shader->release();
}

void BackgroundRenderer::initializeConcrete() {

  setDefaultColors();

  Eigen::Matrix<float, 3, 4> bkgPoints;

  bkgPoints(0) = -1.0;
  bkgPoints(1) = -1.0;
  bkgPoints(2) = 0.0f;
  bkgPoints(3) = 1.0;
  bkgPoints(4) = -1.0;
  bkgPoints(5) = 0.0f;
  bkgPoints(6) = 1.0;
  bkgPoints(7) = 1.0;
  bkgPoints(8) = 0.0f;
  bkgPoints(9) = -1.0;
  bkgPoints(10) = 1.0;
  bkgPoints(11) = 0.0f;

  vao->uploadPositions(shader->getVertexLocations().position, bkgPoints.data(), bkgPoints.cols());
}

void BackgroundRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  assert(shader);

  Eigen::Matrix4f P = camera.getProjection();
  P(2, 3) = 0;
  P(2, 2) = 1;
  P(3, 2) = 0;
  P(3, 3) = 1;

  Eigen::Matrix4f V = Eigen::Matrix4f::Identity();
  V.topLeftCorner<3, 3>() = camera.getView().topLeftCorner<3, 3>();

  Eigen::Matrix<float, 4, 4, Eigen::RowMajor> PVinv = (P * V).inverse();
  QMatrix4x4 PVinv_q(PVinv.data());

  shader->bind();

  shader->getProgram().setUniformValue("inv_vp", PVinv_q);
  glExtraFunctions->glBindVertexArray(vao->getVao());
  glDrawArrays(GL_TRIANGLE_FAN, 0, vao->size());
  glExtraFunctions->glBindVertexArray(0);

  shader->release();
}

} // namespace csVisOpenGL
