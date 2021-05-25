/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// modified from https://blackberry.github.io/Cascades-Samples/imagerotation-src-rotationsensor-cpp.html
#pragma once

#include <QObject>
#include <QRotationFilter>
#include <QRotationSensor>
#include <QRotationReading>

// The RotatioReadingSource class uses the QRotationSensor class from the
// QtSensors module to retrieve the current x, y and z values from the
// rotation sensor of the device and provides it as a property.
class RotationReadingSource : public QObject, public QRotationFilter
{
  Q_OBJECT
  Q_PROPERTY(QRotationReading *rotation READ rotation NOTIFY rotationChanged)

public:
  RotationReadingSource(QObject *parent = nullptr);

  QRotationReading *rotation() const;

signals:
  void rotationChanged(QRotationReading *);

public slots:
  // Start receiving values from the sensors. You will not get
  // oriententation updates until this method is invoked.  This method
  // implements the delegate of responsibility pattern to call the
  // rotation source start() method.
  void startUpdates();
  
protected:
  // This method is reimplemented from the QRotationFilter interface
  // and is called by the QRotationSensor whenever new values are
  // available.
  bool filter(QRotationReading *reading);
  
private:
  // The rotation sensor
  QRotationSensor m_rotationSensor;
  
  // The rotation property
  QRotationReading *m_rotation;
};
