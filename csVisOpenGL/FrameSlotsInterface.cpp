#include <csVisOpenGL/FrameSignals.hpp>
#include <csVisOpenGL/FrameSlotsInterface.hpp>

#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

namespace csVisOpenGL {

void FrameSlotsInterface::connect(FrameSignals const& frameSignals, FrameSlotsInterface const& frameSlots) {
  QObject::connect(&frameSignals, &FrameSignals::focusOutEvent, &frameSlots, &FrameSlotsInterface::onFocusOutEvent);
  QObject::connect(&frameSignals, &FrameSignals::keyPressEvent, &frameSlots, &FrameSlotsInterface::onKeyPressEvent);
  QObject::connect(&frameSignals, &FrameSignals::keyReleaseEvent, &frameSlots, &FrameSlotsInterface::onKeyReleaseEvent);
  QObject::connect(&frameSignals, &FrameSignals::mouseMoveEvent, &frameSlots, &FrameSlotsInterface::onMouseMoveEvent);
  QObject::connect(&frameSignals, &FrameSignals::mousePressEvent, &frameSlots, &FrameSlotsInterface::onMousePressEvent);
  QObject::connect(&frameSignals, &FrameSignals::mouseReleaseEvent, &frameSlots, &FrameSlotsInterface::onMouseReleaseEvent);
  QObject::connect(&frameSignals, &FrameSignals::mouseDoubleClick, &frameSlots, &FrameSlotsInterface::onMouseDoubleClick);
  QObject::connect(&frameSignals, &FrameSignals::wheelEvent, &frameSlots, &FrameSlotsInterface::onWheelEvent);
}

} // namespace csVisOpenGL
