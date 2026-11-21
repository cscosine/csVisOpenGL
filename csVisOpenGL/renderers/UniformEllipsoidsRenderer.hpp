#pragma once

#include <Eigen/Geometry>
#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class UniformEllipsoidsRenderer3D : public Renderer {
public:
  UniformEllipsoidsRenderer3D();
  virtual ~UniformEllipsoidsRenderer3D();

  void setLineWidth(float lineWidth) { this->m_lineWidth = lineWidth; }

  void setUniformColor(const Eigen::Vector4f &color);
  void setUniformColor(const Eigen::Vector3f &color);

  void setCovariances(const std::vector<Eigen::Isometry3f> &poses, const std::vector<Eigen::Matrix3d> &covariances,
                      float nSigma);
  void setCovariances(const Eigen::Isometry3f &poses, const Eigen::Matrix3d &covariances, float nSigma);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) override;

  GLfloat m_lineWidth;

  Eigen::Matrix3Xf m_spherePoints;
};

class UniformEllipsoidsRenderer2D : public Renderer {

public:
  UniformEllipsoidsRenderer2D();
  virtual ~UniformEllipsoidsRenderer2D();

  void setLineWidth(float lineWidth) { this->m_lineWidth = lineWidth; }

  void setUniformColor(const Eigen::Vector4f &color);
  void setUniformColor(const Eigen::Vector3f &color);

  void setCovariances(const Eigen::Isometry3f &poses, const Eigen::Matrix2d &covariances, float nSigma);
  void setCovariances(const std::vector<Eigen::Isometry3f> &poses, const std::vector<Eigen::Matrix2d> &covariances,
                      float nSigma);
  // void setCovariances(const std::vector<Pose2Dd> & poses, std::vector<Eigen::Matrix2d> & covariances, float nSigma);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) override;

  GLfloat m_lineWidth;

  Eigen::Matrix3Xf m_circlePoints;
};

} // namespace csVisOpenGL
