#pragma once

#include <QSet>

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace csVisOpenGL {

class Camera;

class CameraController {
protected:
  Camera& camera;
  QSet<int> keysPressed;

public:
  CameraController(Camera& camera);
  virtual ~CameraController();

  virtual void focusOutEvent(QFocusEvent* event) {}
  virtual void keyPressEvent(QKeyEvent* e);
  virtual void keyReleaseEvent(QKeyEvent* e);
  virtual void mouseMoveEvent(QMouseEvent* e) {}
  virtual void mousePressEvent(QMouseEvent* e) {}
  virtual void mouseReleaseEvent(QMouseEvent* e) {}
  virtual void mouseDoubleClick(QMouseEvent* e) {}
  virtual void wheelEvent(QWheelEvent* e) {}

  virtual void update(float dT) = 0;
};

} // namespace csVisOpenGL
