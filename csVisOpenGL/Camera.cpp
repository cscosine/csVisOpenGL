#include <csVisOpenGL/Camera.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace csVisOpenGL {

const float Camera::zNearDefault = 0.01f;
const float Camera::zFarDefault = 1000.0f;

Camera::Camera()
    : m_fov(45.0f)
    , m_zNear(zNearDefault)
    , m_zFar(zFarDefault)
    , m_viewSize(Eigen::Vector2f::Constant(100))
    , m_pose(Eigen::Isometry3f::Identity())
    , m_scale(1.0f) {
  updateProjectionMatrix();
  updateViewMatrix();
}

Camera::~Camera() {}

void Camera::setOrthogonal() {
  m_fov = -1;
  updateProjectionMatrix();
}

void Camera::setScale(float s) {
  m_scale = s;
  updateProjectionMatrix();
}

void Camera::setFov(float fov) {
  m_fov = fov;
  updateProjectionMatrix();
}

void Camera::setClippingPlanes(float zNear, float zFar) {
  m_zNear = zNear;
  m_zFar = zFar;
  updateProjectionMatrix();
}

void Camera::setViewSize(const Eigen::Vector2f& viewSize) {
  m_viewSize = viewSize;
  updateProjectionMatrix();
}

void Camera::setPose(const Eigen::Isometry3f& pose) {
  m_pose = pose;
  updateViewMatrix();
}

void Camera::updateProjectionMatrix() {
  // From http://www.songho.ca/opengl/gl_projectionmatrix.html

  if (m_fov >= 0) {
    // Get frustum coordinates
    float const ratio = m_viewSize.x() / m_viewSize.y();
    float const top = m_scale * m_zNear * tanf(m_fov * float(M_PI) / 180.0f * 0.5f);
    float const right = m_scale * top * ratio;
    float const bottom = -top;
    float const left = -right;

    // Some intermediate calculations
    float const temp = 2.0f * m_zNear;
    float const temp2 = right - left;
    float const temp3 = top - bottom;
    float const temp4 = m_zFar - m_zNear;

    // Update projection and inverse projection matrices
    m_projection.setZero();
    m_projection(0, 0) = temp / temp2;
    m_projection(1, 1) = temp / temp3;
    m_projection(0, 2) = (right + left) / temp2;
    m_projection(1, 2) = (top + bottom) / temp3;
    m_projection(2, 2) = -(m_zFar + m_zNear) / temp4;
    m_projection(3, 2) = -1.0;
    m_projection(2, 3) = -(temp * m_zFar) / temp4;
    m_inverseProjection = m_projection.inverse();
  } else {
    // orthographic camera

    float const ratio = m_viewSize.x() / m_viewSize.y();
    float const right = m_scale * ratio;
    float const top = m_scale;

    float const tz = -(m_zFar + m_zNear) / (m_zFar - m_zNear);

    m_projection.setZero();
    m_projection(0, 0) = 1.0 / right;
    m_projection(1, 1) = 1.0 / top;
    m_projection(2, 2) = -2.0 / (m_zFar - m_zNear);

    m_projection(2, 3) = tz;
    m_projection(3, 3) = 1;
    m_inverseProjection = m_projection.inverse();
  }

  m_viewProjection = m_projection * m_view;
}

void Camera::updateViewMatrix() {
  // Considering the reference frame of the perspective projection, X axis has to be -Y, Y -> Z and Z -> -X

  m_inverseView.setIdentity();
  m_inverseView.col(0).head<3>() = -m_pose.linear().col(1);
  m_inverseView.col(1).head<3>() = m_pose.linear().col(2);
  m_inverseView.col(2).head<3>() = -m_pose.linear().col(0);
  m_inverseView.col(3).head<3>() = m_pose.translation();
  m_view = m_inverseView.inverse();

  m_viewProjection = m_projection * m_view;
}

Eigen::Vector2f Camera::worldToScreen(const Eigen::Vector3f& worldPosition, bool flipY) const {
  Eigen::Vector4f const homogeneousWorld = worldPosition.homogeneous();
  Eigen::Vector4f const homogeneousScreen = m_viewProjection * homogeneousWorld;
  Eigen::Vector2f point2dGL = Eigen::Vector2f::Zero();
  if (std::abs(homogeneousScreen.w()) > 0.000001f) {
    point2dGL = (homogeneousScreen / std::abs(homogeneousScreen.w())).head<2>();
  }
  Eigen::Vector2f point2dScreen = point2dGL / 2 + Eigen::Vector2f::Constant(0.5f);
  point2dScreen.x() *= m_viewSize.x();
  point2dScreen.y() *= m_viewSize.y();
  if (flipY)
    point2dScreen.y() = m_viewSize.y() - 1 - point2dScreen.y();

  return point2dScreen;
}

Eigen::Vector3f Camera::screenToWorld(const Eigen::Vector2f& screenPosition, bool flipY) const {
  Eigen::Vector2f screenPos = screenPosition;
  if (flipY)
    screenPos.y() = m_viewSize.y() - 1 - screenPosition.y();

  float x = (2.0f * screenPos.x()) / m_viewSize.x() - 1.0f;
  float y = (2.0f * screenPos.y()) / m_viewSize.y() - 1.0f;
  Eigen::Vector4f rayEye = m_inverseProjection * Eigen::Vector4f(x, y, -1, 1);
  rayEye.z() = -1;
  rayEye.w() = 0;
  Eigen::Vector4f rayWorld = m_inverseView * rayEye;

  return Eigen::Vector3f(rayWorld.x(), rayWorld.y(), rayWorld.z()).normalized();
}
} // namespace csVisOpenGL
