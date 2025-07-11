#include "Frame.h"
#include "Camera.h"
#include "OrbitCameraController.h"
#include "Visualizer.h"
#include <QPainter>
#include <QtGui/QOpenGLFunctions>

#include <QFocusEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "ShaderFactory.h"

namespace csVisOpenGL {

Frame::Frame(QWidget* parent)
    : QOpenGLWidget(parent)
    , initialized(false)
    , enabled(false)
    , camera(new Camera())
    , cameraController(new OrbitCameraController(*camera)) {
  setMinimumSize(300, 200);
  setMouseTracking(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setFocusPolicy(Qt::StrongFocus);
}

Frame::~Frame() {}

// -------------------------------------------------- Visualizers -------------------------------------------------

void Frame::addVisualizer(Visualizer* visualizer) {
  if (!visualizer)
    return;
  visualizers.push_back(visualizer);
  if (initialized) {
    this->makeCurrent();
    visualizer->initialize(shaderFactory.get());
  }
}

bool Frame::removeVisualizer(Visualizer* visualizer) {
  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i] == visualizer) {
      visualizers.erase(visualizers.begin() + i);
      return true;
    }
  }
  return false;
}

// -------------------------------------------------- GL Methdos --------------------------------------------------

void Frame::initializeGL() {
  // Initialize OpenGL
  makeCurrent();
  initializeOpenGLFunctions();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  // glShadeModel(GL_SMOOTH);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepthf(0.0);

  // Create shader factory
  shaderFactory.reset(new ShaderFactory(this));

  // Initialize visualizers already added
  for (int i = 0; i < visualizers.size(); i++)
    visualizers[i]->initialize(shaderFactory.get());

  // Initialize timers
  this->startTimer(40);
  this->lastRenderTime = std::chrono::steady_clock::now();

  // Done
  initialized = true;
  enabled = true;
}

void Frame::timerEvent(QTimerEvent* event) {
  this->update();
}

void Frame::paintGL() {
  if (!enabled || !initialized)
    return;

  // Get time since last paint
  auto t_now = std::chrono::steady_clock::now();
  float deltaTime = std::chrono::duration<float>(t_now - this->lastRenderTime).count();
  this->lastRenderTime = t_now;

  // Create a Qt Painter
  QPainter painter;
  if (!painter.begin(this))
    return;
  painter.setRenderHint(QPainter::Antialiasing);
  painter.beginNativePainting();

  // Update camera
  if (cameraController)
    cameraController->update(deltaTime);

  // Update visualizers
  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i]->isEnabled())
      visualizers[i]->update(*camera);
  }

  // Draw
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDepthMask(true);

  // Background
  glDisable(GL_DEPTH_TEST);
  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i]->isEnabled())
      visualizers[i]->paintBackground(*camera);
  }

  // Solid
  glEnable(GL_DEPTH_TEST);
  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i]->isEnabled())
      visualizers[i]->paint(*camera);
  }

  // Transparent
  glDepthMask(false);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i]->isEnabled())
      visualizers[i]->paintTransparent(*camera);
  }
  glDepthMask(true);
  glDisable(GL_BLEND);

  // Qt painer
  glDisable(GL_DEPTH_TEST);
  painter.endNativePainting();
  painter.save();
  painter.setClipping(true);
  painter.setClipRect(QRect(0, 0, camera->getViewSize().x(), camera->getViewSize().y()));
  painter.translate(QPoint(0, 0));

  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i]->isEnabled())
      visualizers[i]->paintQt(*camera, painter);
  }

  painter.restore();
  painter.setClipping(false);
  painter.beginNativePainting();
  glEnable(GL_DEPTH_TEST);
  painter.endNativePainting();

  // after
  // glFlush();
  for (int i = 0; i < visualizers.size(); i++) {
    if (visualizers[i]->isEnabled())
      visualizers[i]->afterPaint(*camera);
  }
}

void Frame::resizeGL(int width, int height) {
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  glViewport(0, 0, width, height);
  camera->setViewSize(Eigen::Vector2f(width, height));
}

// ---------------------------------------------------- Events ----------------------------------------------------

bool Frame::event(QEvent* event) {
  if (event->type() == QEvent::MouseButtonDblClick) {
    mouseDoubleClick(static_cast<QMouseEvent*>(event));
    return true;
  }
  return QWidget::event(event);
}

void Frame::focusOutEvent(QFocusEvent* event) {
  if (!cameraController)
    return;
  cameraController->focusOutEvent(event);
}

void Frame::keyPressEvent(QKeyEvent* e) {
  if (!cameraController)
    return;
  cameraController->keyPressEvent(e);
}

void Frame::keyReleaseEvent(QKeyEvent* e) {
  if (!cameraController)
    return;
  cameraController->keyReleaseEvent(e);
}

void Frame::mouseMoveEvent(QMouseEvent* e) {
  if (!cameraController)
    return;
  cameraController->mouseMoveEvent(e);
}

void Frame::mousePressEvent(QMouseEvent* e) {
  if (!cameraController)
    return;
  cameraController->mousePressEvent(e);
}

void Frame::mouseReleaseEvent(QMouseEvent* e) {
  if (!cameraController)
    return;
  cameraController->mouseReleaseEvent(e);
}

void Frame::mouseDoubleClick(QMouseEvent* e) {
  if (!cameraController)
    return;
  cameraController->mouseDoubleClick(e);
}

void Frame::wheelEvent(QWheelEvent* e) {
  if (!cameraController)
    return;
  cameraController->wheelEvent(e);
}
} // namespace csVisOpenGL
