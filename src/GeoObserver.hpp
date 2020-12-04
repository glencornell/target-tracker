#include <QGeoCoordinate>
#include <QTextStream>

class GeoObserver : public QObject {
  Q_OBJECT

public:
  GeoObserver(QObject * parent = nullptr);
  void setObserver(QGeoCoordinate &observer);
  void setObserved(QGeoCoordinate &observed);

private slots:
  void getLookAngle();

signals:
  void finished();

private:
  QGeoCoordinate m_observer;
  QGeoCoordinate m_observed;
};

