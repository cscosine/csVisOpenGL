#pragma once

#include <QObject>

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace csVisOpenGL {

class FrameSignals;

class FrameSlotsInterface : public QObject {
  Q_OBJECT;

public slots:
  virtual void onFocusOutEvent(QFocusEvent& eventevent) = 0;
  virtual void onKeyPressEvent(QKeyEvent& eventevent) = 0;
  virtual void onKeyReleaseEvent(QKeyEvent& eventevent) = 0;
  virtual void onMouseMoveEvent(QMouseEvent& eventevent) = 0;
  virtual void onMousePressEvent(QMouseEvent& eventevent) = 0;
  virtual void onMouseReleaseEvent(QMouseEvent& eventevent) = 0;
  virtual void onMouseDoubleClick(QMouseEvent& eventevent) = 0;
  virtual void onWheelEvent(QWheelEvent& eventevent) = 0;

  virtual void onResizeGL(int width, int height) = 0;
  virtual void onPaintGL() = 0;
  virtual void onInitializeGL() = 0;

public:
  static void connect(FrameSignals const& frameSignals, FrameSlotsInterface const& frameSlots);
};
} // namespace csVisOpenGL
