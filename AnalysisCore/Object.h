#ifndef ANALYSIS_OBJECT_H
#define ANALYSIS_OBJECT_H


#include <assert.h>
#include "../Protocols/Unit.h"
#include "ObjectFlag.h"
#include "../Protocols/JSONReader.h"

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
  bool hasOptDxAndDy;
  double dx;
  double dy;
  bool hasOptPhi;
  double frPhi;
  double toPhi;

 public:
  void setLocationX(const double &);
  void setLocationY(const double &);
  void setTOF(const double &);
  void setMomentumX(const double &);
  void setMomentumY(const double &);
  void setMomentumZ(const double &);
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
  double * const outputLocX() const;
  double * const outputLocY() const;
  double * const outputLocR() const;
  double * const outputLocDir() const;
  double * const outputTOF() const;
  double * const outputPX() const;
  double * const outputPY() const;
  double * const outputPZ() const;
  double * const outputPXY() const;
  double * const outputPYZ() const;
  double * const outputPZX() const; 
  double * const outputP() const;
  double * const outputPDirX() const;
  double * const outputPDirY() const;
  double * const outputPDirZ() const;
  double * const outputPDirXY() const;
  double * const outputPDirXZ() const;
  double * const outputPDirYX() const;
  double * const outputPDirYZ() const;
  double * const outputPDirZX() const;
  double * const outputPDirZY() const;
  double * const outputCosPDirX() const;
  double * const outputCosPDirY() const;
  double * const outputCosPDirZ() const;
  double * const outputE() const;
};
}

#endif
