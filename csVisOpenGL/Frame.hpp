#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

#include <chrono>
#include <memory>
#include <qevent.h>
#include <qtmetamacros.h>

#include <csVisOpenGL/FrameSignals.hpp>

namespace csVisOpenGL {
class Camera;
class CameraControllerInterface;
class ShaderFactory;
class VisualizerInterface;

class FrameSlotsInterface;

class Frame : public QOpenGLWidget {

public:
  Frame(QWidget* parent = 0);
  virtual ~Frame();
  // ------------- Visualizers
  void addVisualizer(std::shared_ptr<VisualizerInterface> const& visualizer);
  void setCameraController(std::shared_ptr<CameraControllerInterface> const& cameraController) {
    m_cameraController = cameraController;
  }

  void connectSlotsInterface(FrameSlotsInterface const& frameSlots) const;

  FrameSignals const& getFrameSignals() const {
    return m_frameSignals;
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
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

  std::shared_ptr<QOpenGLExtraFunctions> const m_openGLExtraFunctions;
  std::shared_ptr<CameraControllerInterface> m_cameraController;
  std::unique_ptr<ShaderFactory> const m_shaderFactory;
  std::vector<std::shared_ptr<VisualizerInterface>> m_visualizers;
  bool m_initialized;
  std::chrono::steady_clock::time_point m_lastRenderTime;
  FrameSignals m_frameSignals;
};
} // namespace csVisOpenGL
