#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <csVisOpenGL/ShaderFactory.hpp>
#include <iostream>

#include <csVisOpenGL/Shader.hpp>

#include <QFile>
#include <QImage>
#include <QOpenGLWidget>

namespace csVisOpenGL {

namespace {

// The #version line of the shaders for the current context: GLSL ES 3.00 for OpenGL ES; for a core profile (macOS: its
// only context newer than OpenGL 2.1) GLSL 3.30 core, 1.50 core below OpenGL 3.3; else GLSL 1.30 (OpenGL 3.0 and
// newer, compatibility profile: Linux, Windows)
QString glslVersionLine() {
  QOpenGLContext const* context = QOpenGLContext::currentContext();
  if (context->isOpenGLES())
    return QStringLiteral("#version 300 es\n");
  QSurfaceFormat const format = context->format();
  if (format.profile() == QSurfaceFormat::CoreProfile)
    return format.version() < qMakePair(3, 3) ? QStringLiteral("#version 150 core\n") : QStringLiteral("#version 330 core\n");
  return QStringLiteral("#version 130\n");
}

} // namespace

ShaderFactory::ShaderFactory() {
  m_shaderProgramNames[ShaderNames::DiffuseUniform] = ShaderProgramNames(":/shaders/uniformColor.vp", ":/shaders/diffuse.fp");
  m_shaderProgramNames[ShaderNames::DiffusePerVertex] = ShaderProgramNames(":/shaders/vertexColor.vp", ":/shaders/diffuse.fp");
  m_shaderProgramNames[ShaderNames::Mesh] = ShaderProgramNames(":/shaders/mesh.vp", ":/shaders/mesh.fp");
  m_shaderProgramNames[ShaderNames::MeshColor] = ShaderProgramNames(":/shaders/meshColor.vp", ":/shaders/meshColor.fp");
  m_shaderProgramNames[ShaderNames::Background] = ShaderProgramNames(":/shaders/bkg.vp", ":/shaders/bkg.fp");
  m_shaderProgramNames[ShaderNames::Texture] = ShaderProgramNames(":/shaders/texture.vp", ":/shaders/texture.fp");
}

ShaderFactory::~ShaderFactory() {
  for (auto it : m_vertexShaders)
    delete it.second;
  for (auto it : m_fragmentShaders)
    delete it.second;

  m_vertexShaders.clear();
  m_fragmentShaders.clear();
}

std::unique_ptr<Shader> ShaderFactory::createShader(ShaderNames shader) {
  // Get the program names
  auto shaderNamesIterator = m_shaderProgramNames.find(int(shader));
  if (shaderNamesIterator == m_shaderProgramNames.end())
    return nullptr;
  const QString& vertexName = shaderNamesIterator->second.vertexProgramName;
  const QString& fragmentName = shaderNamesIterator->second.fragmentProgramName;

  // Get vertex shader
  QOpenGLShader* vertex = nullptr;
  {
    auto it = m_vertexShaders.find(vertexName);
    if (it != m_vertexShaders.end())
      vertex = it->second;
    else {
      vertex = new QOpenGLShader(QOpenGLShader::Vertex);

      QFile f(vertexName);
      f.open(QFile::ReadOnly);
      QString source(f.readAll());
      source.prepend(glslVersionLine());
      // if (!vertex->compileSourceFile(vertexName))
      if (!vertex->compileSourceCode(source))

      {
        std::cerr << vertexName.toStdString() << " shader compile error; " << vertex->log().toStdString() << std::endl;
        return nullptr;
      }
      m_vertexShaders[vertexName] = vertex;
    }
  }

  // Get fragment shader
  QOpenGLShader* fragment = nullptr;
  {
    auto it = m_fragmentShaders.find(fragmentName);
    if (it != m_fragmentShaders.end())
      fragment = it->second;
    else {
      fragment = new QOpenGLShader(QOpenGLShader::Fragment);
      QFile f(fragmentName);
      f.open(QFile::ReadOnly);
      QString source(f.readAll());
      source.prepend(glslVersionLine());

      // if (!fragment->compileSourceFile(fragmentName))
      if (!fragment->compileSourceCode(source)) {
        std::cerr << fragmentName.toStdString() << " shader compile error: " << fragment->log().toStdString() << std::endl;
        return nullptr;
      }
      m_fragmentShaders[fragmentName] = fragment;
    }
  }

  // Crate shader
  return std::make_unique<Shader>(vertex, fragment);
}

} // namespace csVisOpenGL
