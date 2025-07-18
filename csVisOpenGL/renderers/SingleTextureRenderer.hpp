#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {

class SingleTextureRenderer : public Renderer {
private:
  GLuint texid;

public:
  using Matrix3x4f = Eigen::Matrix<float, 3, 4>;
  using Matrix2x4f = Eigen::Matrix<float, 2, 4>;

  enum class TextureInterpolation { Linear, Nearest };

  SingleTextureRenderer();
  virtual ~SingleTextureRenderer();

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);

  void setTexture(GLuint texid, const Matrix3x4f& vertex, const Matrix2x4f& texCoord, TextureInterpolation ti);
  void resetTexture() {
    texid = -1;
  }
  GLuint textureId() const {
    return texid;
  }

protected:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};

} // namespace csVisOpenGL
