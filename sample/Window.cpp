#include <sample/Window.hpp>

#include <sample/ui_Window.h>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/OrbitCameraController.hpp>

#include <QFileDialog>
#include <atomic>
#include <iostream>

Window::Window(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Window())

{
  ui->setupUi(this);
  this->show();

  vis.reset(new TestcsVisOpenGL());
  ui->preview->addVisualizer(vis.get());
  static_cast<csVisOpenGL::OrbitCameraController*>(ui->preview->getCameraController())->setRadius(10);
  static_cast<csVisOpenGL::OrbitCameraController*>(ui->preview->getCameraController())
      ->setTraslationLimits(-Eigen::Vector3f::Ones(), Eigen::Vector3f::Ones());

  vis->bkgSolid = ui->radioButtonBkgSolid->isChecked();
}

Window::~Window() {}

void Window::on_checkBoxOrthoCamera_clicked() {
  if (ui->checkBoxOrthoCamera->isChecked()) {
    static_cast<csVisOpenGL::OrbitCameraController*>(ui->preview->getCameraController())->resetDestinations();
    static_cast<csVisOpenGL::OrbitCameraController*>(ui->preview->getCameraController())->setCameraOrthogonal();
  } else {
    static_cast<csVisOpenGL::OrbitCameraController*>(ui->preview->getCameraController())->setCameraPerspective(45.0);
    static_cast<csVisOpenGL::OrbitCameraController*>(ui->preview->getCameraController())->resetDestinations();
  }
}

void Window::on_radioButtonBkgSolid_clicked() {
  vis->bkgSolid = ui->radioButtonBkgSolid->isChecked();
}

void Window::on_radioButtonBkgGradient_clicked() {
  vis->bkgSolid = ui->radioButtonBkgSolid->isChecked();
}
