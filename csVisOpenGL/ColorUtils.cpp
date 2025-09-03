#include <QColor>
#include <csVisOpenGL/ColorUtils.hpp>

namespace csVisOpenGL {

Eigen::Vector3f ColorUtils::fromHSV(int h, int s, int v) {
  auto qc = QColor::fromHsv(h, s, v);
  return Eigen::Vector3f(qc.redF(), qc.greenF(), qc.blueF());
}

Eigen::Vector3f ColorUtils::gradientBlueToGreen(float v, float min, float max) {
  float vr = (v - min) / (max - min);
  vr = std::max(vr, 0.0f);
  vr = std::min(vr, 1.0f);

  int h = 240.0f - vr * 120.0f;
  return fromHSV(h, 255, 255);
}

Eigen::Vector3f ColorUtils::gradientRedToGreen(float v, float min, float max, bool passBlue) {
  float vr = (v - min) / (max - min);
  vr = std::max(vr, 0.0f);
  vr = std::min(vr, 1.0f);

  if (passBlue) {
    int h = 359.0f - vr * 239.0f;
    return fromHSV(h, 255, 255);
  } else {
    int h = vr * 119.0f;
    return fromHSV(h, 255, 255);
  }
}

Eigen::Vector3f ColorUtils::gradientGreenToRed(float v, float min, float max, bool passBlue) {
  float vr = (v - min) / (max - min);
  vr = std::max(vr, 0.0f);
  vr = std::min(vr, 1.0f);
  vr = 1 - vr;

  if (passBlue) {
    int h = 359.0f - vr * 239.0f;
    return fromHSV(h, 255, 255);
  } else {
    int h = vr * 119.f;
    return fromHSV(h, 255, 255);
  }
}

Eigen::Matrix3Xf ColorUtils::gradientBlueToGreen(const std::vector<double>& v, float min, float max) {
  Eigen::Matrix3Xf mat(3, v.size());
  for (size_t i = 0; i < v.size(); i++) {
    mat.col(i) = gradientBlueToGreen(v[i], min, max);
  }
  return mat;
}

Eigen::Matrix3Xf ColorUtils::gradientRedToGreen(const std::vector<double>& v, float min, float max) {
  Eigen::Matrix3Xf mat(3, v.size());
  for (int i = 0; i < v.size(); i++) {
    mat.col(i) = gradientRedToGreen(v[i], min, max);
  }
  return mat;
}

Eigen::Matrix3Xf ColorUtils::gradientGreenToRed(const std::vector<double>& v, float min, float max) {
  Eigen::Matrix3Xf mat(3, v.size());
  for (int i = 0; i < v.size(); i++) {
    mat.col(i) = gradientGreenToRed(v[i], min, max);
  }
  return mat;
}

} // namespace csVisOpenGL
