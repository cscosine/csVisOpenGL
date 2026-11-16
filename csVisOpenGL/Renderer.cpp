#include <cassert>
#include <csVisOpenGL/Frame.hpp>
#include <csVisOpenGL/Renderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

Renderer::Renderer(ShaderFactory::ShaderNames shaderName)
    : m_shaderName(shaderName)
    , m_shader(nullptr)
    , m_vao(std::make_unique<Vao>()) {}

Renderer::~Renderer() {}

void Renderer::initialize(ShaderFactory& shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions) {
  m_glExtraFunctions = glExtraFunctions;
  m_shader = shaderFactory.createShader(m_shaderName);
  m_vao->initialize(glExtraFunctions);
  assert(m_shader);
  initializeConcrete();
}

void Renderer::draw(const Camera& camera) {
  Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
  draw(camera, m);
}

Shader& Renderer::getShader() {
  return *m_shader;
}

Vao& Renderer::getVao() {
  return *m_vao;
}

QOpenGLExtraFunctions& Renderer::getQOpenGLExtraFunctions() {
  return *m_glExtraFunctions;
}

void Renderer::draw(const Camera& camera, const Eigen::Matrix4f& m) {
  assert(m_shader);

  Eigen::Matrix4f mvp = camera.getViewProjection() * m;
  Eigen::Matrix4f mv = camera.getView() * m;
  const Shader::UniformLocations& uniforms = m_shader->getUniformLocations();

  m_shader->bind();
  if (uniforms.m >= 0)
    m_glExtraFunctions->glUniformMatrix4fv(uniforms.m, 1, false, m.data());
  if (uniforms.v >= 0)
    m_glExtraFunctions->glUniformMatrix4fv(uniforms.v, 1, false, camera.getView().data());
  if (uniforms.p >= 0)
    m_glExtraFunctions->glUniformMatrix4fv(uniforms.p, 1, false, camera.getProjection().data());
  if (uniforms.vp >= 0)
    m_glExtraFunctions->glUniformMatrix4fv(uniforms.vp, 1, false, camera.getViewProjection().data());
  if (uniforms.mv >= 0)
    m_glExtraFunctions->glUniformMatrix4fv(uniforms.mv, 1, false, mv.data());
  if (uniforms.mvp >= 0)
    m_glExtraFunctions->glUniformMatrix4fv(uniforms.mvp, 1, false, mvp.data());
  m_shader->release();

  drawConcrete(camera, m);
}
} // namespace csVisOpenGL
