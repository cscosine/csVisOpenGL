#include <QApplication>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QTimer>
#include <sample/Window.hpp>

#include <cstdlib>
#include <iostream>

// --smoke: start, then after a while check that every OpenGL view got its
// context and end (exit code 0), else exit code 1; for the automatic runs of
// the deployed sample (CI, a machine without the build). The process ends
// right after the check (std::_Exit, the output flushed): no teardown, which
// the software OpenGL of Mesa on Windows can hang at the unload of its
// threads, and nothing left to check.
static void checkAndQuit(Window const& window) {
  auto const views = window.findChildren<QOpenGLWidget*>();
  if (views.isEmpty()) {
    std::cerr << "smoke: failed, no OpenGL view" << std::endl;
    std::_Exit(1);
  }
  for (QOpenGLWidget const* view : views) {
    if (!view->isValid() || view->context() == nullptr) {
      std::cerr << "smoke: failed, no OpenGL context for " << view->objectName().toStdString() << std::endl;
      std::_Exit(1);
    }
  }
  QSurfaceFormat const format = views.front()->format();
  std::cout << "smoke: ok, platform " << QApplication::platformName().toStdString() << ", " << views.size()
            << " OpenGL view(s), OpenGL " << format.majorVersion() << "." << format.minorVersion() << std::endl;
  std::_Exit(0);
}

int main(int argc, char* argv[]) {
  Q_INIT_RESOURCE(vis);

  QApplication app(argc, argv);

  Window window(nullptr);
  if (QApplication::arguments().contains("--smoke")) {
    QTimer::singleShot(2000, &app, [&window] { checkAndQuit(window); });
  }

  return app.exec();
}
