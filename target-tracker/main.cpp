#include <QCoreApplication>
#include <QTimer>
#include "TargetTrackerApp.hpp"

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  TargetTrackerApp app(&a, argc, argv);

  // quit application when work is complete:
  QObject::connect(&app, &TargetTrackerApp::finished, &a, &QCoreApplication::quit);

  // Run the observer's user hook in the main loop:
  QTimer::singleShot(0, &app, &TargetTrackerApp::main);

  // Run the main loop:
  return a.exec();
}
