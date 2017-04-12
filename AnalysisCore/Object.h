#ifndef ANALYSIS_OBJECT_H
#define ANALYSIS_OBJECT_H


#include <memory>
#include <assert.h>
#include "../Core/Unit.h"
#include "ObjectFlag.h"
#include "../Core/JSONReader.h"
#include <functional>

namespace Analysis {
class Object: public ObjectFlag {
  double mass;
  double charge;
  double minTOF;
  double maxTOF;
  double locationX;
  double locationY;
  double TOF;
  double momentumX;
  double momentumY;
  double momentumZ;
 public:
  Object(const FlagName f1, const FlagName f2,
         double m = 0, double q = 0, const double t0 = 0, const double t1 = 0);
  Object(const FlagName f, const JSONReader &reader, const std::string prefix);
  ~Object();
  void resetEventData();
  Object getCopy() const;

 private:
  double dx, dy;
  double frPhi, toPhi;
  double frPx, toPx, frPy, toPy, frPz, toPz, frPr, toPr, frE, toE;
 public:
  bool isMomentumAndEnergyConserved() const;

 public:
  void setLocationX(const double &);
  void setLocationY(const double &);
  void setTOF(const double &);
  void setMomentumX(const double &);
  void setMomentumY(const double &);
  void setMomentumZ(const double &);

 public:
  const double &getMass() const;
  const double &getCharge() const;
  const double &getMinOfTOF() const;
  const double &getMaxOfTOF() const;
  const double &getLocationX() const;
  const double &getLocationY() const;
  const double &getTOF() const;
  const double &getMomentumX() const;
  const double &getMomentumY() const;
  const double &getMomentumZ() const;
  const double getLocationXY() const;
  const double getLocation() const;
  const double getLocationalDirectionX() const;
  const double getLocationalDirectionY() const;
  const double getLocationalDirectionXY() const;
  const double getLocationalDirectionYX() const;
  const double getMomentumXY() const;
  const double getMomentumYZ() const;
  const double getMomentumZX() const;
  const double getMomentum() const;
  const double getMotionalDirectionX() const;
  const double getMotionalDirectionY() const;
  const double getMotionalDirectionZ() const;
  const double getMotionalDirectionXY() const;
  const double getMotionalDirectionXZ() const;
  const double getMotionalDirectionYX() const;
  const double getMotionalDirectionYZ() const;
  const double getMotionalDirectionZX() const;
  const double getMotionalDirectionZY() const;
  const double getEnergy() const;

  // output
public:
  std::shared_ptr<double> outputLocX() const;
  std::shared_ptr<double> outputLocY() const;
  std::shared_ptr<double> outputLocR() const;
  std::shared_ptr<double> outputLocDir() const;
  std::shared_ptr<double> outputTOF() const;
  std::shared_ptr<double> outputPX() const;
  std::shared_ptr<double> outputPY() const;
  std::shared_ptr<double> outputPZ() const;
  std::shared_ptr<double> outputPXY() const;
  std::shared_ptr<double> outputPYZ() const;
  std::shared_ptr<double> outputPZX() const;
  std::shared_ptr<double> outputP() const;
  std::shared_ptr<double> outputPDirX() const;
  std::shared_ptr<double> outputPDirY() const;
  std::shared_ptr<double> outputPDirZ() const;
  std::shared_ptr<double> outputPDirXY() const;
  std::shared_ptr<double> outputPDirXZ() const;
  std::shared_ptr<double> outputPDirYX() const;
  std::shared_ptr<double> outputPDirYZ() const;
  std::shared_ptr<double> outputPDirZX() const;
  std::shared_ptr<double> outputPDirZY() const;
  std::shared_ptr<double> outputCosPDirX() const;
  std::shared_ptr<double> outputCosPDirY() const;
  std::shared_ptr<double> outputCosPDirZ() const;
  std::shared_ptr<double> outputE() const;
};
}

#endif
