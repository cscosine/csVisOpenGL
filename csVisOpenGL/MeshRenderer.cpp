#include <csVisOpenGL/MeshRenderer.hpp>

#include <math.h>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

MeshRenderer::Light::Light()
    : enabled(false)
    , position(Eigen::Vector3f::Zero())
    , dir(Eigen::Vector3f::UnitX())
    , color(Eigen::Vector3f::Ones())
    , spot(false)
    , specularEnabled(true)
    , attenuation(0)
    , ambientCoefficient(0.2f)
    , coneAngle(float(M_PI / 4.0f)) {}

MeshRenderer::MeshRenderer()
    : Renderer(ShaderFactory::ShaderNames::Mesh)
    , cull(false) {}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::initializeConcrete() {

  for (int i = 0; i < 10; i++)
    lights.push_back(new Light());
  for (int i = 0; i < 10; i++)
    updateLight(i);

  setUniformColor(Eigen::Vector4f(0.6f, 0.6f, 0.6f, 1.0f));
  setMaterialShininess(30.0f);
  setMaterialSpecularIntensity(0.9f);

  lights[0]->enabled = true;
  lights[0]->dir = -Eigen::Vector3f::Ones().normalized();
  lights[0]->spot = false;
  updateLight(0);
}

void MeshRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (cull)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  shader->bind();
  {
    shader->getProgram().setUniformValue("cameraPosition", camera.getPose().translation().x(), camera.getPose().translation().y(),
                                         camera.getPose().translation().z());
    glExtraFunctions->glBindVertexArray(vao->getVao());
    glExtraFunctions->glDrawArrays(GL_TRIANGLES, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);
  }
  shader->release();
}

void MeshRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void MeshRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void MeshRenderer::setMaterialShininess(float shininess) {
  shader->bind();
  shader->getProgram().setUniformValue("materialShininess", shininess);
  shader->release();
}

void MeshRenderer::setMaterialSpecularIntensity(float specular) {
  shader->bind();
  shader->getProgram().setUniformValue("materialSpecularIntensity", specular);
  shader->release();
}

void MeshRenderer::updateLight(int i) {
  if (i < 0 || i >= lights.size())
    return;

  QString lightBaseLocation = QString("lights[%1].").arg(i);
  shader->bind();
  shader->getProgram().setUniformValue((lightBaseLocation + "enabled").toStdString().c_str(), lights[i]->enabled);
  shader->getProgram().setUniformValue((lightBaseLocation + "position").toStdString().c_str(), lights[i]->position.x(),
                                       lights[i]->position.y(), lights[i]->position.z());
  shader->getProgram().setUniformValue((lightBaseLocation + "dir").toStdString().c_str(), lights[i]->dir.x(), lights[i]->dir.y(),
                                       lights[i]->dir.z());
  shader->getProgram().setUniformValue((lightBaseLocation + "color").toStdString().c_str(), lights[i]->color.x(), lights[i]->color.y(),
                                       lights[i]->color.z());
  shader->getProgram().setUniformValue((lightBaseLocation + "spot").toStdString().c_str(), lights[i]->spot);
  shader->getProgram().setUniformValue((lightBaseLocation + "specularEnabled").toStdString().c_str(), lights[i]->specularEnabled);
  shader->getProgram().setUniformValue((lightBaseLocation + "attenuation").toStdString().c_str(), lights[i]->attenuation);
  shader->getProgram().setUniformValue((lightBaseLocation + "ambientCoefficient").toStdString().c_str(),
                                       lights[i]->ambientCoefficient);
  shader->getProgram().setUniformValue((lightBaseLocation + "coneAngle").toStdString().c_str(), lights[i]->coneAngle);
  shader->release();
}

void MeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& normals) {
  vao->uploadPositions(shader->getVertexLocations().position, positions.data(), positions.cols());
  vao->uploadNormals(shader->getVertexLocations().normal, normals.data(), normals.cols());
}
} // namespace csVisOpenGL
