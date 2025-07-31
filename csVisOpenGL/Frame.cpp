#include "csVisOpenGL/CameraControllerSlots.hpp"
#include <QPainter>
#include <QtGui/QOpenGLFunctions>
#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Frame.hpp>
#include <csVisOpenGL/FrameSlotsInterface.hpp>
#include <csVisOpenGL/VisualizerInterface.hpp>

#include <QFocusEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <csVisOpenGL/ShaderFactory.hpp>

namespace csVisOpenGL {

void Frame::CameraState::onNewCamera(Camera const& camera) {
  m_camera = camera;
}

// --------------------------------------------------------------------------------------------------------------

Frame::Frame(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_openGLExtraFunctions{std::make_unique<QOpenGLExtraFunctions>()}
    , m_shaderFactory{std::make_unique<ShaderFactory>()}
    , m_visualizers{}
    , m_initialized(false)
    , m_cameraState() {
  setMinimumSize(300, 200);
  setMouseTracking(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setFocusPolicy(Qt::StrongFocus);
}

Frame::~Frame() {
  // make sure is current frame, to allow visualizer deletion
  this->makeCurrent();
}

// -------------------------------------------------- Visualizers -------------------------------------------------
void Frame::connectSlotsInterface(FrameSlotsInterface const& frameSlots) const {
  FrameSlotsInterface::connect(m_frameSignals, frameSlots);
}

void Frame::connectCameraControllerSignals(CameraControllerSignals const& cameraSignals) const {
  CameraControllerSlots::connect(cameraSignals, m_cameraState);
}

void Frame::addVisualizer(std::shared_ptr<VisualizerInterface> const& visualizer) {
  m_visualizers.emplace_back(visualizer);
  if (m_initialized) {
    this->makeCurrent();
    visualizer->initialize(*m_shaderFactory, m_openGLExtraFunctions);
  }
}

// -------------------------------------------------- GL Methdos --------------------------------------------------

void Frame::initializeGL() {
  // Initialize OpenGL
  makeCurrent();
  m_openGLExtraFunctions->initializeOpenGLFunctions();

  m_openGLExtraFunctions->glEnable(GL_DEPTH_TEST);
  m_openGLExtraFunctions->glEnable(GL_SCISSOR_TEST);
  // glShadeModel(GL_SMOOTH);
  m_openGLExtraFunctions->glClearColor(0.0, 0.0, 0.0, 0.0);
  m_openGLExtraFunctions->glClearDepthf(0.0);

  // Initialize visualizers already added
  for (int i = 0; i < m_visualizers.size(); i++)
    m_visualizers[i]->initialize(*m_shaderFactory, m_openGLExtraFunctions);

  // Initialize timers
  this->startTimer(40);

  emit m_frameSignals.initializeGL();

  // Done
  m_initialized = true;
}

void Frame::timerEvent(QTimerEvent* event) {
  this->update();
}

void Frame::paintGL() {
  if (!m_initialized)
    return;

  // Create a Qt Painter
  QPainter painter;
  if (!painter.begin(this))
    return;
  painter.setRenderHint(QPainter::Antialiasing);
  painter.beginNativePainting();

  // here give the possibility of updating the camera
  emit m_frameSignals.paintGL();

  // Update visualizers
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->update(m_cameraState.getCamera());
  }

  // Draw
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDepthMask(true);

  // Background
  glDisable(GL_DEPTH_TEST);
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paintBackground(m_cameraState.getCamera());
  }

  // Solid
  glEnable(GL_DEPTH_TEST);
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paint(m_cameraState.getCamera());
  }

  // Transparent
  glDepthMask(false);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paintTransparent(m_cameraState.getCamera());
  }
  glDepthMask(true);
  glDisable(GL_BLEND);

  // Qt painer
  glDisable(GL_DEPTH_TEST);
  painter.endNativePainting();
  painter.save();
  painter.setClipping(true);
  painter.setClipRect(QRect(0, 0, m_cameraState.getCamera().getViewSize().x(), m_cameraState.getCamera().getViewSize().y()));
  painter.translate(QPoint(0, 0));

  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paintQt(m_cameraState.getCamera(), painter);
  }

  painter.restore();
  painter.setClipping(false);
  painter.beginNativePainting();
  glEnable(GL_DEPTH_TEST);
  painter.endNativePainting();

  // after
  // glFlush();
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->afterPaint(m_cameraState.getCamera());
  }
}

void Frame::resizeGL(int width, int height) {
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  glViewport(0, 0, width, height);
  emit m_frameSignals.resizeGL(width, height);
}

// ---------------------------------------------------- Events ----------------------------------------------------

bool Frame::event(QEvent* event) {
  if (event->type() == QEvent::MouseButtonDblClick) {
    emit m_frameSignals.mouseDoubleClick(static_cast<QMouseEvent&>(*event));
    return true;
  }
  return QWidget::event(event);
}

void Frame::focusOutEvent(QFocusEvent* event) {
  // TODO(sc) add asserts on event != nullptr
  emit m_frameSignals.focusOutEvent(*event);
}

void Frame::keyPressEvent(QKeyEvent* event) {
  emit m_frameSignals.keyPressEvent(*event);
}

void Frame::keyReleaseEvent(QKeyEvent* event) {
  emit m_frameSignals.keyReleaseEvent(*event);
}

void Frame::mouseMoveEvent(QMouseEvent* event) {
  emit m_frameSignals.mouseMoveEvent(*event);
}

void Frame::mousePressEvent(QMouseEvent* event) {
  emit m_frameSignals.mousePressEvent(*event);
}

void Frame::mouseReleaseEvent(QMouseEvent* event) {
  emit m_frameSignals.mouseReleaseEvent(*event);
}

void Frame::wheelEvent(QWheelEvent* event) {
  emit m_frameSignals.wheelEvent(*event);
}

} // namespace csVisOpenGL
