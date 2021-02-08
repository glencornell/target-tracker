#include <QCoreApplication>
#include <QTimer>
#include "BlosApp.hpp"

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  BlosApp blos_app(&a, argc, argv);

  // quit application when work is complete:
  QObject::connect(&blos_app, &BlosApp::finished, &a, &QCoreApplication::quit);

  // Run the observer's user hook in the main loop:
  QTimer::singleShot(0, &blos_app, &BlosApp::main);

  // Run the main loop:
  return a.exec();
}
