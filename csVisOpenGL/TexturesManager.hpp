#pragma once

#include <map>

#include <Eigen/Core>
#include <QOpenGLFunctions>
#include <QString>

class QImage;
class QOpenGLShader;

namespace csVisOpenGL {
class Shader;

class TexturesManager {
public:
  TexturesManager();
  virtual ~TexturesManager();

  GLuint uploadTexture(const QImage &tex);
  Eigen::Vector2i getTextureSize(GLuint id) const;
  void releaseTexture(GLuint id);

  std::unordered_map<GLuint, Eigen::Vector2i> m_textureSizes;
};
} // namespace csVisOpenGL
