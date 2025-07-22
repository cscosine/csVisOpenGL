#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

#include <chrono>
#include <memory>

namespace csVisOpenGL {
class Camera;
class CameraController;
class ShaderFactory;
class VisualizerInterface;

class Frame : public QOpenGLWidget {

public:
  Frame(QWidget* parent = 0);
  virtual ~Frame();
  // ------------- Visualizers
  void addVisualizer(std::shared_ptr<VisualizerInterface> const& visualizer);

  // ------------- Getters
  const Camera& getCamera() const {
    return *m_camera;
  }
  CameraController* getCameraController() {
    return m_cameraController.get();
  }
  const CameraController* getCameraController() const {
    return m_cameraController.get();
  }

private:
  void timerEvent(QTimerEvent* event) override;

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

  bool m_initialized;
  std::shared_ptr<QOpenGLExtraFunctions> m_openGLExtraFunctions;
  std::chrono::steady_clock::time_point m_lastRenderTime;
  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<CameraController> m_cameraController;
  std::unique_ptr<ShaderFactory> m_shaderFactory;
  std::vector<std::shared_ptr<VisualizerInterface>> m_visualizers;
};
} // namespace csVisOpenGL
