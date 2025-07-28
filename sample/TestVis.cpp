#include <sample/TestVis.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Colors.hpp>

#include <iostream>
#include <math.h>

TestcsVisOpenGL::TestcsVisOpenGL()
    : csVisOpenGL::VisualizerInterface()
    , bkgSolid(false) {}
TestcsVisOpenGL::~TestcsVisOpenGL() {

  std::cout << "~TestcsVisOpenGL" << std::endl;
}

void TestcsVisOpenGL::initialize(csVisOpenGL::ShaderFactory& shaderFactory,
                                 std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions) {

  {
    QImage img(2, 2, QImage::Format_RGB32);
    img.fill(Qt::red);

    auto tid = m_texturesManager.uploadTexture(img);
    singleTextureRenderer.initialize(shaderFactory, glExtraFunctions);
    csVisOpenGL::SingleTextureRenderer::Matrix3x4f vectCoords;
    vectCoords.col(0) = Eigen::Vector3f(4 + 0, 4 + 0, 0);
    vectCoords.col(1) = Eigen::Vector3f(4 + 2, 4 + 0, 1);
    vectCoords.col(2) = Eigen::Vector3f(4 + 3, 4 + 1, 2);
    vectCoords.col(3) = Eigen::Vector3f(4 + 0, 4 + 2, 3);
    csVisOpenGL::SingleTextureRenderer::Matrix2x4f texCoords;
    texCoords.col(0) = Eigen::Vector2f(0, 0);
    texCoords.col(1) = Eigen::Vector2f(1, 0);
    texCoords.col(2) = Eigen::Vector2f(1, 1);
    texCoords.col(3) = Eigen::Vector2f(0, 1);
    singleTextureRenderer.setTexture(tid, vectCoords, texCoords, csVisOpenGL::SingleTextureRenderer::TextureInterpolation::Linear);
  }

  bkgRenderer.initialize(shaderFactory, glExtraFunctions);

  bkgSolidRenderer.setColor(Eigen::Vector3f(123 / 255.0f, 72 / 255.0f, 48 / 255.0f));
  bkgSolidRenderer.initialize(shaderFactory, glExtraFunctions);
  uniformPointRenderer.initialize(shaderFactory, glExtraFunctions);
  perVertexPointRenderer.initialize(shaderFactory, glExtraFunctions);
  uniformLineRenderer.initialize(shaderFactory, glExtraFunctions);
  perVertexLineRenderer.initialize(shaderFactory, glExtraFunctions);

  {
    Eigen::Matrix3Xf points = Eigen::Matrix3Xf::Random(3, 512);
    points.colwise().normalize();
    uniformPointRenderer.setPoints(points);
    uniformPointRenderer.setPointSize(5);
    uniformPointRenderer.setSmoothPoints(true);
    uniformPointRenderer.setUniformColor(Eigen::Vector3f(Eigen::Vector3f::UnitX()));
  }

  {
    Eigen::Matrix3Xf pointsV = Eigen::Matrix3Xf::Random(3, 512) * 2;
    Eigen::Matrix3Xf colorsV = Eigen::Matrix3Xf::Random(3, 512) / 2;
    colorsV.colwise() += Eigen::Vector3f::Constant(0.5f);
    perVertexPointRenderer.setPoints(pointsV, colorsV);
    perVertexPointRenderer.setPointSize(2);
    perVertexPointRenderer.setSmoothPoints(true);
  }

  {
    Eigen::Matrix3Xf points = Eigen::Matrix3Xf::Random(3, 128);
    points.colwise().normalize();
    points /= 2;
    uniformLineRenderer.setLines(points);
    uniformLineRenderer.setLineWidth(4);
    Eigen::Vector4f c = Eigen::Vector4f::Random() / 2 + Eigen::Vector4f::Constant(0.5f);
    c.w() = 0.5f;
    uniformLineRenderer.setUniformColor(c);
  }

  {
    Eigen::Matrix3Xf pointsV = Eigen::Matrix3Xf::Random(3, 128);
    pointsV.colwise().normalize();
    pointsV *= 4;
    Eigen::Matrix3Xf colorsV = Eigen::Matrix3Xf::Random(3, 128) / 2;
    colorsV.colwise() += Eigen::Vector3f::Constant(0.5f);
    perVertexLineRenderer.setLines(pointsV, colorsV);
    perVertexLineRenderer.setLineWidth(1);
  }

  axes.initialize(shaderFactory, glExtraFunctions);
  axes.setPose(Eigen::Isometry3f::Identity(), 1.0f);
  grid.initialize(shaderFactory, glExtraFunctions);

  uniformMeshRenderer.initialize(shaderFactory, glExtraFunctions);
  {
    Eigen::Matrix3Xf triangles(3, 6);
    double d = 0.5;
    triangles.col(0) = Eigen::Vector3f(-d, -d, 0);
    triangles.col(1) = Eigen::Vector3f(+d, -d, 0);
    triangles.col(2) = Eigen::Vector3f(+d, +d, 0);
    triangles.col(3) = triangles.col(2);
    triangles.col(4) = Eigen::Vector3f(-d, +d, 0);
    triangles.col(5) = triangles.col(0);

    uniformMeshRenderer.setTriangles(triangles);
    auto c = csVisOpenGL::Color::cyan;
    c.w() = 0.5;
    uniformMeshRenderer.setUniformColor(c);
  }

  perVertexMeshRendered.initialize(shaderFactory, glExtraFunctions);
  {
    Eigen::Matrix3Xf triangles(3, 6);
    Eigen::Matrix3Xf colors(3, 6);
    double d = 0.5;
    double z = 1;
    triangles.col(0) = Eigen::Vector3f(-d, -d, z);
    triangles.col(1) = Eigen::Vector3f(+d, -d, z);
    triangles.col(2) = Eigen::Vector3f(+d, +d, z);
    triangles.col(3) = triangles.col(2);
    triangles.col(4) = Eigen::Vector3f(-d, +d, z);
    triangles.col(5) = triangles.col(0);

    colors.col(0) = csVisOpenGL::Color::red.head<3>();
    colors.col(1) = csVisOpenGL::Color::green.head<3>();
    colors.col(2) = csVisOpenGL::Color::blue.head<3>();
    colors.col(3) = colors.col(2);
    colors.col(4) = csVisOpenGL::Color::yellow.head<3>();
    colors.col(5) = colors.col(0);

    perVertexMeshRendered.setTriangles(triangles, colors);
  }

  //
  meshRenderer.initialize(shaderFactory, glExtraFunctions);
  {
    Eigen::Matrix3Xf triangles(3, 6);
    Eigen::Matrix3Xf normals(3, 6);
    double d = 0.5;
    double z = 2;
    triangles.col(0) = Eigen::Vector3f(-d, -d, z);
    triangles.col(1) = Eigen::Vector3f(+d, -d, z);
    triangles.col(2) = Eigen::Vector3f(+d, +d, z);

    normals.col(0) = Eigen::Vector3f::UnitZ();
    normals.col(1) = Eigen::Vector3f::UnitZ();
    normals.col(2) = Eigen::Vector3f::UnitZ();
    normals.col(3) = -Eigen::Vector3f::UnitZ();
    normals.col(4) = -Eigen::Vector3f::UnitZ();
    normals.col(5) = -Eigen::Vector3f::UnitZ();

    triangles.col(3) = triangles.col(2);
    triangles.col(4) = Eigen::Vector3f(-d, +d, z);
    triangles.col(5) = triangles.col(0);

    meshRenderer.setTriangles(triangles, normals);
    auto c = csVisOpenGL::Color::deepPurple;
    c.w() = 1.0f;
    meshRenderer.setCullFaceEnabled(false);
    meshRenderer.setUniformColor(c);
    meshRenderer.setMaterialShininess(10.0f);
    meshRenderer.setMaterialSpecularIntensity(0.4f);
  }

  //
  meshColorRenderer.initialize(shaderFactory, glExtraFunctions);
  {
    Eigen::Matrix3Xf triangles(3, 6);
    Eigen::Matrix3Xf normals(3, 6);
    Eigen::Matrix3Xf colors(3, 6);
    double d = 0.5;
    double z = -2;
    triangles.col(0) = Eigen::Vector3f(-d, -d, z);
    triangles.col(1) = Eigen::Vector3f(+d, -d, z);
    triangles.col(2) = Eigen::Vector3f(+d, +d, z);
    triangles.col(3) = triangles.col(2);
    triangles.col(4) = Eigen::Vector3f(-d, +d, z);
    triangles.col(5) = triangles.col(0);

    normals.col(0) = Eigen::Vector3f::UnitZ();
    normals.col(1) = Eigen::Vector3f::UnitZ();
    normals.col(2) = Eigen::Vector3f::UnitZ();
    normals.col(3) = Eigen::Vector3f::UnitZ();
    normals.col(4) = Eigen::Vector3f::UnitZ();
    normals.col(5) = Eigen::Vector3f::UnitZ();

    colors.col(0) = csVisOpenGL::Color::red.head<3>();
    colors.col(1) = csVisOpenGL::Color::green.head<3>();
    colors.col(2) = csVisOpenGL::Color::blue.head<3>();
    colors.col(3) = colors.col(2);
    colors.col(4) = csVisOpenGL::Color::yellow.head<3>();
    colors.col(5) = colors.col(0);

    meshColorRenderer.setTriangles(triangles, normals, colors);
    meshColorRenderer.setMaterialShininess(10.0f);
    meshColorRenderer.setMaterialSpecularIntensity(0.4f);
  }
  {
    unifEllipsRenderer3d.initialize(shaderFactory, glExtraFunctions);
    unifEllipsRenderer3d.setUniformColor(csVisOpenGL::Color::indigo);
    std::vector<Eigen::Isometry3f> poses;
    std::vector<Eigen::Matrix3d> covs;

    // radius 1 sphere
    poses.push_back(Eigen::Isometry3f::Identity());
    covs.push_back(Eigen::Matrix3d::Identity());
    // x=2, y=z=1
    poses.push_back(Eigen::Isometry3f::Identity());
    poses.back().translation() = Eigen::Vector3f(2, 2, 1);
    covs.push_back(Eigen::Vector3d(2, 1, 1).asDiagonal());

    // x=2, y=z=1, but 45deg rotated and in 5,5,2 position
    poses.push_back(Eigen::Isometry3f::Identity());
    poses.back().translation() = Eigen::Vector3f(5, 5, 2);
    poses.back().linear() = Eigen::AngleAxisf(float(M_PI / 4), Eigen::Vector3f::UnitZ()).toRotationMatrix();
    covs.push_back(Eigen::Vector3d(2, 1, 1).asDiagonal());

    unifEllipsRenderer3d.setCovariances(poses, covs, 1.0);
  }
  {
    unifEllipsRenderer2d.initialize(shaderFactory, glExtraFunctions);
    unifEllipsRenderer2d.setUniformColor(csVisOpenGL::Color::indigo);
    std::vector<Eigen::Isometry3f> poses;
    std::vector<Eigen::Matrix2d> covs;

    // radius 1 circle
    poses.push_back(Eigen::Isometry3f::Identity());
    poses.back().translation() = Eigen::Vector3f(-5, -5, 0);
    covs.push_back(Eigen::Matrix2d::Identity());
    // x=2, y=z=1
    poses.push_back(Eigen::Isometry3f::Identity());
    poses.back().translation() = Eigen::Vector3f(-5, -5, 2);
    covs.push_back(Eigen::Vector2d(2, 1).asDiagonal());
    // x=2, y=z=1, but 45deg rotated and in 5,5,2 position
    poses.push_back(Eigen::Isometry3f::Identity());
    poses.back().translation() = Eigen::Vector3f(-5, -5, 4);
    poses.back().linear() = Eigen::AngleAxisf(float(M_PI / 4), Eigen::Vector3f::UnitZ()).toRotationMatrix();
    covs.push_back(Eigen::Vector2d(2, 1).asDiagonal());

    unifEllipsRenderer2d.setCovariances(poses, covs, 1.0);
  }
}

