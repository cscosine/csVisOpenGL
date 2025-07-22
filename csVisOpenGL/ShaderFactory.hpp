#pragma once

#include <map>
#include <memory>
#include <unordered_map>

#include <Eigen/Core>

#include <QOpenGLFunctions>
#include <QString>

class QImage;
class QOpenGLShader;

namespace csVisOpenGL {
class Shader;

class ShaderFactory {
public:
  enum ShaderNames { DiffuseUniform, DiffusePerVertex, Mesh, MeshColor, Background, Texture };

public:
  ShaderFactory();
  virtual ~ShaderFactory();

  std::unique_ptr<Shader> createShader(ShaderNames shader);

  GLuint uploadTexture(const QImage& tex);
  Eigen::Vector2i getTextureSize(GLuint id) const;
  void releaseTexture(GLuint id);

private:
  struct ShaderProgramNames {
    QString vertexProgramName;
    QString fragmentProgramName;

    ShaderProgramNames(const QString& vertexProgramName, const QString& fragmentProgramName)
        : vertexProgramName(vertexProgramName)
        , fragmentProgramName(fragmentProgramName) {}
    ShaderProgramNames() {}
  };

  std::map<int, ShaderProgramNames> m_shaderProgramNames;
  std::map<QString, QOpenGLShader*> m_vertexShaders;
  std::map<QString, QOpenGLShader*> m_fragmentShaders;
  std::unordered_map<GLuint, Eigen::Vector2i> m_textureSizes;
};
} // namespace csVisOpenGL
