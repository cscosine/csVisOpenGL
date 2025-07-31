#include <csVisOpenGL/CameraControllerSignals.hpp>
#include <csVisOpenGL/CameraControllerSlots.hpp>

namespace csVisOpenGL {

void CameraControllerSlots::connect(CameraControllerSignals const& cameraControllerSignals,
                                    CameraControllerSlots const& cameraControllerSlots) {
  QObject::connect(&cameraControllerSignals, &CameraControllerSignals::newCamera, &cameraControllerSlots,
                   &CameraControllerSlots::onNewCamera);
}

} // namespace csVisOpenGL
