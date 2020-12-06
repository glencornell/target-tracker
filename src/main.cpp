#include <QCoreApplication>
#include <QTimer>
#include "GeoObserver.hpp"

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  GeoObserver observer;

  // quit application when work is complete:
  QObject::connect(&observer, SIGNAL(finished()), &a, SLOT(quit()));

  // Run the observer's user hook in the main loop:
  QTimer::singleShot(0, &observer, SLOT(main()));

  // Run the main loop:
  return a.exec();
}