void TestcsVisOpenGL::update(const csVisOpenGL::Camera& camera) {
  auto& light0 = meshRenderer.getLight(0);
  light0.enabled = true;
  light0.dir = (-(camera.up() / 4.0f) + (camera.left() / 4.0f) + camera.forward()).normalized();
  light0.color = Eigen::Vector3f::Ones() * 0.6f;
  light0.ambientCoefficient = 0.1f;
  meshRenderer.updateLight(0);

  auto& light1 = meshRenderer.getLight(1);
  light1.enabled = true;
  light1.dir = -(-(camera.up() / 4.0f) + (camera.left()) + camera.forward()).normalized();
  light1.color = Eigen::Vector3f::Ones() * 0.2f;
  light1.ambientCoefficient = 0;
  light1.specularEnabled = false;
  meshRenderer.updateLight(1);
}

void TestcsVisOpenGL::paintBackground(const csVisOpenGL::Camera& camera) {
  if (!bkgSolid)
    bkgRenderer.draw(camera);
  else
    bkgSolidRenderer.draw(camera);
}

void TestcsVisOpenGL::paint(const csVisOpenGL::Camera& camera) {

  uniformPointRenderer.draw(camera);
  perVertexPointRenderer.draw(camera);
  perVertexLineRenderer.draw(camera);
  axes.draw(camera);
  grid.draw(camera);
  unifEllipsRenderer3d.draw(camera);
  unifEllipsRenderer2d.draw(camera);

  meshRenderer.draw(camera);
}

void TestcsVisOpenGL::paintTransparent(const csVisOpenGL::Camera& camera) {
  singleTextureRenderer.draw(camera);

  uniformLineRenderer.draw(camera);

  uniformMeshRenderer.draw(camera);
  perVertexMeshRendered.draw(camera);
  meshColorRenderer.draw(camera);
}

void TestcsVisOpenGL::paintQt(const csVisOpenGL::Camera& camera, QPainter& painter) {
  Eigen::Vector2f origin2d = camera.worldToScreen(Eigen::Vector3f::Zero(), true);

  painter.setPen(QPen(QBrush(Qt::yellow), 2));
  painter.setBrush(QBrush(QColor::fromRgbF(1, 1, 1, 0.5f)));
  painter.drawEllipse(QPoint(origin2d.x(), origin2d.y()), 5, 5);

  painter.setPen(Qt::white);
  painter.drawText(QPoint(origin2d.x() + 10, origin2d.y() + 4), "This is the origin");
}
