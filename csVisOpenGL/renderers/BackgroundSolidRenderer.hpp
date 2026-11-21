#pragma once
#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {

class BackgroundSolidRenderer : public Renderer {

public:
  BackgroundSolidRenderer();
  virtual ~BackgroundSolidRenderer() {};

  void setColor(const Eigen::Vector4f &color);
  void setColor(const Eigen::Vector3f &color);

  void setDefaultColor();

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) override;

  Eigen::Vector4f m_color;
};

} // namespace csVisOpenGL
