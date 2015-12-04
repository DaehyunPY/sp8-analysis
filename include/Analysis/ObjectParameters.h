#ifndef ANALYSIS_OBJECTPARAMETERS_H
#define ANALYSIS_OBJECTPARAMETERS_H

#include "Unit.h"
#include "JSONReader.h"

namespace Analysis {
class ObjectParameters {
 private:
  const double angleOfDetector;
  const double pixelSizeOfX;
  const double pixelSizeOfY;
  const double deadTime;
  const double xZeroOfCOM;
  const double yZeroOfCOM;
  const double timeZeroOfTOF;

  ObjectParameters(const double &theta,
                   const double &dx, const double &dy, const double &t1,
                   const double &x0, const double &y0, const double &t0);
  ObjectParameters(const Analysis::Unit &unit, const double &theta,
                   const double &dx, const double &dy, const double &t1,
                   const double &x0, const double &y0, const double &t0);

 public:
  ObjectParameters(const Unit &unit, const JSONReader &reader,
                   const std::string &prefix);
  virtual ~ObjectParameters();

  const double &getAngleOfDetector() const;
  const double &getPixelSizeOfX() const;
  const double &getPixelSizeOfY() const;
  const double &getDeadTime() const;
  const double &getXZeroOfCOM() const;
  const double &getYZeroOfCOM() const;
  const double &getTimeZeroOfTOF() const;
};
}

#endif
