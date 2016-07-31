#ifndef ANALYSIS_OBJECT_H
#define ANALYSIS_OBJECT_H


#include <assert.h>
#include "../Protocols/Unit.h"
#include "ObjectFlag.h"

namespace Analysis {
class Object: public ObjectFlag {
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

 protected:
  Object(const double &m, const double &q, const double &t0, const double &t1);

 public:
  Object(); // for dummy object
  ~Object();
  void resetEventData();
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
  double * const outputCosPDirZ() const;
  double * const outputE() const;
};
}

#endif
