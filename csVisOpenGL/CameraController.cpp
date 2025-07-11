#include "CameraController.h"
#include <QKeyEvent>

namespace csVisOpenGL {

CameraController::CameraController(Camera& camera)
    : camera(camera) {}
CameraController::~CameraController() {}

void CameraController::keyPressEvent(QKeyEvent* e) {
  keysPressed += e->key();
}

void CameraController::keyReleaseEvent(QKeyEvent* e) {
  keysPressed -= e->key();
}

} // namespace csVisOpenGL
