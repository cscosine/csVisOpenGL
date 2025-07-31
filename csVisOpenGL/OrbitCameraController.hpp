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

    Settings()
        : enableRot(true)
        , enableTrasl(true)
        , enableScale(true) {}
  };
  CameraControllerSignals const& getCameraControllerSignals() const {
    return m_cameraSignals;
  }

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
  float mouseMoveSpeed;
  float keyboardMoveSpeed;
  float moveSpeedScale;
  float keyboardSpeedBoost;
  float rotateSpeed;
  float zoomSpeed;
  float zoomSpeedScale;
  float moveSmoothing;
  float rotateSmoothing;
  float zoomSmoothing;

  // Interpolation
  Eigen::Vector3f pivotPos;
  Eigen::Vector3f pivotPosDest;
  Eigen::Vector3f ypr;
  Eigen::Vector3f yprDest;
  float radius;
  float radiusDest;
  float scale;
  float scaleDest;

  // Movement
  Eigen::Vector3f dPivotPos;
  Eigen::Vector3f dYpr;
  float dRadius;
  float dScale;

  // Mouse state
  QPointF lastMousePosition;
  bool leftClicked;
  bool rightClicked;
  bool middleClicked;

  // limits
  float minRadius, maxRadius;
  float minScale, maxScale;
  Eigen::Vector3f minPos;
  Eigen::Vector3f maxPos;

  // control enabled
  Settings settings;

  QSet<int> m_keysPressed;
  CameraControllerSignals m_cameraSignals;
  std::chrono::steady_clock::time_point m_lastRenderTime;

public:
  OrbitCameraController();
  virtual ~OrbitCameraController();

  // Controller interface
public slots:
  void onFocusOutEvent(QFocusEvent& eventevent) override;
  void onKeyPressEvent(QKeyEvent& eventevent) override;
  void onKeyReleaseEvent(QKeyEvent& eventevent) override;
  void onMouseMoveEvent(QMouseEvent& eventevent) override;
  void onMousePressEvent(QMouseEvent& eventevent) override;
  void onMouseReleaseEvent(QMouseEvent& eventevent) override;
  void onMouseDoubleClick(QMouseEvent& eventevent) override;
  void onWheelEvent(QWheelEvent& eventevent) override;

  void onResizeGL(int width, int height) override;
  void onPaintGL() override;
  void onInitializeGL() override;

  // camera direct control
  void setCameraOrthogonal();
  void setCameraPerspective(float fovDeg);

  // Pose setters
  void setPivotPosition(const Eigen::Vector3f& position, bool smooth = true);
  void setYpr(const Eigen::Vector3f& ypr, bool smooth = true);
  void setRotation(const Eigen::Matrix3f& R, bool smooth = true);
  void setRadius(float radius, bool smooth = true);
  void setScale(float scale, bool smooth = true);
  void setZNearFar(float zNear, float zFar);

  // Pose getters
  const Eigen::Vector3f& getPivotPosition() const {
    return pivotPos;
  }
  const Eigen::Vector3f& getYpr() const {
    return ypr;
  }
  const Eigen::Vector3f& getYprDest() const {
    return yprDest;
  }
  float getRadius() const {
    return radius;
  }

  // General getters
  bool isLeftClicked() const {
    return leftClicked;
  }
  bool isRightClicked() const {
    return rightClicked;
  }
  bool isMiddleClicked() const {
    return middleClicked;
  }

  float getMinRadius() {
    return minRadius;
  }
  float getMaxRadius() {
    return maxRadius;
  }
  float getMinScale() {
    return minScale;
  }
  float getMaxScale() {
    return maxScale;
  }
  const Eigen::Vector3f& getMinPos() {
    return minPos;
  }
  const Eigen::Vector3f& getMaxPos() {
    return maxPos;
  }

  // limits
  void setMinRadius(float v) {
    minRadius = v;
  }
  void setMaxRadius(float v) {
    maxRadius = v;
  }
  void setMinScale(float v) {
    minScale = v;
  }
  void setMaxScale(float v) {
    maxScale = v;
  }
  void setScaleLimits(float m, float M) {
    minScale = m;
    maxScale = M;
  }
  void setTraslationLimits(const Eigen::Vector3f& minPos, const Eigen::Vector3f& maxPos);

  // reset to camera defaults
  void resetDestinations();

  // Interaction setters
  void setMouseMoveSpeed(float moveSpeed) {
    this->mouseMoveSpeed = moveSpeed;
  }
  void setKeyboardMoveSpeed(float moveSpeed) {
    this->keyboardMoveSpeed = moveSpeed;
  }
  void setMoveSpeedScale(float scale) {
    this->moveSpeedScale = scale;
  }
  void setKeyboardSpeedBoost(float boost) {
    this->keyboardSpeedBoost = boost;
  }
  void setRotateSpeed(float rotateSpeed) {
    this->rotateSpeed = rotateSpeed;
  }
  void setZoomSpeed(float zoomSpeed) {
    this->zoomSpeed = zoomSpeed;
  }
  void setZoomSpeedScale(float scale) {
    this->zoomSpeedScale = scale;
  }
  void setMoveSmoothing(float moveSmoothing) {
    this->moveSmoothing = moveSmoothing;
  }
  void setRotateSmoothing(float rotateSmoothing) {
    this->rotateSmoothing = rotateSmoothing;
  }
  void setZoomSmoothing(float zoomSmoothing) {
    this->zoomSmoothing = zoomSmoothing;
  }
  void setSmoothing(float smoothing) {
    moveSmoothing = smoothing;
    rotateSmoothing = smoothing;
    zoomSmoothing = smoothing;
  }

  void setEnabledControls(const Settings& set) {
    this->settings = set;
  }

private:
  void normalizeYpr(Eigen::Vector3f& ypr) const;

  void setCameraPose(Eigen::Isometry3f const& pose) {
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

signals:
  void focusLost();
  void leftPressed(QMouseEvent* pos);
  void leftReleased(QMouseEvent* pos);
  void leftDoubleClicked(QMouseEvent* pos);
  void rightPressed(QMouseEvent* pos);
  void rightReleased(QMouseEvent* pos);
  void middlePressed(QMouseEvent* pos);
  void middleReleased(QMouseEvent* pos);
  void wheelScrolled(QWheelEvent* pos);
  void mouseMove(QPointF globalPos, QPointF prevGlobalPos);
};
} // namespace csVisOpenGL
