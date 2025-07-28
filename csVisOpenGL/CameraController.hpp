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

  void setCameraPose(Eigen::Isometry3f const& pose);
  void setCameraScale(float scale);
  void setCameraClippingPlanes(float zNear, float zFar);
  void setCameraOrthogonal();
  void setCameraFov(float fov);

public:
  CameraController(Camera& camera);
  virtual ~CameraController();

  virtual void focusOutEvent(QFocusEvent* event) = 0;
  virtual void keyPressEvent(QKeyEvent* e) = 0;
  virtual void keyReleaseEvent(QKeyEvent* e) = 0;
  virtual void mouseMoveEvent(QMouseEvent* e) = 0;
  virtual void mousePressEvent(QMouseEvent* e) = 0;
  virtual void mouseReleaseEvent(QMouseEvent* e) = 0;
  virtual void mouseDoubleClick(QMouseEvent* e) = 0;
  virtual void wheelEvent(QWheelEvent* e) = 0;

  virtual void update(float dT) = 0;

private:
  Camera& m_camera;
};

} // namespace csVisOpenGL
