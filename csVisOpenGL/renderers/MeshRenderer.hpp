#pragma once

#include <csVisOpenGL/Renderer.hpp>

namespace csVisOpenGL {
class MeshRenderer : public Renderer {
public:
  struct Light {
    bool enabled;
    Eigen::Vector3f position;
    Eigen::Vector3f dir;
    Eigen::Vector3f color;
    bool spot;
    bool specularEnabled;
    float attenuation;
    float ambientCoefficient;
    float coneAngle;

    Light();
  };

  bool cull;
  std::vector<Light*> lights;

public:
  MeshRenderer();
  virtual ~MeshRenderer();

  void setCullFaceEnabled(bool set) {
    this->cull = set;
  }

  void setUniformColor(const Eigen::Vector4f& color);
  void setUniformColor(const Eigen::Vector3f& color);
  void setMaterialShininess(float shininess);
  void setMaterialSpecularIntensity(float specular);

  void setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& normals);

  int getLightCount() const {
    return int(lights.size());
  }
  Light& getLight(int i) {
    assert(i >= 0);
    assert(i < lights.size());
    return *lights[i];
  }
  void updateLight(int i);

private:
  virtual void initializeConcrete() override;
  virtual void drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) override;
};
} // namespace csVisOpenGL
