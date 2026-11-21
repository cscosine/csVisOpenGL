#include <csVisOpenGL/Colors.hpp>
#include <csVisOpenGL/renderers/BackgroundSolidRenderer.hpp>

#include <csVisOpenGL/ShaderFactory.hpp>

namespace csVisOpenGL {

BackgroundSolidRenderer::BackgroundSolidRenderer() : Renderer(ShaderFactory::ShaderNames::Background) {
  setDefaultColor();
}

void BackgroundSolidRenderer::setDefaultColor() { setColor(Color::white); }

void BackgroundSolidRenderer::setColor(const Eigen::Vector4f &color) { this->m_color = color; }

void BackgroundSolidRenderer::setColor(const Eigen::Vector3f &color) { this->m_color.head<3>() = color; }

void BackgroundSolidRenderer::initializeConcrete() {}

void BackgroundSolidRenderer::drawConcrete(const Camera &camera, const Eigen::Matrix4f &m) {
  glClearColor(m_color.x(), m_color.y(), m_color.z(), m_color.w());
  glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace csVisOpenGL
