#include <csVisOpenGL/renderers/SingleTextureRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/ShaderFactory.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

SingleTextureRenderer::SingleTextureRenderer()
    : Renderer(ShaderFactory::ShaderNames::Texture)
    , texid(-1) {}

void SingleTextureRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

SingleTextureRenderer::~SingleTextureRenderer() {}

void SingleTextureRenderer::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void SingleTextureRenderer::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void SingleTextureRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (texid >= 0) {

    getShader().bind();

    glEnable(GL_TEXTURE_2D);
    getQOpenGLExtraFunctions().glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, this->texid);

    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    glDrawArrays(GL_TRIANGLE_FAN, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    getShader().release();
  }
}

void SingleTextureRenderer::setTexture(GLuint texid, const Matrix3x4f& vertex, const Matrix2x4f& texCoord, TextureInterpolation ti) {
  getVao().uploadPositions(getShader().getVertexLocations().position, vertex.data(), vertex.cols());
  getVao().uploadTexCoords0(getShader().getVertexLocations().texCoord0, texCoord.data(), texCoord.cols());
  this->texid = texid;

  GLuint mode = GL_LINEAR;

  if (ti == TextureInterpolation::Linear) {
    mode = GL_LINEAR;
  } else if (ti == TextureInterpolation::Nearest) {
    mode = GL_NEAREST;

  } else {
    assert(false);
  }

  getShader().bind();
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
  glBindTexture(GL_TEXTURE_2D, 0);
  getShader().release();
}

} // namespace csVisOpenGL
