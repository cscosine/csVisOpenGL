#pragma once
#include <Eigen/Core>
#include <csVisOpenGL/ShaderFactory.hpp>

class QOpenGLExtraFunctions;

namespace csVisOpenGL {
class Frame;
class Shader;
class Vao;
class Camera;

class Renderer {
protected:
  ShaderFactory::ShaderNames shaderName;
  Shader* shader;
  Vao* vao;

  Frame* frame;
  QOpenGLExtraFunctions* glExtraFunctions;

public:
  Renderer(ShaderFactory::ShaderNames shaderName);
  virtual ~Renderer();

  bool isInitialized() {
    return shader != nullptr;
  }

  void initialize(ShaderFactory* shaderFactory);

  void draw(const Camera& camera);
  void draw(const Camera& camera, const Eigen::Matrix4f& m);

protected:
  virtual void initializeConcrete() = 0;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) = 0;
};
} // namespace csVisOpenGL
