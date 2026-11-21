#pragma once

#include "csVisOpenGL/FrameSlotsInterface.hpp"
#include <Eigen/Core>
#include <QObject>
#include <QPoint>
#include <QSet>
#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/CameraControllerSignals.hpp>

namespace csVisOpenGL {
class OrbitCameraController : public FrameSlotsInterface {

public:
  struct Settings {
    bool enableRot;
    bool enableTrasl;
    bool enableScale;

    Settings() : enableRot(true), enableTrasl(true), enableScale(true) {}
  };
  CameraControllerSignals const &getCameraControllerSignals() const { return m_cameraSignals; }

private:
  // Default speeds
  static const float defaultMouseMoveSpeed;
  static const float defaultKeyboardMoveSpeed;
  static const float defaultKeyboardSpeedBoost;
  static const float defaultRotateSpeed;
  static const float defaultZoomSpeed;
  static const float defaultMoveSmoothing;
  static const float defaultRotateSmoothing;
  static const float defaultZoomSmoothing;

  Camera m_camera;

  // Speeds
  float m_mouseMoveSpeed;
  float m_keyboardMoveSpeed;
  float m_moveSpeedScale;
  float m_keyboardSpeedBoost;
  float m_rotateSpeed;
  float m_zoomSpeed;
  float m_zoomSpeedScale;
  float m_moveSmoothing;
  float m_rotateSmoothing;
  float m_zoomSmoothing;

  // Interpolation
  Eigen::Vector3f m_pivotPos;
  Eigen::Vector3f m_pivotPosDest;
  Eigen::Vector3f m_ypr;
  Eigen::Vector3f m_yprDest;
  float m_radius;
  float m_radiusDest;
  float m_scale;
  float m_scaleDest;

  // Movement
  Eigen::Vector3f m_dPivotPos;
  Eigen::Vector3f m_dYpr;
  float m_dRadius;
  float m_dScale;

  // Mouse state
  QPointF m_lastMousePosition;
  bool m_leftClicked;
  bool m_rightClicked;
  bool m_middleClicked;

  // limits
  float m_minRadius, m_maxRadius;
  float m_minScale, m_maxScale;
  Eigen::Vector3f m_minPos;
  Eigen::Vector3f m_maxPos;

  // control enabled
  Settings m_settings;

  QSet<int> m_keysPressed;
  CameraControllerSignals m_cameraSignals;
  std::chrono::steady_clock::time_point m_lastRenderTime;

public:
  OrbitCameraController();
  virtual ~OrbitCameraController();

  // Controller interface
public slots:
  void onFocusOutEvent(QFocusEvent &eventevent) override;
  void onKeyPressEvent(QKeyEvent &eventevent) override;
  void onKeyReleaseEvent(QKeyEvent &eventevent) override;
  void onMouseMoveEvent(QMouseEvent &eventevent) override;
  void onMousePressEvent(QMouseEvent &eventevent) override;
  void onMouseReleaseEvent(QMouseEvent &eventevent) override;
  void onMouseDoubleClick(QMouseEvent &eventevent) override;
  void onWheelEvent(QWheelEvent &eventevent) override;

  void onResizeGL(int width, int height) override;
  void onPaintGL() override;
  void onInitializeGL() override;

  // camera direct control
  void setCameraOrthogonal();
  void setCameraPerspective(float fovDeg);

  // Pose setters
  void setPivotPosition(const Eigen::Vector3f &position, bool smooth = true);
  void setYpr(const Eigen::Vector3f &ypr, bool smooth = true);
  void setRotation(const Eigen::Matrix3f &R, bool smooth = true);
  void setRadius(float radius, bool smooth = true);
  void setScale(float scale, bool smooth = true);
  void setZNearFar(float zNear, float zFar);

  // Pose getters
  const Eigen::Vector3f &getPivotPosition() const { return m_pivotPos; }
  const Eigen::Vector3f &getYpr() const { return m_ypr; }
  const Eigen::Vector3f &getYprDest() const { return m_yprDest; }
  float getRadius() const { return m_radius; }

  // General getters
  bool isLeftClicked() const { return m_leftClicked; }
  bool isRightClicked() const { return m_rightClicked; }
  bool isMiddleClicked() const { return m_middleClicked; }

  float getMinRadius() { return m_minRadius; }
  float getMaxRadius() { return m_maxRadius; }
  float getMinScale() { return m_minScale; }
  float getMaxScale() { return m_maxScale; }
  const Eigen::Vector3f &getMinPos() { return m_minPos; }
  const Eigen::Vector3f &getMaxPos() { return m_maxPos; }

  // limits
  void setMinRadius(float v) { m_minRadius = v; }
  void setMaxRadius(float v) { m_maxRadius = v; }
  void setMinScale(float v) { m_minScale = v; }
  void setMaxScale(float v) { m_maxScale = v; }
  void setScaleLimits(float m, float M) {
    m_minScale = m;
    m_maxScale = M;
  }
  void setTraslationLimits(const Eigen::Vector3f &minPos, const Eigen::Vector3f &maxPos);

  // reset to camera defaults
  void resetDestinations();

  // Interaction setters
  void setMouseMoveSpeed(float moveSpeed) { this->m_mouseMoveSpeed = moveSpeed; }
  void setKeyboardMoveSpeed(float moveSpeed) { this->m_keyboardMoveSpeed = moveSpeed; }
  void setMoveSpeedScale(float scale) { this->m_moveSpeedScale = scale; }
  void setKeyboardSpeedBoost(float boost) { this->m_keyboardSpeedBoost = boost; }
  void setRotateSpeed(float rotateSpeed) { this->m_rotateSpeed = rotateSpeed; }
  void setZoomSpeed(float zoomSpeed) { this->m_zoomSpeed = zoomSpeed; }
  void setZoomSpeedScale(float scale) { this->m_zoomSpeedScale = scale; }
  void setMoveSmoothing(float moveSmoothing) { this->m_moveSmoothing = moveSmoothing; }
  void setRotateSmoothing(float rotateSmoothing) { this->m_rotateSmoothing = rotateSmoothing; }
  void setZoomSmoothing(float zoomSmoothing) { this->m_zoomSmoothing = zoomSmoothing; }
  void setSmoothing(float smoothing) {
    m_moveSmoothing = smoothing;
    m_rotateSmoothing = smoothing;
    m_zoomSmoothing = smoothing;
  }

  void setEnabledControls(const Settings &set) { this->m_settings = set; }

private:
  void normalizeYpr(Eigen::Vector3f &ypr) const;

  void setCameraPose(Eigen::Isometry3f const &pose) {
    m_camera.setPose(pose);
    emit m_cameraSignals.newCamera(m_camera);
  }

  void setCameraScale(float scale) {
    m_camera.setScale(scale);
    emit m_cameraSignals.newCamera(m_camera);
  }

  void setCameraClippingPlanes(float zNear, float zFar) {
    m_camera.setClippingPlanes(zNear, zFar);
    emit m_cameraSignals.newCamera(m_camera);
  }

  void setCameraFov(float fov) {
    m_camera.setFov(fov);
    emit m_cameraSignals.newCamera(m_camera);
  }
};
} // namespace csVisOpenGL
