#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace csVisOpenGL {
class Camera {
  Eigen::Matrix4f projection;
  Eigen::Matrix4f inverseProjection;
  Eigen::Matrix4f view;
  Eigen::Matrix4f inverseView;
  Eigen::Matrix4f viewProjection;

  Eigen::Isometry3f pose;

  float scale;
  float fov;
  float zNear;
  float zFar;
  Eigen::Vector2f viewSize;

public:
  static const float zNearDefault, zFarDefault;

  Camera();
  virtual ~Camera();

  bool isPerspective() const {
    return fov > 0;
  }
  bool isOrthogonal() const {
    return !isPerspective();
  }

  void setOrthogonal();
  void setFov(float fov);
  void setClippingPlanes(float zNear, float zFar);
  void setViewSize(const Eigen::Vector2f& viewSize);
  void setPose(const Eigen::Isometry3f& pose);

  void setScale(float s);

  float getScale() const {
    return scale;
  }
  float getFov() const {
    return fov;
  }
  float getZNear() const {
    return zNear;
  }
  float getZFar() const {
    return zFar;
  }
  const Eigen::Vector2f& getViewSize() const {
    return viewSize;
  }
  const Eigen::Isometry3f& getPose() const {
    return pose;
  }

  const Eigen::Matrix4f& getProjection() const {
    return projection;
  }
  const Eigen::Matrix4f& getInverseProjection() const {
    return inverseProjection;
  }
  const Eigen::Matrix4f& getView() const {
    return view;
  }
  const Eigen::Matrix4f& getInverseView() const {
    return inverseView;
  }
  const Eigen::Matrix4f& getViewProjection() const {
    return viewProjection;
  }

  Eigen::Vector3f forward() const {
    return pose.rotation().col(0);
  }
  Eigen::Vector3f left() const {
    return pose.rotation().col(1);
  }
  Eigen::Vector3f up() const {
    return pose.rotation().col(2);
  }

  Eigen::Vector2f worldToScreen(const Eigen::Vector3f& worldPosition, bool flipY) const;
  Eigen::Vector3f screenToWorld(const Eigen::Vector2f& screenPosition, bool flipY) const;

private:
  void updateProjectionMatrix();
  void updateViewMatrix();
};
} // namespace csVisOpenGL
