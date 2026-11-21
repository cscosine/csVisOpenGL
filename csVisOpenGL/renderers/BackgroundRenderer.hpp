#pragma once
#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {

class BackgroundRenderer : public Renderer {

private:
public:
  BackgroundRenderer();
  virtual ~BackgroundRenderer() {};

  void setColors(const Eigen::Vector4f &zenithColor, const Eigen::Vector4f &horizonColor,
                 const Eigen::Vector4f &nadirColor);
  void setColors(const Eigen::Vector3f &zenithColor, const Eigen::Vector3f &horizonColor,
                 const Eigen::Vector3f &nadirColor);

  void setDefaultColors();

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) override;
};

} // namespace csVisOpenGL
