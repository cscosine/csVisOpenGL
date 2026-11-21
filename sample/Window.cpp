#include <memory>
#include <sample/Window.hpp>

#include <sample/ui_Window.h>

#include <csVisOpenGL/Camera.hpp>

#include <QFileDialog>
#include <atomic>
#include <iostream>

Window::Window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Window()),
      m_cameraController(std::make_shared<csVisOpenGL::OrbitCameraController>())

{
  m_cameraController->setRadius(10);
  m_cameraController->setTraslationLimits(-Eigen::Vector3f::Ones(), Eigen::Vector3f::Ones());

  ui->setupUi(this);

  ui->preview->connectSlotsInterface(*m_cameraController);
  ui->preview->connectCameraControllerSignals(m_cameraController->getCameraControllerSignals());

  vis = std::make_shared<TestcsVisOpenGL>();
  vis->bkgSolid = ui->radioButtonBkgSolid->isChecked();
  ui->preview->addVisualizer(vis);

  // later
  this->show();
}

Window::~Window() {}

void Window::on_checkBoxOrthoCamera_clicked() {
  if (ui->checkBoxOrthoCamera->isChecked()) {
    m_cameraController->resetDestinations();
    m_cameraController->setCameraOrthogonal();
  } else {
    m_cameraController->setCameraPerspective(45.0);
    m_cameraController->resetDestinations();
  }
}

void Window::on_radioButtonBkgSolid_clicked() { vis->bkgSolid = ui->radioButtonBkgSolid->isChecked(); }

void Window::on_radioButtonBkgGradient_clicked() { vis->bkgSolid = ui->radioButtonBkgSolid->isChecked(); }
