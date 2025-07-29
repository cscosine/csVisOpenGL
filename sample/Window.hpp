#pragma once
#include "csVisOpenGL/OrbitCameraController.hpp"
#include <QMainWindow>
#include <memory>
#include <sample/TestVis.hpp>

namespace Ui {
class Window;
}

class Window : public QMainWindow {
  Q_OBJECT;

  std::unique_ptr<Ui::Window> ui;
  std::shared_ptr<TestcsVisOpenGL> vis;
  std::shared_ptr<csVisOpenGL::OrbitCameraController> m_cameraController;

  Eigen::Vector3f pivotPos;
  Eigen::Vector3f ypr;
  float radius;

public:
  Window(QWidget* parent = 0);
  virtual ~Window();

public slots:
  void on_radioButtonBkgSolid_clicked();
  void on_radioButtonBkgGradient_clicked();
  void on_checkBoxOrthoCamera_clicked();
};
