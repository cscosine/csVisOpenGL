#include <csVisOpenGL/renderers/UniformPointRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformPointRenderer::UniformPointRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , pointSize(1)
    , smoothPoints(false) {}

UniformPointRenderer::~UniformPointRenderer() {}

void UniformPointRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformPointRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glPointSize(pointSize);

  getShader().bind();
  {
    if (smoothPoints)
      getQOpenGLExtraFunctions().glEnable(GL_POINT_SMOOTH);
    else
      getQOpenGLExtraFunctions().glDisable(GL_POINT_SMOOTH);

    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    getQOpenGLExtraFunctions().glDrawArrays(GL_POINTS, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void UniformPointRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void UniformPointRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void UniformPointRenderer::setPoints(const Eigen::Matrix3Xf& positions) {
  getVao().uploadPositions(getShader().getVertexLocations().position, positions.data(), positions.cols());
}

} // namespace csVisOpenGL
