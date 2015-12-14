#ifndef ANALYSIS_OBJECT_H
#define ANALYSIS_OBJECT_H


#include "Unit.h"
#include "ObjectFlag.h"
#include "JSONReader.h"

namespace Analysis {
class Object {
 private:
  const double mass;
  const double charge;
  const double minOfTOF;
  const double maxOfTOF;
  double locationX;
  double locationY;
  double TOF;
  double momentumX;
  double momentumY;
  double momentumZ;
  ObjectFlag objectFlag;
  const double &getMinOfTOF() const;
  const double &getMaxOfTOF() const;

 protected:
  Object(); // for dummy object
  Object(const double &m, const double &q, const double &t0, const double &t1);
  virtual ~Object();

 public:
  void resetEventData();
  void setLocationX(const double &);
  void setLocationY(const double &);
  void setTOF(const double &);
  void setMomentumX(const double &);
  void setMomentumY(const double &);
  void setMomentumZ(const double &);
  ObjectFlag &setObjectFlagMembers();
  const double &getMass() const;
  const double &getCharge() const;
  const double &getLocationX() const;
  const double &getLocationY() const;
  const double &getTOF() const;
  const double &getMomentumX() const;
  const double &getMomentumY() const;
  const double &getMomentumZ() const;
  const ObjectFlag &getObjectFlag() const;
  const double getMomentum() const;
  const double getMomentumXY() const;
  const double getDirectionX() const;
  const double getDirectionY() const;
  const double getDirectionZ() const;
  const double getEnergy() const;
  const double getEnergyXY() const;
  const double getLocationX(const Unit &) const;
  const double getLocationY(const Unit &) const;
  const double getTOF(const Unit &) const;
  const double getMomentumX(const Unit &) const;
  const double getMomentumY(const Unit &) const;
  const double getMomentumZ(const Unit &) const;
  const double getMomentum(const Unit &) const;
  const double getMomentumXY(const Unit &) const;
  const double getEnergy(const Unit &) const;
  const double getEnergyXY(const Unit &) const;
  const bool isWithinMasterRegion() const;
};
}

#endif
