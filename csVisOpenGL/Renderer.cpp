#include <csVisOpenGL/Frame.hpp>
#include <csVisOpenGL/Renderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

Renderer::Renderer(ShaderFactory::ShaderNames shaderName)
    : shaderName(shaderName)
    , shader(nullptr)
    , vao(new Vao())
    , frame(nullptr) {}

Renderer::~Renderer() {
  if (frame) {
    frame->makeCurrent();
    delete shader;
    delete vao;
  }
}

void Renderer::initialize(ShaderFactory* shaderFactory) {
  this->frame = shaderFactory->getFrame();
  glExtraFunctions = frame->getQOpenGLExtraFunctions();
  this->shader = shaderFactory->createShader(shaderName);
  vao->initialize(glExtraFunctions);
  assert(this->shader);
  initializeConcrete();
}

void Renderer::draw(const Camera& camera) {
  Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
  draw(camera, m);
}

void Renderer::draw(const Camera& camera, const Eigen::Matrix4f& m) {
  if (!isInitialized())
    return;

  Eigen::Matrix4f mvp = camera.getViewProjection() * m;
  Eigen::Matrix4f mv = camera.getView() * m;
  const Shader::UniformLocations& uniforms = shader->getUniformLocations();

  shader->bind();
  if (uniforms.m >= 0)
    glExtraFunctions->glUniformMatrix4fv(uniforms.m, 1, false, m.data());
  if (uniforms.v >= 0)
    glExtraFunctions->glUniformMatrix4fv(uniforms.v, 1, false, camera.getView().data());
  if (uniforms.p >= 0)
    glExtraFunctions->glUniformMatrix4fv(uniforms.p, 1, false, camera.getProjection().data());
  if (uniforms.vp >= 0)
    glExtraFunctions->glUniformMatrix4fv(uniforms.vp, 1, false, camera.getViewProjection().data());
  if (uniforms.mv >= 0)
    glExtraFunctions->glUniformMatrix4fv(uniforms.mv, 1, false, mv.data());
  if (uniforms.mvp >= 0)
    glExtraFunctions->glUniformMatrix4fv(uniforms.mvp, 1, false, mvp.data());
  shader->release();

  drawConcrete(camera, m);
}
} // namespace csVisOpenGL
