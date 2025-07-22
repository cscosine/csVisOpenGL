#pragma once

#include <QOpenGLContext>
#include <QPainter>

namespace csVisOpenGL {
class Camera;
class ShaderFactory;

class VisualizerInterface {

public:
  VisualizerInterface() {}
  virtual ~VisualizerInterface() {}

  virtual void initialize(ShaderFactory* shaderFactory, std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions) {};
  virtual void update(const Camera& camera) {};
  virtual void paintBackground(const Camera& camera) {};
  virtual void paint(const Camera& camera) {};
  virtual void paintTransparent(const Camera& camera) {};
  virtual void paintQt(const Camera& camera, QPainter& painter) {};
  virtual void afterPaint(const Camera& camera) {};
};
} // namespace csVisOpenGL
