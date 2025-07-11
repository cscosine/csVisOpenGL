#include "ShaderFactory.h"
#include <QOpenGLContext>
#include <iostream>

#include "Shader.h"

#include <QFile>
#include <QImage>
#include <QOpenGLWidget>

namespace csVisOpenGL {

ShaderFactory::ShaderFactory(Frame* frame)
    : frame(frame) {
  shaderProgramNames[ShaderNames::DiffuseUniform] = ShaderProgramNames(":/shaders/uniformColor.vp", ":/shaders/diffuse.fp");
  shaderProgramNames[ShaderNames::DiffusePerVertex] = ShaderProgramNames(":/shaders/vertexColor.vp", ":/shaders/diffuse.fp");
  shaderProgramNames[ShaderNames::Mesh] = ShaderProgramNames(":/shaders/mesh.vp", ":/shaders/mesh.fp");
  shaderProgramNames[ShaderNames::MeshColor] = ShaderProgramNames(":/shaders/meshColor.vp", ":/shaders/meshColor.fp");
  shaderProgramNames[ShaderNames::Background] = ShaderProgramNames(":/shaders/bkg.vp", ":/shaders/bkg.fp");
  shaderProgramNames[ShaderNames::Texture] = ShaderProgramNames(":/shaders/texture.vp", ":/shaders/texture.fp");
}

ShaderFactory::~ShaderFactory() {
  for (auto it : vertexShaders)
    delete it.second;
  for (auto it : fragmentShaders)
    delete it.second;

  vertexShaders.clear();
  fragmentShaders.clear();

  for (auto it : textureSizes) {
    GLuint id = it.first;
    glDeleteTextures(1, &id);
  }
}

Shader* ShaderFactory::createShader(ShaderNames shader) {
  // Get the program names
  auto shaderNamesIterator = shaderProgramNames.find(int(shader));
  if (shaderNamesIterator == shaderProgramNames.end())
    return nullptr;
  const QString& vertexName = shaderNamesIterator->second.vertexProgramName;
  const QString& fragmentName = shaderNamesIterator->second.fragmentProgramName;

  // Get vertex shader
  QOpenGLShader* vertex = nullptr;
  {
    auto it = vertexShaders.find(vertexName);
    if (it != vertexShaders.end())
      vertex = it->second;
    else {
      vertex = new QOpenGLShader(QOpenGLShader::Vertex);

      QFile f(vertexName);
      f.open(QFile::ReadOnly);
      QString source(f.readAll());
      if (QOpenGLContext::currentContext()->isOpenGLES())
        source.prepend(QByteArrayLiteral("#version 300 es\n"));
      else
        source.prepend(QByteArrayLiteral("#version 130\n"));
      // if (!vertex->compileSourceFile(vertexName))
      if (!vertex->compileSourceCode(source))

      {
        std::cout << vertexName.toStdString() << " shader compile error; " << vertex->log().toStdString() << std::endl;
        return nullptr;
      }
      vertexShaders[vertexName] = vertex;
    }
  }

  // Get fragment shader
  QOpenGLShader* fragment = nullptr;
  {
    auto it = fragmentShaders.find(fragmentName);
    if (it != fragmentShaders.end())
      fragment = it->second;
    else {
      fragment = new QOpenGLShader(QOpenGLShader::Fragment);
      QFile f(fragmentName);
      f.open(QFile::ReadOnly);
      QString source(f.readAll());
      if (QOpenGLContext::currentContext()->isOpenGLES())
        source.prepend(QByteArrayLiteral("#version 300 es\n"));
      else
        source.prepend(QByteArrayLiteral("#version 130\n"));

      // if (!fragment->compileSourceFile(fragmentName))
      if (!fragment->compileSourceCode(source)) {
        std::cout << fragmentName.toStdString() << " shader compile error: " << fragment->log().toStdString() << std::endl;
        return nullptr;
      }
      fragmentShaders[fragmentName] = fragment;
    }
  }

  // Crate shader
  Shader* res = Shader::createShader(vertex, fragment);
  return res;
}

GLuint ShaderFactory::uploadTexture(const QImage& tex) {
  assert(!tex.isNull());

  GLuint texID;
  glGenTextures(1, &texID);

  QImage glImage = tex.convertToFormat(QImage::Format_RGBA8888);

  glBindTexture(GL_TEXTURE_2D, texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
  glBindTexture(GL_TEXTURE_2D, 0);

  auto size = glImage.size();
  this->textureSizes[texID] = Eigen::Vector2i(size.width(), size.height());
  return texID;
}

Eigen::Vector2i ShaderFactory::getTextureSize(GLuint id) const {
  assert(textureSizes.count(id) > 0);
  return textureSizes.at(id);
}

void ShaderFactory::releaseTexture(GLuint id) {
  textureSizes.erase(id);
  glDeleteTextures(1, &id);
}

} // namespace csVisOpenGL
