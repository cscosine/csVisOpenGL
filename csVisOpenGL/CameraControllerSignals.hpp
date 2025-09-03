#pragma once

#include <QObject>

namespace csVisOpenGL {

class Camera;

class CameraControllerSignals : public QObject {
  Q_OBJECT

public:
signals:
  void newCamera(Camera const& camera);

public:
  static void connect(CameraControllerSignals const& cameraControllerSignals,
                      CameraControllerSignals const& cameraControllerSignalsForward);
};

} // namespace csVisOpenGL
