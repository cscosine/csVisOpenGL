#include <QApplication>
#include <sample/Window.hpp>

int main(int argc, char* argv[]) {
  Q_INIT_RESOURCE(vis);

  QApplication app(argc, argv);

  Window window(nullptr);

  app.exec();

  return 0;
}
