#pragma once
#include "Renderer.h"

namespace csVisOpenGL {

class BackgroundSolidRenderer : public Renderer {

private:
  Eigen::Vector4f color;

public:
  BackgroundSolidRenderer();
  virtual ~BackgroundSolidRenderer() {};

  void setColor(const Eigen::Vector4f& color);
  void setColor(const Eigen::Vector3f& color);

  void setDefaultColor();

protected:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};

} // namespace csVisOpenGL
