#ifndef ANALYSIS_OBJECTPARAMETERS_H
#define ANALYSIS_OBJECTPARAMETERS_H

#include "Core/Unit.h"
#include "Core/JSONReader.h"

namespace Analysis {
class ObjectParameters {
 public:
  enum ParameterType {
    default_type = 0,
    legacy_ion_parameters = 1,
    legacy_elec_parameters_not_corrected = 2,
    legacy_elec_parameters = 3
  };

 private:
  ParameterType type;
  double angleOfDetector;
  double pixelSizeOfX;
  double pixelSizeOfY;
  double deadTime;
  double xZeroOfCOM;
  double yZeroOfCOM;
  double timeZeroOfTOF;

 private:
  ObjectParameters(
      const ParameterType tp,
      const double theta,
      const double dx, const double dy, const double t1,
      const double x0, const double y0, const double t0);

 public:
  ObjectParameters(const JSONReader &reader, const std::string &prefix);
  virtual ~ObjectParameters();

 public:
  const ParameterType getParameterType() const;
  void correctLegacyParameters(const double &elecTOF);
  const double &getAngleOfDetector() const;
  const double &getPixelSizeOfX() const;
  const double &getPixelSizeOfY() const;
  const double &getDeadTime() const;
  const double &getXZeroOfCOM() const;
  const double &getYZeroOfCOM() const;
  const double &getTimeZeroOfTOF() const;
  const double getAngleOfDetector(const Unit &unit) const;
  const double getDeadTime(const Unit &unit) const;
  const double getXZeroOfCOM(const Unit &unit) const;
  const double getYZeroOfCOM(const Unit &unit) const;
  const double getTimeZeroOfTOF(const Unit &unit) const;
};
}

#endif
