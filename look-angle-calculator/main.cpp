#include <QCoreApplication>
#include <QTimer>
#include "LACApp.hpp"

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  LACApp lac_app(&a, argc, argv);
  
  // quit application when work is complete:
  QObject::connect(&lac_app, &LACApp::finished, &a, &QCoreApplication::quit);

  // Run the observer's user hook in the main loop:
  QTimer::singleShot(0, &lac_app, &LACApp::main);

  // Run the main loop:
  return a.exec();
}
