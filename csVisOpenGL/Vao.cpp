#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

namespace detail {

void uploadVBO(QOpenGLExtraFunctions& glExtraFunctions, GLuint vao, GLuint vbo, const float* values, int count, GLenum usage) {
  assert(vbo > 0);
  glExtraFunctions.glBindVertexArray(vao);
  {
    glExtraFunctions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glExtraFunctions.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, values, usage);
    glExtraFunctions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glExtraFunctions.glBindVertexArray(0);
}

// vao is a reference, gets updated
GLuint createVBO(QOpenGLExtraFunctions& glExtraFunctions, GLuint& vao, GLuint location, GLint size, GLenum type,
                 GLboolean normalized) {
  // Create (if it does not exist) and bind the VAO
  if (vao == 0) {
    glExtraFunctions.glGenVertexArrays(1, &vao);
  }
  glExtraFunctions.glBindVertexArray(vao);

  // Create the VBO
  GLuint vbo;
  glExtraFunctions.glGenBuffers(1, &vbo);

  // Setup the VBO
  glExtraFunctions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glExtraFunctions.glVertexAttribPointer(location, size, type, normalized, 0, 0);
  glExtraFunctions.glEnableVertexAttribArray(location);
  glExtraFunctions.glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind VAO
  glExtraFunctions.glBindVertexArray(0);

  return vbo;
}
} // namespace detail

Vao::Vao()
    : m_vao(0)
    , m_locationToVbo{}
    , m_count(-1)
    , m_glExtraFunctions{nullptr} {}

Vao::~Vao() {
  if (m_vao != 0) {
    for (auto it : m_locationToVbo) {
      m_glExtraFunctions->glDeleteBuffers(1, &it.second);
    }
    m_glExtraFunctions->glDeleteVertexArrays(1, &m_vao);
  }
  m_vao = 0;
}

void Vao::uploadPositions(GLuint location, const float* values, int count) {
  if (m_locationToVbo.find(location) == m_locationToVbo.end()) {
    m_locationToVbo[location] = detail::createVBO(*m_glExtraFunctions, m_vao, location, 3, GL_FLOAT, false);
  }
  detail::uploadVBO(*m_glExtraFunctions, m_vao, m_locationToVbo[location], values, count * 3, GL_STATIC_DRAW);
  m_count = count;
}

void Vao::uploadVertexColors(GLuint location, const float* values, int count) {
  if (m_locationToVbo.find(location) == m_locationToVbo.end()) {
    m_locationToVbo[location] = detail::createVBO(*m_glExtraFunctions, m_vao, location, 4, GL_FLOAT, false);
  }
  detail::uploadVBO(*m_glExtraFunctions, m_vao, m_locationToVbo[location], values, count * 4, GL_STATIC_DRAW);
  m_count = count;
}

void Vao::uploadNormals(GLuint location, const float* values, int count) {
  if (m_locationToVbo.find(location) == m_locationToVbo.end()) {
    m_locationToVbo[location] = detail::createVBO(*m_glExtraFunctions, m_vao, location, 3, GL_FLOAT, true);
  }
  detail::uploadVBO(*m_glExtraFunctions, m_vao, m_locationToVbo[location], values, count * 3, GL_STATIC_DRAW);
  m_count = count;
}

void Vao::uploadTexCoords0(GLuint location, const float* values, int count) {
  if (m_locationToVbo.find(location) == m_locationToVbo.end()) {
    m_locationToVbo[location] = detail::createVBO(*m_glExtraFunctions, m_vao, location, 2, GL_FLOAT, false);
  }
  detail::uploadVBO(*m_glExtraFunctions, m_vao, m_locationToVbo[location], values, count * 2, GL_STATIC_DRAW);
  m_count = count;
}

} // namespace csVisOpenGL
