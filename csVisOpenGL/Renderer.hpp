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
protected:
  std::unique_ptr<Shader> shader;
  std::unique_ptr<Vao> vao;

  std::shared_ptr<QOpenGLExtraFunctions> glExtraFunctions;

public:
  Renderer(ShaderFactory::ShaderNames shaderName);
  virtual ~Renderer();

  void initialize(ShaderFactory* shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions);

  void draw(const Camera& camera);
  void draw(const Camera& camera, const Eigen::Matrix4f& m);

protected:
  virtual void initializeConcrete() = 0;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) = 0;

private:
  ShaderFactory::ShaderNames shaderName;
};
} // namespace csVisOpenGL
