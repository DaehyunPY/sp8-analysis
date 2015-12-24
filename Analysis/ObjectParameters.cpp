#include "ObjectParameters.h"

Analysis::ObjectParameters::ObjectParameters(const double &theta,
                                             const double &dx,
                                             const double &dy,
                                             const double &t1,
                                             const double &x0,
                                             const double &y0,
                                             const double &t0)
    : angleOfDetector(theta),
      pixelSizeOfX(dx),
      pixelSizeOfY(dy),
      deadTime(t1),
      xZeroOfCOM(x0),
      yZeroOfCOM(y0),
      timeZeroOfTOF(t0) {
  return;
}
Analysis::ObjectParameters::ObjectParameters(const Analysis::Unit &unit,
                                             const double &theta,
                                             const double &dx,
                                             const double &dy,
                                             const double &t1,
                                             const double &x0,
                                             const double &y0,
                                             const double &t0)
    : ObjectParameters(unit.readDegree(theta),
                       dx,
                       dy,
                       unit.readTime(t1),
                       unit.readLength(x0),
                       unit.readLength(y0),
                       unit.readTime(t0)) {
  return;
}
Analysis::ObjectParameters::ObjectParameters(const Unit &unit,
                                             const JSONReader &reader,
                                             const std::string &prefix)
    : ObjectParameters(unit,
                       reader.getDoubleAt(prefix + "angle_of_detector"),
                       reader.getDoubleAt(prefix + "pixel_size_of_x"),
                       reader.getDoubleAt(prefix + "pixel_size_of_y"),
                       reader.getDoubleAt(prefix + "dead_time"),
                       reader.getDoubleAt(prefix + "x_zero_of_COM"),
                       reader.getDoubleAt(prefix + "y_zero_of_COM"),
                       reader.getDoubleAt(prefix + "time_zero_of_TOF")) {
  return;
}
Analysis::ObjectParameters::~ObjectParameters() { return; }
const double &Analysis::ObjectParameters::getXZeroOfCOM() const {
  return this->xZeroOfCOM;
}
const double &Analysis::ObjectParameters::getYZeroOfCOM() const {
  return this->yZeroOfCOM;
}
const double &Analysis::ObjectParameters::getTimeZeroOfTOF() const {
  return this->timeZeroOfTOF;
}
const double &Analysis::ObjectParameters::getPixelSizeOfX() const {
  return this->pixelSizeOfX;
}
const double &Analysis::ObjectParameters::getPixelSizeOfY() const {
  return this->pixelSizeOfY;
}
const double &Analysis::ObjectParameters::getAngleOfDetector() const {
  return this->angleOfDetector;
}
const double &Analysis::ObjectParameters::getDeadTime() const {
  return this->deadTime;
}
