#pragma once

#include <Eigen/Core>
#include <vector>

namespace csVisOpenGL {

class ColorUtils {
public:
  static Eigen::Vector3f fromHSV(int h, int s, int v);
  static Eigen::Vector3f gradientBlueToGreen(float v, float min, float max);
  static Eigen::Vector3f gradientRedToGreen(float v, float min, float max, bool passBlue = true);
  static Eigen::Vector3f gradientGreenToRed(float v, float min, float max, bool passBlue = true);

  static Eigen::Matrix3Xf gradientBlueToGreen(const std::vector<double> &v, float min, float max);
  static Eigen::Matrix3Xf gradientRedToGreen(const std::vector<double> &v, float min, float max);
  static Eigen::Matrix3Xf gradientGreenToRed(const std::vector<double> &v, float min, float max);

  template <typename Derived>
  static Eigen::Matrix3Xf gradientBlueToGreen(const Eigen::MatrixBase<Derived> &v, float min, float max) {
    Eigen::Matrix3Xf mat(3, v.size());

    for (int i = 0; i < v.size(); i++) {
      mat.col(i) = gradientBlueToGreen(v(i), min, max);
    }
    return mat;
  }

  template <typename Derived>
  static Eigen::Matrix3Xf gradientRedToGreen(const Eigen::MatrixBase<Derived> &v, float min, float max) {
    Eigen::Matrix3Xf mat(3, v.size());

    for (int i = 0; i < v.size(); i++) {
      mat.col(i) = gradientRedToGreen(v(i), min, max);
    }
    return mat;
  }

  template <typename Derived>
  static Eigen::Matrix3Xf gradientGreenToRed(const Eigen::MatrixBase<Derived> &v, float min, float max) {
    Eigen::Matrix3Xf mat(3, v.size());

    for (int i = 0; i < v.size(); i++) {
      mat.col(i) = gradientGreenToRed(v(i), min, max);
    }
    return mat;
  }
};
} // namespace csVisOpenGL
