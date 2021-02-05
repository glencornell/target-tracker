#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QGeoCoordinate>

class LACApp : public QObject
{
  Q_OBJECT

public:
  LACApp(QCoreApplication *app, int argc, char *argv[]);

public slots:
  void main();

signals:
  void finished();

private:
  void help();

  QCoreApplication *m_app;
  QGeoCoordinate m_observer;
  QGeoCoordinate m_target;
};

