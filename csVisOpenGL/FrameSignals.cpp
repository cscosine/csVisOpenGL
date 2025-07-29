#include <csVisOpenGL/FrameSignals.hpp>

#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

namespace csVisOpenGL {

void FrameSignals::connect(FrameSignals const& frameSignals, FrameSignals const& frameSignalsForward) {
  QObject::connect(&frameSignals, &FrameSignals::focusOutEvent, &frameSignalsForward, &FrameSignals::focusOutEvent);
  QObject::connect(&frameSignals, &FrameSignals::keyPressEvent, &frameSignalsForward, &FrameSignals::keyPressEvent);
  QObject::connect(&frameSignals, &FrameSignals::keyReleaseEvent, &frameSignalsForward, &FrameSignals::keyReleaseEvent);
  QObject::connect(&frameSignals, &FrameSignals::mouseMoveEvent, &frameSignalsForward, &FrameSignals::mouseMoveEvent);
  QObject::connect(&frameSignals, &FrameSignals::mousePressEvent, &frameSignalsForward, &FrameSignals::mousePressEvent);
  QObject::connect(&frameSignals, &FrameSignals::mouseReleaseEvent, &frameSignalsForward, &FrameSignals::mouseReleaseEvent);
  QObject::connect(&frameSignals, &FrameSignals::mouseDoubleClick, &frameSignalsForward, &FrameSignals::mouseDoubleClick);
  QObject::connect(&frameSignals, &FrameSignals::wheelEvent, &frameSignalsForward, &FrameSignals::wheelEvent);
}

} // namespace csVisOpenGL
