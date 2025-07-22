#include <csVisOpenGL/renderers/PerVertexLineRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

PerVertexLineRenderer::PerVertexLineRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffusePerVertex)
    , lineWidth(1) {}

PerVertexLineRenderer::~PerVertexLineRenderer() {}

void PerVertexLineRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void PerVertexLineRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glLineWidth(lineWidth);

  getShader().bind();
  {
    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    getQOpenGLExtraFunctions().glDrawArrays(GL_LINES, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void PerVertexLineRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void PerVertexLineRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void PerVertexLineRenderer::setLines(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors) {
  assert(positions.cols() == colors.cols());
  getVao().uploadPositions(getShader().getVertexLocations().position, positions.data(), positions.cols());
  getVao().uploadVertexColors(getShader().getVertexLocations().color, colors.data(), colors.cols());
}

void PerVertexLineRenderer::setLines(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors) {
  Eigen::Matrix4Xf c = Eigen::Matrix4Xf::Ones(4, colors.cols());
  c.topLeftCorner(3, colors.cols()) = colors;
  setLines(positions, c);
}
} // namespace csVisOpenGL
