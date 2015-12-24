#ifndef ANALYSIS_OBJECT_H
#define ANALYSIS_OBJECT_H


#include "Unit.h"
#include "ObjectFlag.h"

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
  ObjectFlag flag;
  const double &getMinOfTOF() const;
  const double &getMaxOfTOF() const;

 protected:
  Object(const double &m, const double &q, const double &t0, const double &t1);
  virtual ~Object();

 public:
  Object(); // for dummy object
  void resetEventData();
  void setLocationX(const double &);
  void setLocationY(const double &);
  void setTOF(const double &);
  void setMomentumX(const double &);
  void setMomentumY(const double &);
  void setMomentumZ(const double &);
  ObjectFlag &setFlagMembers();
  const double &getMass() const;
  const double &getCharge() const;
  const double &getLocationX() const;
  const double &getLocationY() const;
  const double &getTOF() const;
  const double &getMomentumX() const;
  const double &getMomentumY() const;
  const double &getMomentumZ() const;
  const ObjectFlag &getFlag() const;
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
  const double getLocationX(const Unit &) const;
  const double getLocationY(const Unit &) const;
  const double getLocationXY(const Unit &) const;
  const double getLocation(const Unit &) const;
  const double getLocationalDirectionX(const Unit &) const;
  const double getLocationalDirectionY(const Unit &) const;
  const double getLocationalDirectionXY(const Unit &) const;
  const double getLocationalDirectionYX(const Unit &) const;
  const double getTOF(const Unit &) const;
  const double getMomentumX(const Unit &) const;
  const double getMomentumY(const Unit &) const;
  const double getMomentumZ(const Unit &) const;
  const double getMomentumXY(const Unit &) const;
  const double getMomentumYZ(const Unit &) const;
  const double getMomentumZX(const Unit &) const;
  const double getMomentum(const Unit &) const;
  const double getMotionalDirectionX(const Unit &) const;
  const double getMotionalDirectionY(const Unit &) const;
  const double getMotionalDirectionZ(const Unit &) const;
  const double getMotionalDirectionXY(const Unit &) const;
  const double getMotionalDirectionXZ(const Unit &) const;
  const double getMotionalDirectionYX(const Unit &) const;
  const double getMotionalDirectionYZ(const Unit &) const;
  const double getMotionalDirectionZX(const Unit &) const;
  const double getMotionalDirectionZY(const Unit &) const;
  const double getEnergy(const Unit &) const;
  const bool isWithinMasterRegion() const;
  const bool isDead() const;
};
}

#endif
