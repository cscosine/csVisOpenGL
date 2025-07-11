#include "BackgroundSolidRenderer.h"
#include "Colors.h"

#include "ShaderFactory.h"
#include <iostream>

namespace csVisOpenGL {

BackgroundSolidRenderer::BackgroundSolidRenderer()
    : Renderer(ShaderFactory::ShaderNames::Background) {
  setDefaultColor();
}

void BackgroundSolidRenderer::setDefaultColor() {
  setColor(Color::white);
}

void BackgroundSolidRenderer::setColor(const Eigen::Vector4f& color) {
  this->color = color;
}

void BackgroundSolidRenderer::setColor(const Eigen::Vector3f& color) {
  this->color.head<3>() = color;
}

void BackgroundSolidRenderer::initializeConcrete() {}

void BackgroundSolidRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glClearColor(color.x(), color.y(), color.z(), color.w());
  glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace csVisOpenGL
