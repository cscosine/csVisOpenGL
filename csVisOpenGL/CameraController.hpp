#pragma once

#include <Eigen/Geometry>
#include <QSet>
#include <csVisOpenGL/Camera.hpp>

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace csVisOpenGL {

class CameraControllerInterface {
public:
  virtual Camera const& getUpdatedCamera(float dT) = 0;
  virtual void setViewSize(Eigen::Vector2f const& viewSize) = 0;
};
class CameraController : public CameraControllerInterface {
protected:
  void setCameraPose(Eigen::Isometry3f const& pose);
  void setCameraScale(float scale);
  void setCameraClippingPlanes(float zNear, float zFar);
  void setCameraOrthogonal();
  void setCameraFov(float fov);

public:
  CameraController();
  virtual ~CameraController();

  void setViewSize(Eigen::Vector2f const& viewSize) override {
    m_camera.setViewSize(viewSize);
  }

  Eigen::Vector2f const& getViewSize() {
    return m_camera.getViewSize();
  }

protected:
  Camera m_camera;
};

} // namespace csVisOpenGL
