#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

#include <chrono>
#include <memory>

namespace csVisOpenGL {
class Camera;
class CameraController;
class ShaderFactory;
class Visualizer;

class Frame : public QOpenGLWidget, public QOpenGLExtraFunctions {
  bool initialized;
  bool enabled;

  std::chrono::steady_clock::time_point lastRenderTime;

  std::unique_ptr<Camera> camera;
  std::unique_ptr<CameraController> cameraController;
  std::unique_ptr<ShaderFactory> shaderFactory;
  std::vector<Visualizer*> visualizers;

  void timerEvent(QTimerEvent* event) override;

public:
  Frame(QWidget* parent = 0);
  virtual ~Frame();
  // ------------- Visualizers
  void addVisualizer(Visualizer* visualizer);
  bool removeVisualizer(Visualizer* visualizer);

  // ------------- Getters
  const Camera& getCamera() const {
    return *camera;
  }
  CameraController* getCameraController() {
    return cameraController.get();
  }
  const CameraController* getCameraController() const {
    return cameraController.get();
  }

protected:
  // ------------- GL methods
  virtual void initializeGL() override;
  virtual void paintGL() override;
  virtual void resizeGL(int width, int height) override;

  // ------------- Events
  bool event(QEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void keyPressEvent(QKeyEvent* e) override;
  void keyReleaseEvent(QKeyEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void mouseDoubleClick(QMouseEvent* e);
  void wheelEvent(QWheelEvent* e) override;
};
} // namespace csVisOpenGL
