#include <csVisOpenGL/renderers/PerVertexPointRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

PerVertexPointRenderer::PerVertexPointRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffusePerVertex)
    , m_pointSize(1)
    , m_smoothPoints(false) {}

PerVertexPointRenderer::~PerVertexPointRenderer() {}

void PerVertexPointRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void PerVertexPointRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glPointSize(m_pointSize);

  getShader().bind();
  {
    if (m_smoothPoints)
      glEnable(GL_POINT_SMOOTH);
    else
      glDisable(GL_POINT_SMOOTH);

    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    getQOpenGLExtraFunctions().glDrawArrays(GL_POINTS, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void PerVertexPointRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void PerVertexPointRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void PerVertexPointRenderer::setColors(const Eigen::Matrix4Xf& colors) {
  getVao().uploadVertexColors(getShader().getVertexLocations().color, colors.data(), colors.cols());
}

void PerVertexPointRenderer::setColors(const Eigen::Matrix3Xf& colors) {
  Eigen::Matrix4Xf c = Eigen::Matrix4Xf::Ones(4, colors.cols());
  c.topLeftCorner(3, colors.cols()) = colors;
  setColors(c);
}

void PerVertexPointRenderer::setPoints(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors) {
  assert(positions.cols() == colors.cols());
  getVao().uploadPositions(getShader().getVertexLocations().position, positions.data(), positions.cols());
  setColors(colors);
}

void PerVertexPointRenderer::setPoints(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors) {
  Eigen::Matrix4Xf c = Eigen::Matrix4Xf::Ones(4, colors.cols());
  c.topLeftCorner(3, colors.cols()) = colors;
  setPoints(positions, c);
}
} // namespace csVisOpenGL
