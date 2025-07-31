#include <csVisOpenGL/CameraControllerSignals.hpp>
#include <qobject.h>

namespace csVisOpenGL {

void CameraControllerSignals::connect(CameraControllerSignals const& cameraControllerSignals,
                                      CameraControllerSignals const& cameraControllerSignalsForward) {
  QObject::connect(&cameraControllerSignals, &CameraControllerSignals::newCamera, &cameraControllerSignalsForward,
                   &CameraControllerSignals::newCamera);
}

} // namespace csVisOpenGL
