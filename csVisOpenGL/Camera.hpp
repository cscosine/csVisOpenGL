#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace csVisOpenGL {
class Camera {

public:
  static const float zNearDefault, zFarDefault;

  Camera();
  virtual ~Camera();

  bool isPerspective() const { return m_fov > 0; }
  bool isOrthogonal() const { return !isPerspective(); }

  void setOrthogonal();
  void setFov(float fov);
  void setClippingPlanes(float zNear, float zFar);
  void setViewSize(const Eigen::Vector2f &viewSize);
  void setPose(const Eigen::Isometry3f &pose);

  void setScale(float s);

  // only to init camera controller
  float getScale() const { return m_scale; }

  // in frame
  const Eigen::Vector2f &getViewSize() const { return m_viewSize; }

  // in orbit camera controller and in mesh renderer (for lights?)
  const Eigen::Isometry3f &getPose() const { return m_pose; }

  // in renderers for uniforms
  const Eigen::Matrix4f &getProjection() const { return m_projection; }
  const Eigen::Matrix4f &getInverseProjection() const { return m_inverseProjection; }
  const Eigen::Matrix4f &getView() const { return m_view; }
  const Eigen::Matrix4f &getInverseView() const { return m_inverseView; }
  const Eigen::Matrix4f &getViewProjection() const { return m_viewProjection; }

  // can be changed with rotation, used in orbit camera controller
  Eigen::Vector3f forward() const { return m_pose.rotation().col(0); }
  Eigen::Vector3f left() const { return m_pose.rotation().col(1); }
  Eigen::Vector3f up() const { return m_pose.rotation().col(2); }

  Eigen::Vector2f worldToScreen(const Eigen::Vector3f &worldPosition, bool flipY) const;
  Eigen::Vector3f screenToWorld(const Eigen::Vector2f &screenPosition, bool flipY) const;

private:
  void updateProjectionMatrix();
  void updateViewMatrix();

  Eigen::Matrix4f m_projection;
  Eigen::Matrix4f m_inverseProjection;
  Eigen::Matrix4f m_view;
  Eigen::Matrix4f m_inverseView;
  Eigen::Matrix4f m_viewProjection;

  Eigen::Isometry3f m_pose;

  float m_scale;
  float m_fov;
  float m_zNear;
  float m_zFar;
  Eigen::Vector2f m_viewSize;
};
} // namespace csVisOpenGL
