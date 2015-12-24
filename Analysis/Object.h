#ifndef ANALYSIS_OBJECT_H
#define ANALYSIS_OBJECT_H


#include "Unit.h"
#include "ObjectFlag.h"
#define OUT_OF_FRAME -100000000

namespace Analysis {
class Object {
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
  const ObjectFlag &getFlag() const;

  const double &getLocationX() const; // could be out out frame 
  const double &getLocationY() const; // could be out out frame 
  const double &getTOF() const; // could be out out frame 
  const double &getMomentumX() const; // could be out out frame 
  const double &getMomentumY() const; // could be out out frame 
  const double &getMomentumZ() const; // could be out out frame 
  const double getLocationXY() const; // could be out out frame 
  const double getLocation() const; // could be out out frame 
  const double getLocationalDirectionX() const; // could be out out frame 
  const double getLocationalDirectionY() const; // could be out out frame 
  const double getLocationalDirectionXY() const; // could be out out frame 
  const double getLocationalDirectionYX() const; // could be out out frame 
  const double getMomentumXY() const; // could be out out frame 
  const double getMomentumYZ() const; // could be out out frame 
  const double getMomentumZX() const; // could be out out frame 
  const double getMomentum() const; // could be out out frame 
  const double getMotionalDirectionX() const; // could be out out frame 
  const double getMotionalDirectionY() const; // could be out out frame 
  const double getMotionalDirectionZ() const; // could be out out frame 
  const double getMotionalDirectionXY() const; // could be out out frame 
  const double getMotionalDirectionXZ() const; // could be out out frame 
  const double getMotionalDirectionYX() const; // could be out out frame 
  const double getMotionalDirectionYZ() const; // could be out out frame 
  const double getMotionalDirectionZX() const; // could be out out frame 
  const double getMotionalDirectionZY() const; // could be out out frame 
  const double getEnergy() const; // could be out out frame 

  const double getLocationX(const Unit &) const; // could be out out frame 
  const double getLocationY(const Unit &) const; // could be out out frame 
  const double getLocationXY(const Unit &) const; // could be out out frame 
  const double getLocation(const Unit &) const; // could be out out frame 
  const double getLocationalDirectionX(const Unit &) const; // could be out out frame 
  const double getLocationalDirectionY(const Unit &) const; // could be out out frame 
  const double getLocationalDirectionXY(const Unit &) const; // could be out out frame 
  const double getLocationalDirectionYX(const Unit &) const; // could be out out frame 
  const double getTOF(const Unit &) const; // could be out out frame 
  const double getMomentumX(const Unit &) const; // could be out out frame 
  const double getMomentumY(const Unit &) const; // could be out out frame 
  const double getMomentumZ(const Unit &) const; // could be out out frame 
  const double getMomentumXY(const Unit &) const; // could be out out frame 
  const double getMomentumYZ(const Unit &) const; // could be out out frame 
  const double getMomentumZX(const Unit &) const; // could be out out frame 
  const double getMomentum(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionX(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionY(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionZ(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionXY(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionXZ(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionYX(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionYZ(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionZX(const Unit &) const; // could be out out frame 
  const double getMotionalDirectionZY(const Unit &) const; // could be out out frame 
  const double getEnergy(const Unit &) const; // could be out out frame 

  const bool isWithinMasterRegion() const;
  const bool isDead() const;
};
}

#endif
