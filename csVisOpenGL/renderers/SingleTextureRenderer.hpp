#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {

class SingleTextureRenderer : public Renderer {
private:
public:
  using Matrix3x4f = Eigen::Matrix<float, 3, 4>;
  using Matrix2x4f = Eigen::Matrix<float, 2, 4>;

  enum class TextureInterpolation { Linear, Nearest };

  SingleTextureRenderer();
  virtual ~SingleTextureRenderer();

  void setUniformColor(const Eigen::Vector4f &color);
  void setUniformColor(const Eigen::Vector3f &color);

  void setTexture(GLuint texid, const Matrix3x4f &vertex, const Matrix2x4f &texCoord, TextureInterpolation ti);
  void resetTexture() { m_texid = -1; }
  GLuint textureId() const { return m_texid; }

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) override;

  GLuint m_texid;
};

} // namespace csVisOpenGL
