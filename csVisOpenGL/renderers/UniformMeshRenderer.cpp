#include <csVisOpenGL/renderers/UniformMeshRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformMeshRenderer::UniformMeshRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , m_cull(false) {}

UniformMeshRenderer::~UniformMeshRenderer() {}

void UniformMeshRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformMeshRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (m_cull)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  getShader().bind();
  {
    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    getQOpenGLExtraFunctions().glDrawArrays(GL_TRIANGLES, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void UniformMeshRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void UniformMeshRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void UniformMeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions) {
  getVao().uploadPositions(getShader().getVertexLocations().position, positions.data(), positions.cols());
}
} // namespace csVisOpenGL
