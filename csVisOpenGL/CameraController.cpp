#include <QKeyEvent>
#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/CameraController.hpp>

namespace csVisOpenGL {

CameraController::CameraController()
    : m_camera() {}

CameraController::~CameraController() {}

void CameraController::setCameraPose(Eigen::Isometry3f const& pose) {
  m_camera.setPose(pose);
}

void CameraController::setCameraScale(float scale) {
  m_camera.setScale(scale);
}

void CameraController::setCameraClippingPlanes(float zNear, float zFar) {
  m_camera.setClippingPlanes(zNear, zFar);
}

void CameraController::setCameraOrthogonal() {
  m_camera.setOrthogonal();
}

void CameraController::setCameraFov(float fov) {
  m_camera.setFov(fov);
}

} // namespace csVisOpenGL
