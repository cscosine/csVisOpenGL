#pragma once
#include <Eigen/Core>
#include <csVisOpenGL/ShaderFactory.hpp>
#include <memory>

class QOpenGLExtraFunctions;

namespace csVisOpenGL {
class Frame;
class Shader;
class Vao;
class Camera;

class Renderer {
public:
  Renderer(ShaderFactory::ShaderNames shaderName);
  virtual ~Renderer();

  void initialize(ShaderFactory &shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const &glExtraFunctions);

  void draw(const Camera &camera);
  void draw(const Camera &camera, const Eigen::Matrix4f &m);

protected:
  virtual void initializeConcrete() = 0;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) = 0;

  Shader &getShader();
  Vao &getVao();
  QOpenGLExtraFunctions &getQOpenGLExtraFunctions();

private:
  std::unique_ptr<Shader> m_shader;
  std::unique_ptr<Vao> m_vao;

  std::shared_ptr<QOpenGLExtraFunctions> m_glExtraFunctions;

  ShaderFactory::ShaderNames m_shaderName;
};
} // namespace csVisOpenGL
