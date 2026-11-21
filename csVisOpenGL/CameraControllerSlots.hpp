#pragma once

#include <QObject>

namespace csVisOpenGL {

class Camera;

class CameraControllerSignals;

class CameraControllerSlots : public QObject {
  Q_OBJECT
public slots:
  virtual void onNewCamera(Camera const &camera) = 0;

public:
  static void connect(CameraControllerSignals const &cameraControllerSignals,
                      CameraControllerSlots const &cameraControllerSlots);
};

} // namespace csVisOpenGL
