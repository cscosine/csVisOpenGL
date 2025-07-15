#include <csVisOpenGL/Camera.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace csVisOpenGL {

const float Camera::zNearDefault = 0.01f;
const float Camera::zFarDefault = 1000.0f;

Camera::Camera()
    : fov(45.0f)
    , zNear(zNearDefault)
    , zFar(zFarDefault)
    , viewSize(Eigen::Vector2f::Constant(100))
    , pose(Eigen::Isometry3f::Identity())
    , scale(1.0f) {
  updateProjectionMatrix();
  updateViewMatrix();
}

Camera::~Camera() {}

void Camera::setOrthogonal() {
  this->fov = -1;
  updateProjectionMatrix();
}

void Camera::setScale(float s) {
  this->scale = s;
  updateProjectionMatrix();
}

void Camera::setFov(float fov) {
  this->fov = fov;
  updateProjectionMatrix();
}

void Camera::setClippingPlanes(float zNear, float zFar) {
  this->zNear = zNear;
  this->zFar = zFar;
  updateProjectionMatrix();
}

void Camera::setViewSize(const Eigen::Vector2f& viewSize) {
  this->viewSize = viewSize;
  updateProjectionMatrix();
}

void Camera::setPose(const Eigen::Isometry3f& pose) {
  this->pose = pose;
  updateViewMatrix();
}

void Camera::updateProjectionMatrix() {
  // From http://www.songho.ca/opengl/gl_projectionmatrix.html

  if (fov >= 0) {
    // Get frustum coordinates
    float ratio = viewSize.x() / viewSize.y();
    float top = scale * zNear * tanf(fov * float(M_PI) / 180.0f * 0.5f);
    float right = scale * top * ratio;
    float bottom = -top;
    float left = -right;

    // Some intermediate calculations
    float temp = 2.0f * zNear;
    float temp2 = right - left;
    float temp3 = top - bottom;
    float temp4 = zFar - zNear;

    // Update projection and inverse projection matrices
    projection.setZero();
    projection(0, 0) = temp / temp2;
    projection(1, 1) = temp / temp3;
    projection(0, 2) = (right + left) / temp2;
    projection(1, 2) = (top + bottom) / temp3;
    projection(2, 2) = -(zFar + zNear) / temp4;
    projection(3, 2) = -1.0;
    projection(2, 3) = -(temp * zFar) / temp4;
    inverseProjection = projection.inverse();
  } else {
    // orthographic camera
    float right, top;
    // if (viewSize.x() > viewSize.y()) {
    //   float ratio = viewSize.y() / viewSize.x();
    //   right = scale;
    //   top = scale * ratio;
    // }
    // else {
    float ratio = viewSize.x() / viewSize.y();
    right = scale * ratio;
    top = scale;
    //}

    float tz = -(zFar + zNear) / (zFar - zNear);

    projection.setZero();
    projection(0, 0) = 1.0 / right;
    projection(1, 1) = 1.0 / top;
    projection(2, 2) = -2.0 / (zFar - zNear);

    projection(2, 3) = tz;
    projection(3, 3) = 1;
    inverseProjection = projection.inverse();
  }

  viewProjection = projection * view;
}

void Camera::updateViewMatrix() {
  // Considering the reference frame of the perspective projection, X axis has to be -Y, Y -> Z and Z -> -X

  inverseView.setIdentity();
  inverseView.col(0).head<3>() = -pose.linear().col(1);
  inverseView.col(1).head<3>() = pose.linear().col(2);
  inverseView.col(2).head<3>() = -pose.linear().col(0);
  inverseView.col(3).head<3>() = pose.translation();
  view = inverseView.inverse();

  viewProjection = projection * view;
}

Eigen::Vector2f Camera::worldToScreen(const Eigen::Vector3f& worldPosition, bool flipY) const {
  Eigen::Vector4f homogeneousWorld = worldPosition.homogeneous();
  Eigen::Vector4f homogeneousScreen = viewProjection * homogeneousWorld;
  Eigen::Vector2f point2dGL = Eigen::Vector2f::Zero();
  if (std::abs(homogeneousScreen.w()) > 0.000001f)
    point2dGL = (homogeneousScreen / std::abs(homogeneousScreen.w())).head<2>();
  Eigen::Vector2f point2dScreen = point2dGL / 2 + Eigen::Vector2f::Constant(0.5f);
  point2dScreen.x() *= viewSize.x();
  point2dScreen.y() *= viewSize.y();
  if (flipY)
    point2dScreen.y() = viewSize.y() - 1 - point2dScreen.y();

  return point2dScreen;
}

Eigen::Vector3f Camera::screenToWorld(const Eigen::Vector2f& screenPosition, bool flipY) const {
  Eigen::Vector2f screenPos = screenPosition;
  if (flipY)
    screenPos.y() = viewSize.y() - 1 - screenPosition.y();

  float x = (2.0f * screenPos.x()) / viewSize.x() - 1.0f;
  float y = (2.0f * screenPos.y()) / viewSize.y() - 1.0f;
  Eigen::Vector4f rayEye = inverseProjection * Eigen::Vector4f(x, y, -1, 1);
  rayEye.z() = -1;
  rayEye.w() = 0;
  Eigen::Vector4f rayWorld = inverseView * rayEye;

  return Eigen::Vector3f(rayWorld.x(), rayWorld.y(), rayWorld.z()).normalized();
}
} // namespace csVisOpenGL
