#pragma once

#include <QOpenGLContext>
#include <QPainter>

namespace csVisOpenGL {
class Camera;
class ShaderFactory;

class Visualizer {
  bool enabled;

public:
  Visualizer()
      : enabled(true) {}
  virtual ~Visualizer() {}

  bool isEnabled() const {
    return enabled;
  }
  void setEnabled(bool set) {
    enabled = set;
  }

  virtual void initialize(ShaderFactory* shaderFactory) {};
  virtual void update(const Camera& camera) {};
  virtual void paintBackground(const Camera& camera) {};
  virtual void paint(const Camera& camera) {};
  virtual void paintTransparent(const Camera& camera) {};
  virtual void paintQt(const Camera& camera, QPainter& painter) {};
  virtual void afterPaint(const Camera& camera) {};
};
} // namespace csVisOpenGL
