#include <csVisOpenGL/OrbitCameraController.hpp>

#include <math.h>

#include <csVisOpenGL/Camera.hpp>

#include <QFocusEvent>
#include <QMouseEvent>
#include <QWheelEvent>

namespace csVisOpenGL {

const float OrbitCameraController::defaultMouseMoveSpeed = 0.0015f;
const float OrbitCameraController::defaultKeyboardMoveSpeed = 0.15f;
const float OrbitCameraController::defaultKeyboardSpeedBoost = 4.0f;
const float OrbitCameraController::defaultRotateSpeed = 0.005f;
const float OrbitCameraController::defaultZoomSpeed = 0.005f;
const float OrbitCameraController::defaultMoveSmoothing = 0.2f;
const float OrbitCameraController::defaultRotateSmoothing = 0.2f;
const float OrbitCameraController::defaultZoomSmoothing = 0.2f;

OrbitCameraController::OrbitCameraController()
    : FrameSlotsInterface()
    , m_camera()
    , m_pivotPos(m_camera.getPose().translation())
    , m_pivotPosDest(m_pivotPos)
    , m_radius(0)
    , m_radiusDest(m_radius)
    , m_scale(m_camera.getScale())
    , m_scaleDest(m_camera.getScale())
    , m_ypr(m_camera.getPose().linear().eulerAngles(2, 1, 0))
    , m_yprDest(m_camera.getPose().linear().eulerAngles(2, 1, 0))
    , m_mouseMoveSpeed(defaultMouseMoveSpeed)
    , m_keyboardMoveSpeed(defaultKeyboardMoveSpeed)
    , m_moveSpeedScale(1)
    , m_keyboardSpeedBoost(defaultKeyboardSpeedBoost)
    , m_rotateSpeed(defaultRotateSpeed)
    , m_zoomSpeed(defaultZoomSpeed)
    , m_zoomSpeedScale(1)
    , m_moveSmoothing(defaultMoveSmoothing)
    , m_rotateSmoothing(defaultRotateSmoothing)
    , m_zoomSmoothing(defaultZoomSmoothing)
    , m_dPivotPos(Eigen::Vector3f::Zero())
    , m_dRadius(0)
    , m_dScale(0)
    , m_minRadius(0.01f)
    , m_maxRadius(1000.0f)
    , m_minScale(0.01f)
    , m_maxScale(1000.0f)
    , m_dYpr(Eigen::Vector3f::Zero())
    , m_lastMousePosition(-1, -1)
    , m_leftClicked(false)
    , m_rightClicked(false)
    , m_middleClicked(false)
    , m_minPos(
          Eigen::Vector3f(Eigen::NumTraits<float>::lowest(), Eigen::NumTraits<float>::lowest(), Eigen::NumTraits<float>::lowest()))
    , m_maxPos(
          Eigen::Vector3f(Eigen::NumTraits<float>::highest(), Eigen::NumTraits<float>::highest(), Eigen::NumTraits<float>::highest()))
    , m_settings(Settings())
    , m_keysPressed{}
    , m_lastRenderTime{std::chrono::steady_clock::now()}

{}

OrbitCameraController::~OrbitCameraController() {}

// ----------------------------------------- Events
void OrbitCameraController::onKeyPressEvent(QKeyEvent& event) {
  m_keysPressed += event.key();
}

void OrbitCameraController::onKeyReleaseEvent(QKeyEvent& event) {
  m_keysPressed -= event.key();
}

void OrbitCameraController::onFocusOutEvent(QFocusEvent& event) {
  if (m_leftClicked)
    m_leftClicked = false;
  if (m_rightClicked)
    m_rightClicked = false;
  if (m_middleClicked)
    m_middleClicked = false;

  event.accept();
}

void OrbitCameraController::onMouseMoveEvent(QMouseEvent& event) {
  // Initialization
  if (m_lastMousePosition == QPointF(-1, -1)) {
    m_lastMousePosition = event.globalPosition();
    return;
  }

  // Perform the proper action
  QPointF dMouse = event.globalPosition() - m_lastMousePosition;
  if (m_rightClicked && m_settings.enableTrasl) {
    Eigen::Vector3f movement = (m_camera.left() * dMouse.x()) + (m_camera.up() * dMouse.y());
    m_dPivotPos += movement * m_mouseMoveSpeed * m_moveSpeedScale * pow(m_radius, 0.75f);
  }

  if (m_leftClicked && m_settings.enableRot) {
    m_dYpr.x() -= dMouse.x() * m_rotateSpeed;
    m_dYpr.y() += dMouse.y() * m_rotateSpeed;
  }

  // Done
  QPointF _lastMousePosition = m_lastMousePosition;
  m_lastMousePosition = event.globalPosition();
  event.accept();
}

void OrbitCameraController::onMousePressEvent(QMouseEvent& event) {
  if (event.button() == Qt::LeftButton) {
    m_leftClicked = true;
    m_lastMousePosition = QPointF(-1, -1);
  } else if (event.button() == Qt::RightButton) {
    m_rightClicked = true;
    m_lastMousePosition = QPointF(-1, -1);
  } else if (event.button() == Qt::MouseButton::MiddleButton) {
    m_middleClicked = true;
    m_lastMousePosition = QPointF(-1, -1);
  }

  event.accept();
}

void OrbitCameraController::onMouseReleaseEvent(QMouseEvent& event) {
  if (event.button() == Qt::LeftButton) {
    m_leftClicked = false;
    m_lastMousePosition = QPointF(-1, -1);
  } else if (event.button() == Qt::RightButton) {
    m_rightClicked = false;
    m_lastMousePosition = QPointF(-1, -1);
  } else if (event.button() == Qt::MouseButton::MiddleButton) {
    m_middleClicked = false;
    m_lastMousePosition = QPointF(-1, -1);
  }

  event.accept();
}

void OrbitCameraController::onMouseDoubleClick(QMouseEvent& event) {
  event.accept();
}

void OrbitCameraController::onWheelEvent(QWheelEvent& event) {
  if (m_settings.enableScale) {
    if (m_camera.isPerspective()) {
      m_dRadius = -event.angleDelta().y() * m_zoomSpeed * m_zoomSpeedScale * pow(m_radius, 0.5f);
    } else {
      m_dScale = -event.angleDelta().y() * m_zoomSpeed * m_zoomSpeedScale * pow(m_scale, 0.5f);
    }
    event.accept();
  }
}

void OrbitCameraController::onResizeGL(int width, int height) {
  m_camera.setViewSize(Eigen::Vector2f(width, height));
  emit m_cameraSignals.newCamera(m_camera);
}

void OrbitCameraController::onInitializeGL() {
  m_lastRenderTime = std::chrono::steady_clock::now();
  emit m_cameraSignals.newCamera(m_camera);
}

// ----------------------------------------- Update

void OrbitCameraController::resetDestinations() {
  m_pivotPosDest = m_pivotPos;
  m_radiusDest = m_radius;
  m_scaleDest = m_scale;
}

void OrbitCameraController::onPaintGL() {
  // Get time since last paint
  auto t_now = std::chrono::steady_clock::now();
  float const dT = std::chrono::duration<float>(t_now - m_lastRenderTime).count();
  m_lastRenderTime = t_now;

  // Keyboard input
  float speedBoost = 1;
  QPointF keyDelta;
  if (m_keysPressed.contains(Qt::Key_W) || m_keysPressed.contains(Qt::Key_Up))
    keyDelta.setY(keyDelta.y() + 1);
  if (m_keysPressed.contains(Qt::Key_S) || m_keysPressed.contains(Qt::Key_Down))
    keyDelta.setY(keyDelta.y() - 1);
  if (m_keysPressed.contains(Qt::Key_A) || m_keysPressed.contains(Qt::Key_Left))
    keyDelta.setX(keyDelta.x() + 1);
  if (m_keysPressed.contains(Qt::Key_D) || m_keysPressed.contains(Qt::Key_Right))
    keyDelta.setX(keyDelta.x() - 1);
  if (m_keysPressed.contains(Qt::Key_Shift))
    speedBoost = m_keyboardSpeedBoost;
  m_dPivotPos += (m_camera.left() * keyDelta.x()) * m_keyboardMoveSpeed * m_moveSpeedScale * speedBoost * dT * pow(m_radius, 0.75f);
  if (m_camera.isPerspective()) {
    m_dPivotPos +=
        (m_camera.forward() * keyDelta.y()) * m_keyboardMoveSpeed * m_moveSpeedScale * speedBoost * dT * pow(m_radius, 0.75f);
  } else {
    m_dScale -= keyDelta.y() * m_keyboardMoveSpeed * m_moveSpeedScale * speedBoost * dT * pow(m_scale, 0.75f);
  }

  // Lerp position
  m_pivotPosDest += m_dPivotPos;
  m_pivotPos += (m_pivotPosDest - m_pivotPos) * m_moveSmoothing;

  // Lerp radius
  m_radiusDest += m_dRadius;
  if (m_radiusDest < m_minRadius)
    m_radiusDest = m_minRadius;
  if (m_radiusDest > m_maxRadius)
    m_radiusDest = m_maxRadius;
  m_radius += (m_radiusDest - m_radius) * m_zoomSmoothing;

  // Lerp scale
  m_scaleDest += m_dScale;
  if (m_scaleDest < m_minScale)
    m_scaleDest = m_minScale;
  if (m_scaleDest > m_maxScale)
    m_scaleDest = m_maxScale;
  m_scale += (m_scaleDest - m_scale) * m_zoomSmoothing;

  // Lerp rotation
  m_yprDest += m_dYpr;

  for (int i = 0; i < 3; i++) {
    while (m_ypr(i) - m_yprDest(i) < -M_PI)
      m_yprDest(i) -= float(2 * M_PI);
    while (m_ypr(i) - m_yprDest(i) >= M_PI)
      m_yprDest(i) += float(2 * M_PI);
  }
  Eigen::Vector3f diff = m_yprDest - m_ypr;
  for (int i = 0; i < 3; i++) {
    if (diff(i) < -M_PI)
      diff(i) = (2 * M_PI) + diff(i);
    if (diff(i) >= M_PI)
      diff(i) = (2 * M_PI) - diff(i);
  }
  m_ypr += diff * m_rotateSmoothing;
  normalizeYpr(m_ypr);
  Eigen::Matrix3f R;
  R = (Eigen::AngleAxisf(m_ypr(0), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(m_ypr(1), Eigen::Vector3f::UnitY()) *
       Eigen::AngleAxisf(m_ypr(2), Eigen::Vector3f::UnitX()));

  // Update pose
  Eigen::Isometry3f pose = m_camera.getPose();
  pose.linear() = R;
  pose.translation() = m_pivotPos - (R.col(0) * m_radius * (m_camera.isPerspective() ? 1 : 0));
  for (int i = 0; i < 3; i++) {
    if (pose.translation()(i) < m_minPos(i)) {
      pose.translation()(i) = m_minPos(i);
      m_pivotPos(i) = pose.translation()(i) + (R.col(0)(i) * m_radius * (m_camera.isPerspective() ? 1 : 0));
      m_pivotPosDest(i) = m_pivotPos(i);
    }
    if (pose.translation()(i) > m_maxPos(i)) {
      pose.translation()(i) = m_maxPos(i);
      m_pivotPos(i) = pose.translation()(i) + (R.col(0)(i) * m_radius * (m_camera.isPerspective() ? 1 : 0));
      m_pivotPosDest(i) = m_pivotPos(i);
    }
  }

  setCameraPose(pose);

  // update scale
  setCameraScale(m_scale);

  // Reset velocities
  m_dPivotPos.setZero();
  m_dYpr.setZero();
  m_dRadius = 0;
  m_dScale = 0;

  emit m_cameraSignals.newCamera(m_camera);
}

void OrbitCameraController::normalizeYpr(Eigen::Vector3f& ypr) const {
  // [0..2pi[
  for (int i = 0; i < 3; i++) {
    while (ypr(i) < 0)
      ypr(i) += float(2 * M_PI);
    while (ypr(i) >= 2 * M_PI)
      ypr(i) -= float(2 * M_PI);
  }
}

// ----------------------------------------- Pose setters

void OrbitCameraController::setPivotPosition(const Eigen::Vector3f& position, bool smooth) {
  m_pivotPosDest = position;
  if (!smooth) {
    m_pivotPos = m_pivotPosDest;
    Eigen::Isometry3f pose = m_camera.getPose();
    pose.translation() = m_pivotPos - (m_camera.forward() * m_radius * (m_camera.isPerspective() ? 1 : 0));

    setCameraPose(pose);
  }
}

void OrbitCameraController::setYpr(const Eigen::Vector3f& ypr, bool smooth) {
  m_yprDest = ypr;
  if (!smooth) {
    this->m_ypr = m_yprDest;
    Eigen::Vector3f pos = m_camera.getPose().translation();
    Eigen::Isometry3f pose;
    pose.translation() = pos;

    pose.linear() = (Eigen::AngleAxisf(ypr(0), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(ypr(1), Eigen::Vector3f::UnitY()) *
                     Eigen::AngleAxisf(ypr(2), Eigen::Vector3f::UnitX()))
                        .toRotationMatrix();
    setCameraPose(pose);
  }
}

void OrbitCameraController::setRotation(const Eigen::Matrix3f& R, bool smooth) {
  Eigen::Isometry3f pose;
  pose.linear() = R;
  pose.translation() = m_camera.getPose().translation();

  m_yprDest = pose.linear().eulerAngles(2, 1, 0);
  if (!smooth) {
    this->m_ypr = m_yprDest;
    setCameraPose(pose);
  }
}

void OrbitCameraController::setRadius(float r, bool smooth) {
  m_radiusDest = r;
  if (!smooth) {
    m_radius = m_radiusDest;
    auto pose = m_camera.getPose();
    pose.translation() = m_pivotPos - (m_camera.forward() * m_radius * (m_camera.isPerspective() ? 1 : 0));

    setCameraPose(pose);
  }
}

void OrbitCameraController::setZNearFar(float zNear, float zFar) {
  setCameraClippingPlanes(zNear, zFar);
}

void OrbitCameraController::setScale(float s, bool smooth) {
  m_scaleDest = s;
  if (!smooth) {
    m_scale = m_scaleDest;
    setCameraScale(m_scaleDest);
  }
}

void OrbitCameraController::setTraslationLimits(const Eigen::Vector3f& minPos, const Eigen::Vector3f& maxPos) {
  this->m_minPos = minPos;
  this->m_maxPos = maxPos;
}

void OrbitCameraController::setCameraOrthogonal() {
  m_camera.setOrthogonal();
  emit m_cameraSignals.newCamera(m_camera);
}

void OrbitCameraController::setCameraPerspective(float fovDeg) {
  setCameraFov(fovDeg);
  this->setScale(1.0, false);
  emit m_cameraSignals.newCamera(m_camera);
}

} // namespace csVisOpenGL
