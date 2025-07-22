#pragma once

#include <Eigen/Geometry>
#include <QSet>

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace csVisOpenGL {

class Camera;

class CameraController {

protected:
  Camera const& getCamera() const {
    return m_camera;
  }
  QSet<int> const& getKeysPressed() const {
    return m_keysPressed;
  }

  void setCameraPose(Eigen::Isometry3f const& pose);
  void setCameraScale(float scale);
  void setCameraClippingPlanes(float zNear, float zFar);
  void setCameraOrthogonal();
  void setCameraFov(float fov);

public:
  CameraController(Camera& camera);
  virtual ~CameraController();

  virtual void focusOutEvent(QFocusEvent* event) {}
  virtual void keyPressEvent(QKeyEvent* e);
  virtual void keyReleaseEvent(QKeyEvent* e);
  virtual void mouseMoveEvent(QMouseEvent* e) {}
  virtual void mousePressEvent(QMouseEvent* e) {}
  virtual void mouseReleaseEvent(QMouseEvent* e) {}
  virtual void mouseDoubleClick(QMouseEvent* e) {}
  virtual void wheelEvent(QWheelEvent* e) {}

  virtual void update(float dT) = 0;

private:
  Camera& m_camera;
  QSet<int> m_keysPressed;
};

} // namespace csVisOpenGL
