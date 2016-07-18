#include "ObjectParameters.h"

void Analysis::ObjectParameters::inputParameters(const ParameterType tp, const double theta, const double dx, const double dy, const double t1, const double x0, const double y0, const double t0)
{
	type = tp;
	angleOfDetector = theta;
	pixelSizeOfX = dx; 
	pixelSizeOfY = dy; 
	deadTime = t1; 
	xZeroOfCOM = x0;
	yZeroOfCOM = y0;
	timeZeroOfTOF = t0;
}

Analysis::ObjectParameters::ObjectParameters(
    const ParameterType tp,
    const double theta,
    const double dx,
    const double dy,
    const double t1,
    const double x0,
    const double y0,
    const double t0)
{
	inputParameters(tp, theta, dx, dy, t1, x0, y0, t0); 
}
Analysis::ObjectParameters::ObjectParameters(
    const Analysis::Unit &unit,
    const std::string typeName,
    const double theta,
    const double dx,
    const double dy,
    const double t1,
    const double x0,
    const double y0,
    const double t0) {
  if(typeName.compare("legacy_ion_parameters") == 0) {
    const double th = unit.readDegree(0);
	inputParameters(
        legacy_ion_parameters,
        th,
        dx,
        dy,
        unit.readNanoSec(t1),
        unit.readMilliMeter(100*dx + x0),
        unit.readMilliMeter(100*dy + y0),
        unit.readNanoSec(2000 + t0));
  } else if(typeName.compare("legacy_elec_parameters") == 0) {
    const double th = unit.readDegree(-30);
	inputParameters(
        legacy_elec_parameters_not_corrected,
        th,
        dx,
        dy,
        unit.readNanoSec(t1),
        unit.readMilliMeter(100*(cos(th)-sin(th)) + x0),
        unit.readMilliMeter(100*(sin(th)+cos(th)) + y0),
        unit.readNanoSec(1980 + t0));
  } else {
	  inputParameters(
        default_type,
        unit.readDegree(theta),
        dx,
        dy,
        unit.readNanoSec(t1),
        unit.readMilliMeter(x0),
        unit.readMilliMeter(y0),
        unit.readNanoSec(t0));
  }
  return;
}
Analysis::ObjectParameters::ObjectParameters(const Unit &unit,
                                             const JSONReader &reader,
                                             const std::string &prefix)
    : ObjectParameters(unit,
                       reader.getStringAt(prefix + "type"),
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
  if(type==legacy_elec_parameters_not_corrected) {
    timeZeroOfTOF -= elecTOF;
    type = legacy_elec_parameters;
  }
}
const Analysis::ObjectParameters::ParameterType Analysis::ObjectParameters::getParameterType() const {
  return type;
}
