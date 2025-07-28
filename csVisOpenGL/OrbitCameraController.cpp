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

OrbitCameraController::OrbitCameraController(Camera& camera)
    : CameraController(camera)
    , pivotPos(getCamera().getPose().translation())
    , pivotPosDest(pivotPos)
    , radius(0)
    , radiusDest(radius)
    , scale(getCamera().getScale())
    , scaleDest(getCamera().getScale())
    , ypr(getCamera().getPose().linear().eulerAngles(2, 1, 0))
    , yprDest(getCamera().getPose().linear().eulerAngles(2, 1, 0))
    , mouseMoveSpeed(defaultMouseMoveSpeed)
    , keyboardMoveSpeed(defaultKeyboardMoveSpeed)
    , moveSpeedScale(1)
    , keyboardSpeedBoost(defaultKeyboardSpeedBoost)
    , rotateSpeed(defaultRotateSpeed)
    , zoomSpeed(defaultZoomSpeed)
    , zoomSpeedScale(1)
    , moveSmoothing(defaultMoveSmoothing)
    , rotateSmoothing(defaultRotateSmoothing)
    , zoomSmoothing(defaultZoomSmoothing)
    , dPivotPos(Eigen::Vector3f::Zero())
    , dRadius(0)
    , dScale(0)
    , minRadius(0.01f)
    , maxRadius(1000.0f)
    , minScale(0.01f)
    , maxScale(1000.0f)
    , dYpr(Eigen::Vector3f::Zero())
    , lastMousePosition(-1, -1)
    , leftClicked(false)
    , rightClicked(false)
    , middleClicked(false)
    , minPos(Eigen::Vector3f(Eigen::NumTraits<float>::lowest(), Eigen::NumTraits<float>::lowest(), Eigen::NumTraits<float>::lowest()))
    , maxPos(
          Eigen::Vector3f(Eigen::NumTraits<float>::highest(), Eigen::NumTraits<float>::highest(), Eigen::NumTraits<float>::highest()))
    , settings(Settings())
    , m_keysPressed{}

{}

OrbitCameraController::~OrbitCameraController() {}

// ----------------------------------------- Events
void OrbitCameraController::keyPressEvent(QKeyEvent* e) {
  m_keysPressed += e->key();
}

void OrbitCameraController::keyReleaseEvent(QKeyEvent* e) {
  m_keysPressed -= e->key();
}

void OrbitCameraController::focusOutEvent(QFocusEvent* event) {
  if (leftClicked)
    leftClicked = false;
  if (rightClicked)
    rightClicked = false;
  if (middleClicked)
    middleClicked = false;

  event->accept();
  emit focusLost();
}

void OrbitCameraController::mouseMoveEvent(QMouseEvent* e) {
  // Initialization
  if (lastMousePosition == QPointF(-1, -1)) {
    lastMousePosition = e->globalPosition();
    emit mouseMove(e->globalPosition(), e->globalPosition());
    return;
  }

  // Perform the proper action
  QPointF dMouse = e->globalPosition() - lastMousePosition;
  if (rightClicked && settings.enableTrasl) {
    Eigen::Vector3f movement = (getCamera().left() * dMouse.x()) + (getCamera().up() * dMouse.y());
    dPivotPos += movement * mouseMoveSpeed * moveSpeedScale * pow(radius, 0.75f);
  }

  if (leftClicked && settings.enableRot) {
    dYpr.x() -= dMouse.x() * rotateSpeed;
    dYpr.y() += dMouse.y() * rotateSpeed;
  }

  // Done
  QPointF _lastMousePosition = lastMousePosition;
  lastMousePosition = e->globalPosition();
  e->accept();
  emit mouseMove(e->globalPosition(), _lastMousePosition);
}

void OrbitCameraController::mousePressEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) {
    leftClicked = true;
    lastMousePosition = QPointF(-1, -1);
    emit leftPressed(e);
  } else if (e->button() == Qt::RightButton) {
    rightClicked = true;
    lastMousePosition = QPointF(-1, -1);
    emit rightPressed(e);
  } else if (e->button() == Qt::MouseButton::MiddleButton) {
    middleClicked = true;
    lastMousePosition = QPointF(-1, -1);
    emit middlePressed(e);
  }

  e->accept();
}

void OrbitCameraController::mouseReleaseEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) {
    leftClicked = false;
    lastMousePosition = QPointF(-1, -1);
    emit leftReleased(e);
  } else if (e->button() == Qt::RightButton) {
    rightClicked = false;
    lastMousePosition = QPointF(-1, -1);
    emit rightReleased(e);
  } else if (e->button() == Qt::MouseButton::MiddleButton) {
    middleClicked = false;
    lastMousePosition = QPointF(-1, -1);
    emit middleReleased(e);
  }

  e->accept();
}

void OrbitCameraController::mouseDoubleClick(QMouseEvent* e) {
  e->accept();
  emit leftDoubleClicked(e);
}

void OrbitCameraController::wheelEvent(QWheelEvent* e) {
  if (settings.enableScale) {
    if (getCamera().isPerspective()) {
      dRadius = -e->angleDelta().y() * zoomSpeed * zoomSpeedScale * pow(radius, 0.5f);
    } else {
      dScale = -e->angleDelta().y() * zoomSpeed * zoomSpeedScale * pow(scale, 0.5f);
    }
    e->accept();
  }

  emit wheelScrolled(e);
}

// ----------------------------------------- Update

