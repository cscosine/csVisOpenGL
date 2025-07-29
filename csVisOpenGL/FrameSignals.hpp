#pragma once

#include <QObject>

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
namespace csVisOpenGL {

class FrameSignals : public QObject {
  Q_OBJECT;

public:
signals:
  void focusOutEvent(QFocusEvent& event);
  void keyPressEvent(QKeyEvent& event);
  void keyReleaseEvent(QKeyEvent& event);
  void mouseMoveEvent(QMouseEvent& event);
  void mousePressEvent(QMouseEvent& event);
  void mouseReleaseEvent(QMouseEvent& event);
  void mouseDoubleClick(QMouseEvent& event);
  void wheelEvent(QWheelEvent& event);

public:
  static void connect(FrameSignals const& frameSignals, FrameSignals const& frameSignalsForward);
};

} // namespace csVisOpenGL
