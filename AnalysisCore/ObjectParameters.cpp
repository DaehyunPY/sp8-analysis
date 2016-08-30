#include "ObjectParameters.h"

Analysis::ObjectParameters::ObjectParameters(
    const ParameterType tp,
    const double theta,
    const double dx,
    const double dy,
    const double t1,
    const double x0,
    const double y0,
    const double t0) {
  type = tp;
  if (type == default_type) {
    angleOfDetector = kUnit.readDegree(theta);
    pixelSizeOfX = dx;
    pixelSizeOfY = dy;
    deadTime = kUnit.readNanoSec(t1);
    xZeroOfCOM = kUnit.readMilliMeter(x0);
    yZeroOfCOM = kUnit.readMilliMeter(y0);
    timeZeroOfTOF = kUnit.readNanoSec(t0);
  } else if (type == legacy_ion_parameters) {
    angleOfDetector = kUnit.readDegree(0);
    pixelSizeOfX = dx;
    pixelSizeOfY = dy;
    deadTime = kUnit.readNanoSec(t1);
    xZeroOfCOM = kUnit.readMilliMeter(100 * dx + x0);
    yZeroOfCOM = kUnit.readMilliMeter(100 * y0 + y0);
    timeZeroOfTOF = kUnit.readNanoSec(2000 + t0);
  } else if (type == legacy_elec_parameters_not_corrected) {
    angleOfDetector = kUnit.readDegree(-30);
    pixelSizeOfX = dx;
    pixelSizeOfY = dy;
    deadTime = kUnit.readNanoSec(t1);
    xZeroOfCOM = kUnit.readMilliMeter(100 * (cos(angleOfDetector) - sin(angleOfDetector)) + x0);
    yZeroOfCOM = kUnit.readMilliMeter(100 * (sin(angleOfDetector) + cos(angleOfDetector)) + y0);
    timeZeroOfTOF = kUnit.readNanoSec(1980 + t0);
  } else {
    assert(false);
  }
  return;
}
Analysis::ObjectParameters::ObjectParameters(const JSONReader &reader, const std::string &prefix)
    : ObjectParameters((reader.hasMember(prefix + "type") ?
                        (ParameterType) reader.getIntAt(prefix + "type") : default_type),
                       reader.getDoubleAt(prefix + "angle_of_detector"),
                       reader.getDoubleAt(prefix + "pixel_size_of_x"),
                       reader.getDoubleAt(prefix + "pixel_size_of_y"),
                       reader.getDoubleAt(prefix + "dead_time"),
                       reader.getDoubleAt(prefix + "x_zero_of_image"),
                       reader.getDoubleAt(prefix + "y_zero_of_image"),
                       reader.getDoubleAt(prefix + "time_zero_of_TOF")) {
  return;
}
Analysis::ObjectParameters::~ObjectParameters() { return; }
const double &Analysis::ObjectParameters::getXZeroOfCOM() const {
  return xZeroOfCOM;
}
const double &Analysis::ObjectParameters::getYZeroOfCOM() const {
  return yZeroOfCOM;
}
const double &Analysis::ObjectParameters::getTimeZeroOfTOF() const {
  return timeZeroOfTOF;
}
const double &Analysis::ObjectParameters::getPixelSizeOfX() const {
  return pixelSizeOfX;
}
const double &Analysis::ObjectParameters::getPixelSizeOfY() const {
  return pixelSizeOfY;
}
const double &Analysis::ObjectParameters::getAngleOfDetector() const {
  return angleOfDetector;
}
const double &Analysis::ObjectParameters::getDeadTime() const {
  return deadTime;
}
const double Analysis::ObjectParameters::getAngleOfDetector(const Analysis::Unit &unit) const {
  return unit.writeDegree(getAngleOfDetector());
}
const double Analysis::ObjectParameters::getDeadTime(const Analysis::Unit &unit) const {
  return unit.writeNanoSec(getDeadTime());
}
const double Analysis::ObjectParameters::getXZeroOfCOM(const Analysis::Unit &unit) const {
  return unit.writeMilliMeter(getXZeroOfCOM());
}
const double Analysis::ObjectParameters::getYZeroOfCOM(const Analysis::Unit &unit) const {
  return unit.writeMilliMeter(getYZeroOfCOM());
}
const double Analysis::ObjectParameters::getTimeZeroOfTOF(const Analysis::Unit &unit) const {
  return unit.writeNanoSec(getTimeZeroOfTOF());
}
void Analysis::ObjectParameters::correctLegacyParameters(const double &elecTOF) {
  if (type == legacy_elec_parameters_not_corrected) {
    timeZeroOfTOF -= elecTOF;
    type = legacy_elec_parameters;
  }
}
const Analysis::ObjectParameters::ParameterType Analysis::ObjectParameters::getParameterType() const {
  return type;
}