void OrbitCameraController::resetDestinations() {
  pivotPosDest = pivotPos;
  radiusDest = radius;
  scaleDest = scale;
}

void OrbitCameraController::update(float dT) {
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
    speedBoost = keyboardSpeedBoost;
  dPivotPos += (getCamera().left() * keyDelta.x()) * keyboardMoveSpeed * moveSpeedScale * speedBoost * dT * pow(radius, 0.75f);
  if (getCamera().isPerspective()) {
    dPivotPos += (getCamera().forward() * keyDelta.y()) * keyboardMoveSpeed * moveSpeedScale * speedBoost * dT * pow(radius, 0.75f);
  } else {
    dScale -= keyDelta.y() * keyboardMoveSpeed * moveSpeedScale * speedBoost * dT * pow(scale, 0.75f);
  }

  // Lerp position
  pivotPosDest += dPivotPos;
  pivotPos += (pivotPosDest - pivotPos) * moveSmoothing;

  // Lerp radius
  radiusDest += dRadius;
  if (radiusDest < minRadius)
    radiusDest = minRadius;
  if (radiusDest > maxRadius)
    radiusDest = maxRadius;
  radius += (radiusDest - radius) * zoomSmoothing;

  // Lerp scale
  scaleDest += dScale;
  if (scaleDest < minScale)
    scaleDest = minScale;
  if (scaleDest > maxScale)
    scaleDest = maxScale;
  scale += (scaleDest - scale) * zoomSmoothing;

  // Lerp rotation
  yprDest += dYpr;

  for (int i = 0; i < 3; i++) {
    while (ypr(i) - yprDest(i) < -M_PI)
      yprDest(i) -= float(2 * M_PI);
    while (ypr(i) - yprDest(i) >= M_PI)
      yprDest(i) += float(2 * M_PI);
  }
  Eigen::Vector3f diff = yprDest - ypr;
  for (int i = 0; i < 3; i++) {
    if (diff(i) < -M_PI)
      diff(i) = (2 * M_PI) + diff(i);
    if (diff(i) >= M_PI)
      diff(i) = (2 * M_PI) - diff(i);
  }
  ypr += diff * rotateSmoothing;
  normalizeYpr(ypr);
  Eigen::Matrix3f R;
  R = (Eigen::AngleAxisf(ypr(0), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(ypr(1), Eigen::Vector3f::UnitY()) *
       Eigen::AngleAxisf(ypr(2), Eigen::Vector3f::UnitX()));

  // Update pose
  Eigen::Isometry3f pose = getCamera().getPose();
  pose.linear() = R;
  pose.translation() = pivotPos - (R.col(0) * radius * (getCamera().isPerspective() ? 1 : 0));
  for (int i = 0; i < 3; i++) {
    if (pose.translation()(i) < minPos(i)) {
      pose.translation()(i) = minPos(i);
      pivotPos(i) = pose.translation()(i) + (R.col(0)(i) * radius * (getCamera().isPerspective() ? 1 : 0));
      pivotPosDest(i) = pivotPos(i);
    }
    if (pose.translation()(i) > maxPos(i)) {
      pose.translation()(i) = maxPos(i);
      pivotPos(i) = pose.translation()(i) + (R.col(0)(i) * radius * (getCamera().isPerspective() ? 1 : 0));
      pivotPosDest(i) = pivotPos(i);
    }
  }

  setCameraPose(pose);

  // update scale
  setCameraScale(scale);

  // Reset velocities
  dPivotPos.setZero();
  dYpr.setZero();
  dRadius = 0;
  dScale = 0;
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
  pivotPosDest = position;
  if (!smooth) {
    pivotPos = pivotPosDest;
    Eigen::Isometry3f pose = getCamera().getPose();
    pose.translation() = pivotPos - (getCamera().forward() * radius * (getCamera().isPerspective() ? 1 : 0));

    setCameraPose(pose);
  }
}

void OrbitCameraController::setYpr(const Eigen::Vector3f& ypr, bool smooth) {
  yprDest = ypr;
  if (!smooth) {
    this->ypr = yprDest;
    Eigen::Vector3f pos = getCamera().getPose().translation();
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
  pose.translation() = getCamera().getPose().translation();

  yprDest = pose.linear().eulerAngles(2, 1, 0);
  if (!smooth) {
    this->ypr = yprDest;
    setCameraPose(pose);
  }
}

void OrbitCameraController::setRadius(float r, bool smooth) {
  radiusDest = r;
  if (!smooth) {
    radius = radiusDest;
    auto pose = getCamera().getPose();
    pose.translation() = pivotPos - (getCamera().forward() * radius * (getCamera().isPerspective() ? 1 : 0));

    setCameraPose(pose);
  }
}

void OrbitCameraController::setZNearFar(float zNear, float zFar) {
  setCameraClippingPlanes(zNear, zFar);
}

void OrbitCameraController::setScale(float s, bool smooth) {
  scaleDest = s;
  if (!smooth) {
    scale = scaleDest;
    setCameraScale(scaleDest);
  }
}

void OrbitCameraController::setTraslationLimits(const Eigen::Vector3f& minPos, const Eigen::Vector3f& maxPos) {
  this->minPos = minPos;
  this->maxPos = maxPos;
}

void OrbitCameraController::setCameraOrthogonal() {
  CameraController::setCameraOrthogonal();
}
void OrbitCameraController::setCameraPerspective(float fovDeg) {
  setCameraFov(fovDeg);
  this->setScale(1.0, false);
}

} // namespace csVisOpenGL
