#include <cassert>
#include <csVisOpenGL/renderers/UniformEllipsoidsRenderer.hpp>

#include <math.h>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformEllipsoidsRenderer3D::UniformEllipsoidsRenderer3D()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , m_lineWidth(1) {
  // create sphere points
  float radius = 1;
  int rings = 32;
  int sectors = 32;
  float const R = 1. / (float)(rings - 1);
  float const S = 1. / (float)(sectors - 1);
  Eigen::Matrix3Xf points;
  points.resize(Eigen::NoChange, rings * sectors);
  int pos = 0;
  for (int r = 0; r < rings; r++) {
    for (int s = 0; s < sectors; s++) {
      float const y = sin(-M_PI_2 + M_PI * r * R);
      float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
      float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

      points.col(pos++) = Eigen::Vector3f(x * radius, y * radius, z * radius);
    }
  }

  m_spherePoints.resize(Eigen::NoChange, (rings - 1) * sectors * 4);
  pos = 0;
  for (int r = 0; r < rings - 1; r++) {
    for (int s = 0; s < sectors; s++) {
      m_spherePoints.col((pos * 4) + 0) = points.col(pos);
      m_spherePoints.col((pos * 4) + 1) = points.col(pos + 1);
      m_spherePoints.col((pos * 4) + 2) = points.col(pos);
      m_spherePoints.col((pos * 4) + 3) = points.col(pos + sectors);
      pos++;
    }
  }
}

UniformEllipsoidsRenderer3D::~UniformEllipsoidsRenderer3D() {}

void UniformEllipsoidsRenderer3D::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformEllipsoidsRenderer3D::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glLineWidth(m_lineWidth);

  getShader().bind();
  {
    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    glDrawArrays(GL_LINES, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void UniformEllipsoidsRenderer3D::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void UniformEllipsoidsRenderer3D::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void UniformEllipsoidsRenderer3D::setCovariances(const Eigen::Isometry3f& poses, const Eigen::Matrix3d& covariances, float nSigma) {
  setCovariances(std::vector<Eigen::Isometry3f>{poses}, std::vector<Eigen::Matrix3d>{covariances}, nSigma);
}

void UniformEllipsoidsRenderer3D::setCovariances(const std::vector<Eigen::Isometry3f>& poses,
                                                 const std::vector<Eigen::Matrix3d>& covariances, float nSigma) {

  Eigen::Matrix3Xf lines(3, poses.size() * m_spherePoints.cols());
  for (int i = 0; i < poses.size(); i++) {
    Eigen::Matrix3d eigenVectors = Eigen::Matrix3d::Zero();
    Eigen::Vector3d eigenValues = Eigen::Vector3d::Zero();

    if (covariances[i].determinant() != 0) {
      Eigen::JacobiSVD<Eigen::Matrix3d> svd(covariances[i], Eigen::ComputeFullU);
      eigenVectors = svd.matrixU();
      eigenValues = svd.singularValues().cwiseSqrt();
    }

    Eigen::Matrix3d covR = eigenVectors;

    Eigen::Matrix3d scale = nSigma * eigenValues.asDiagonal();

    lines.block(0, i * m_spherePoints.cols(), 3, m_spherePoints.cols()) =
        poses[i].cast<float>() * (covR.cast<float>() * scale.cast<float>() * m_spherePoints);
  }

  getVao().uploadPositions(getShader().getVertexLocations().position, lines.data(), lines.cols());
}

//---------------------------------------------------------------------------------------------------------

UniformEllipsoidsRenderer2D::UniformEllipsoidsRenderer2D()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , m_lineWidth(1) {
  // create sphere points
  int sectors = 32;
  float const S = 1. / (float)(sectors - 1);
  Eigen::Matrix3Xf points;
  m_circlePoints.resize(Eigen::NoChange, 2 * sectors);
  int pos = 0;
  m_circlePoints.col(pos++) = Eigen::Vector3f(1, 0.0, 0.0);
  for (int s = 1; s < sectors; s++) {
    float const x = cos(2 * M_PI * s * S);
    float const y = sin(2 * M_PI * s * S);

    m_circlePoints.col(pos++) = Eigen::Vector3f(x, y, 0.0);
    m_circlePoints.col(pos++) = Eigen::Vector3f(x, y, 0.0);
  }
  m_circlePoints.col(pos++) = m_circlePoints.col(0);
  assert(pos == m_circlePoints.cols());
}

UniformEllipsoidsRenderer2D::~UniformEllipsoidsRenderer2D() {}

void UniformEllipsoidsRenderer2D::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformEllipsoidsRenderer2D::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glLineWidth(m_lineWidth);

  getShader().bind();
  {
    getQOpenGLExtraFunctions().glBindVertexArray(getVao().getVao());
    glDrawArrays(GL_LINES, 0, getVao().size());
    getQOpenGLExtraFunctions().glBindVertexArray(0);
  }
  getShader().release();
}

void UniformEllipsoidsRenderer2D::setUniformColor(const Eigen::Vector4f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  getShader().release();
}

void UniformEllipsoidsRenderer2D::setUniformColor(const Eigen::Vector3f& color) {
  getShader().bind();
  getQOpenGLExtraFunctions().glUniform4f(getShader().getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  getShader().release();
}

void UniformEllipsoidsRenderer2D::setCovariances(const Eigen::Isometry3f& poses, const Eigen::Matrix2d& covariances, float nSigma) {
  setCovariances(std::vector<Eigen::Isometry3f>{poses}, std::vector<Eigen::Matrix2d>{covariances}, nSigma);
}

void UniformEllipsoidsRenderer2D::setCovariances(const std::vector<Eigen::Isometry3f>& poses,
                                                 const std::vector<Eigen::Matrix2d>& covariances, float nSigma) {

  Eigen::Matrix3Xf lines(3, poses.size() * m_circlePoints.cols());
  for (int i = 0; i < poses.size(); i++) {
    Eigen::Matrix2d eigenVectors = Eigen::Matrix2d::Zero();
    Eigen::Vector2d eigenValues = Eigen::Vector2d::Zero();

    if (covariances[i].determinant() != 0) {
      Eigen::JacobiSVD<Eigen::Matrix2d> svd(covariances[i], Eigen::ComputeFullU);
      eigenVectors = svd.matrixU();
      eigenValues = svd.singularValues().cwiseSqrt();
    }

    Eigen::Matrix3d covR = Eigen::Matrix3d::Zero();
    covR.topLeftCorner<2, 2>() = eigenVectors;

    Eigen::Matrix3d scale = Eigen::Matrix3d::Identity();
    scale.topLeftCorner<2, 2>() = nSigma * eigenValues.asDiagonal();

    lines.block(0, i * m_circlePoints.cols(), 3, m_circlePoints.cols()) =
        poses[i].cast<float>() * (covR.cast<float>() * scale.cast<float>() * m_circlePoints);
  }

  getVao().uploadPositions(getShader().getVertexLocations().position, lines.data(), lines.cols());
}

} // namespace csVisOpenGL
