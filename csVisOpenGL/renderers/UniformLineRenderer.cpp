#include <csVisOpenGL/renderers/UniformLineRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformLineRenderer::UniformLineRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , lineWidth(1) {}

UniformLineRenderer::~UniformLineRenderer() {}

void UniformLineRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformLineRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glLineWidth(lineWidth);

  getShader().bind();
  {
    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    glDrawArrays(GL_LINES, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void UniformLineRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void UniformLineRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void UniformLineRenderer::setLines(const Eigen::Matrix3Xf& positions) {
  getVao().uploadPositions(getShader().getVertexLocations().position, positions.data(), positions.cols());
}

} // namespace csVisOpenGL
