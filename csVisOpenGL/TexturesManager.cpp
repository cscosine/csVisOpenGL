#include <QImage>
#include <cassert>
#include <csVisOpenGL/TexturesManager.hpp>

namespace csVisOpenGL {

TexturesManager::TexturesManager() {}

TexturesManager::~TexturesManager() {
  for (auto it : m_textureSizes) {
    GLuint id = it.first;
    glDeleteTextures(1, &id);
  }
}

GLuint TexturesManager::uploadTexture(const QImage &tex) {
  assert(!tex.isNull());

  GLuint texID;
  glGenTextures(1, &texID);

  QImage glImage = tex.convertToFormat(QImage::Format_RGBA8888);

  glBindTexture(GL_TEXTURE_2D, texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               glImage.bits());
  glBindTexture(GL_TEXTURE_2D, 0);

  auto size = glImage.size();
  this->m_textureSizes[texID] = Eigen::Vector2i(size.width(), size.height());
  return texID;
}

Eigen::Vector2i TexturesManager::getTextureSize(GLuint id) const {
  assert(m_textureSizes.count(id) > 0);
  return m_textureSizes.at(id);
}

void TexturesManager::releaseTexture(GLuint id) {
  m_textureSizes.erase(id);
  glDeleteTextures(1, &id);
}

} // namespace csVisOpenGL
