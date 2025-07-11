#include "SingleTextureRenderer.h"

#include "Camera.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "Vao.h"

namespace csVisOpenGL {

SingleTextureRenderer::SingleTextureRenderer()
    : Renderer(ShaderFactory::ShaderNames::Texture)
    , texid(-1) {}

void SingleTextureRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

SingleTextureRenderer::~SingleTextureRenderer() {}

void SingleTextureRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void SingleTextureRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void SingleTextureRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (texid >= 0) {
    assert(shader);

    shader->bind();

    glEnable(GL_TEXTURE_2D);
    glExtraFunctions->glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, this->texid);

    glExtraFunctions->glBindVertexArray(vao->getVao());
    glDrawArrays(GL_TRIANGLE_FAN, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    shader->release();
  }
}

void SingleTextureRenderer::setTexture(GLuint texid, const Matrix3x4f& vertex, const Matrix2x4f& texCoord, TextureInterpolation ti) {
  vao->uploadPositions(shader->getVertexLocations().position, vertex.data(), vertex.cols());
  vao->uploadTexCoords0(shader->getVertexLocations().texCoord0, texCoord.data(), texCoord.cols());
  this->texid = texid;

  GLuint mode = GL_LINEAR;

  if (ti == TextureInterpolation::Linear) {
    mode = GL_LINEAR;
  } else if (ti == TextureInterpolation::Nearest) {
    mode = GL_NEAREST;

  } else {
    assert(false);
  }

  shader->bind();
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
  glBindTexture(GL_TEXTURE_2D, 0);
  shader->release();
}

} // namespace csVisOpenGL
