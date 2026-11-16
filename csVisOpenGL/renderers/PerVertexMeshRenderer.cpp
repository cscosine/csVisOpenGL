#include <cassert>
#include <csVisOpenGL/renderers/PerVertexMeshRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

PerVertexMeshRenderer::PerVertexMeshRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffusePerVertex)
    , m_cull(false) {}

PerVertexMeshRenderer::~PerVertexMeshRenderer() {}

void PerVertexMeshRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void PerVertexMeshRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (m_cull)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  getShader().bind();
  {
    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    glDrawArrays(GL_TRIANGLES, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void PerVertexMeshRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void PerVertexMeshRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void PerVertexMeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors) {
  Eigen::Matrix4Xf c = Eigen::Matrix4Xf::Ones(4, colors.cols());
  c.topLeftCorner(3, colors.cols()) = colors;
  setTriangles(positions, c);
}

void PerVertexMeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors) {
  assert(positions.cols() == colors.cols());
  getVao().uploadPositions(getShader().getVertexLocations().position, positions.data(), positions.cols());
  getVao().uploadVertexColors(getShader().getVertexLocations().color, colors.data(), colors.cols());
}
} // namespace csVisOpenGL
