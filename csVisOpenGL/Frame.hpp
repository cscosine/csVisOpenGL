#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

#include <chrono>
#include <memory>
#include <qevent.h>
#include <qtmetamacros.h>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/CameraControllerSlots.hpp>
#include <csVisOpenGL/FrameSignals.hpp>

namespace csVisOpenGL {
class Camera;
class CameraControllerInterface;
class ShaderFactory;
class VisualizerInterface;

class FrameSlotsInterface;

class Frame : public QOpenGLWidget {

  class CameraState : public CameraControllerSlots {
  public:
    CameraState()
        : m_camera() {}
    void onNewCamera(Camera const& camera);

    Camera const& getCamera() const {
      return m_camera;
    };

  private:
    Camera m_camera;
  };

public:
  Frame(QWidget* parent = 0);
  virtual ~Frame();
  // ------------- Visualizers
  void addVisualizer(std::shared_ptr<VisualizerInterface> const& visualizer);

  void connectSlotsInterface(FrameSlotsInterface const& frameSlots) const;
  void connectCameraControllerSignals(CameraControllerSignals const& cameraSignals) const;

  Camera const& getCamera() const {
    return m_cameraState.getCamera();
  };

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
  std::unique_ptr<ShaderFactory> const m_shaderFactory;
  std::vector<std::shared_ptr<VisualizerInterface>> m_visualizers;
  bool m_initialized;
  FrameSignals m_frameSignals;
  CameraState const m_cameraState;
};
} // namespace csVisOpenGL
