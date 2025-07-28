#include <QPainter>
#include <QtGui/QOpenGLFunctions>
#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Frame.hpp>
#include <csVisOpenGL/OrbitCameraController.hpp>
#include <csVisOpenGL/VisualizerInterface.hpp>

#include <QFocusEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <csVisOpenGL/ShaderFactory.hpp>

namespace csVisOpenGL {

Frame::Frame(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_openGLExtraFunctions{std::make_unique<QOpenGLExtraFunctions>()}
    , m_camera(std::make_unique<Camera>())
    , m_cameraController(std::make_unique<OrbitCameraController>(*m_camera))
    , m_shaderFactory{std::make_unique<ShaderFactory>()}
    , m_visualizers{}
    , m_initialized(false)
    , m_lastRenderTime{std::chrono::steady_clock::now()} // unsorted
{
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
  m_lastRenderTime = std::chrono::steady_clock::now();

  // Done
  m_initialized = true;
}

void Frame::timerEvent(QTimerEvent* event) {
  this->update();
}

void Frame::paintGL() {
  if (!m_initialized)
    return;

  // Get time since last paint
  auto t_now = std::chrono::steady_clock::now();
  float deltaTime = std::chrono::duration<float>(t_now - m_lastRenderTime).count();
  m_lastRenderTime = t_now;

  // Create a Qt Painter
  QPainter painter;
  if (!painter.begin(this))
    return;
  painter.setRenderHint(QPainter::Antialiasing);
  painter.beginNativePainting();

  // Update camera
  m_cameraController->update(deltaTime);

  // Update visualizers
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->update(*m_camera);
  }

  // Draw
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDepthMask(true);

  // Background
  glDisable(GL_DEPTH_TEST);
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paintBackground(*m_camera);
  }

  // Solid
  glEnable(GL_DEPTH_TEST);
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paint(*m_camera);
  }

  // Transparent
  glDepthMask(false);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paintTransparent(*m_camera);
  }
  glDepthMask(true);
  glDisable(GL_BLEND);

  // Qt painer
  glDisable(GL_DEPTH_TEST);
  painter.endNativePainting();
  painter.save();
  painter.setClipping(true);
  painter.setClipRect(QRect(0, 0, m_camera->getViewSize().x(), m_camera->getViewSize().y()));
  painter.translate(QPoint(0, 0));

  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->paintQt(*m_camera, painter);
  }

  painter.restore();
  painter.setClipping(false);
  painter.beginNativePainting();
  glEnable(GL_DEPTH_TEST);
  painter.endNativePainting();

  // after
  // glFlush();
  for (int i = 0; i < m_visualizers.size(); i++) {
    m_visualizers[i]->afterPaint(*m_camera);
  }
}

void Frame::resizeGL(int width, int height) {
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  glViewport(0, 0, width, height);
  m_camera->setViewSize(Eigen::Vector2f(width, height));
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
  m_cameraController->focusOutEvent(event);
}

void Frame::keyPressEvent(QKeyEvent* e) {
  m_cameraController->keyPressEvent(e);
}

void Frame::keyReleaseEvent(QKeyEvent* e) {
  m_cameraController->keyReleaseEvent(e);
}

void Frame::mouseMoveEvent(QMouseEvent* e) {
  m_cameraController->mouseMoveEvent(e);
}

void Frame::mousePressEvent(QMouseEvent* e) {
  m_cameraController->mousePressEvent(e);
}

void Frame::mouseReleaseEvent(QMouseEvent* e) {
  m_cameraController->mouseReleaseEvent(e);
}

void Frame::mouseDoubleClick(QMouseEvent* e) {
  m_cameraController->mouseDoubleClick(e);
}

void Frame::wheelEvent(QWheelEvent* e) {
  m_cameraController->wheelEvent(e);
}

} // namespace csVisOpenGL
