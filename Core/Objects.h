//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H

#include "Object.h"

namespace Analysis {
class Objects {
 private:
  const int numberOfHits;
  const int numberOfHitsUsed;
  const double outOfFrame = OUT_OF_FRAME;
  Object **ppObject;
  const int getNumberOfDeadObjects() const;
  const int getNumberOfDeadDummyObjects() const;
  const int getNumberOfDeadRealOrDummyObjects() const;

 protected:
  Objects(const int &n, const int &m); // main initializer
  virtual ~Objects();
  void setObject(const int &, Object &);
  void setDummyObject(const int &, Object &);

 public:
  // set & reset
  Object &setObjectMembers(const int &);
  Object &setDummyObjectMembers(const int &);
  Object &setRealOrDummyObjectMembers(const int &);
  void setAllOfObjectIsOutOfFrameOfBasicDataFlag();
  void setAllOfObjectIsOutOfFrameOfMomentumDataFlag();
  void setAllOfObjectIsInFrameOfAllDataFlag();
  void setAllOfDummyObjectIsOutOfFrameOfBasicDataFlag();
  void setAllOfDummyObjectIsOutOfFrameOfMomentumDataFlag();
  void setAllOfDummyObjectIsInFrameOfAllDataFlag();
  void setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag();
  void setAllOfRealOrDummyObjectIsOutOfFrameOfMomentumDataFlag();
  void setAllOfRealOrDummyObjectIsInFrameOfAllDataFlag();
  void resetEventData();
  const int &getNumberOfObjects() const;
  const int &getNumberOfRealOrDummyObjects() const;
  const Object &getObject(const int &) const;
  const Object &getDummyObject(const int &) const;
  const Object &getRealOrDummyObject(const int &) const;
  // get
  const int getNumberOfDummyObject() const;
  const double getLocationX() const; // could be out of frame 
  const double getLocationY() const; // could be out of frame 
  const double getLocationXY() const; // could be out of frame 
  const double getLocation() const; // could be out of frame 
  const double getLocationalDirectionX() const; // could be out of frame 
  const double getLocationalDirectionY() const; // could be out of frame 
  const double getLocationalDirectionXY() const; // could be out of frame 
  const double getLocationalDirectionYX() const; // could be out of frame 
  const double getSumOfTOF(const int &, const int &) const; // could be out of frame (note: don't use macro)
  const double getSumOfTOF(const int &, const int &, const int &) const; // could be out of frame (note: don't use macro)
  const double getSumOfTOF() const; // could be out of frame (note: don't use macro)
  const double getDiffOfTOF(const int &, const int &) const; // could be out of frame (note: don't use macro)
  const double getMomentumX() const; // could be out of frame 
  const double getMomentumY() const; // could be out of frame 
  const double getMomentumZ() const; // could be out of frame 
  const double getMomentumXY() const; // could be out of frame 
  const double getMomentumYZ() const; // could be out of frame 
  const double getMomentumZX() const; // could be out of frame 
  const double getMomentum() const; // could be out of frame 
  const double getMotionalDirectionX() const; // could be out of frame 
  const double getMotionalDirectionY() const; // could be out of frame 
  const double getMotionalDirectionZ() const; // could be out of frame 
  const double getMotionalDirectionXY() const; // could be out of frame 
  const double getMotionalDirectionXZ() const; // could be out of frame 
  const double getMotionalDirectionYX() const; // could be out of frame 
  const double getMotionalDirectionYZ() const; // could be out of frame 
  const double getMotionalDirectionZX() const; // could be out of frame 
  const double getMotionalDirectionZY() const; // could be out of frame 
  const double getEnergy() const; // could be out of frame 
  // get with unit
  const double getLocationX(const Unit &) const; // could be out of frame 
  const double getLocationY(const Unit &) const; // could be out of frame 
  const double getLocationXY(const Unit &) const; // could be out of frame 
  const double getLocation(const Unit &) const; // could be out of frame 
  const double getLocationalDirectionX(const Unit &) const; // could be out of frame 
  const double getLocaitonalDirectionY(const Unit &) const; // could be out of frame 
  const double getLocationalDirectionXY(const Unit &) const; // could be out of frame 
  const double getLocationDirectionYX(const Unit &) const; // could be out of frame
  const double getSumOfTOF(const Unit &) const;
  const double getSumOfTOF(const Unit &, const int &, const int &) const; // could be out of frame (note: don't use macro)
  const double getSumOfTOF(const Unit &, const int &, const int&, const int&) const; // could be out of frame (note: don't use macro)
  const double getDiffOfTOF(const Unit &, const int &, const int &) const; // could be out of frame (note: don't use macro) 
  const double getMomentumX(const Unit &) const; // could be out of frame 
  const double getMomentumY(const Unit &) const; // could be out of frame 
  const double getMomentumZ(const Unit &) const; // could be out of frame 
  const double getMomentumXY(const Unit &) const; // could be out of frame 
  const double getMomentumYZ(const Unit &) const; // could be out of frame 
  const double getMomentumZX(const Unit &) const; // could be out of frame 
  const double getMomentum(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionX(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionY(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionZ(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionXY(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionXZ(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionYX(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionYZ(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionZX(const Unit &) const; // could be out of frame 
  const double getMotionalDirectionZY(const Unit &) const; // could be out of frame 
  const double getEnergy(const Unit &) const; // could be out of frame 
  // bools
  const bool isDummyObject(const int &) const;
  const bool isRealObject(const int &) const;
  const bool isRealOrDummyObject(const int &) const;
  const bool existDeadObject() const;
  const bool existDeadDummyObject() const;
  const bool existDeadRealOrDummyObject() const;
  const bool existOutOfFrameOfBasicDataObject() const; 
  const bool existOutOfFrameOfMomentumDataObject() const; 
  const bool areAllDeadObjects() const;
  const bool areAllDeadDummyObjects() const;
  const bool areAllDeadRealAndDummyObjects() const;
  const bool areAllWithinMasterRegion() const;
  const bool areAllMostReliableObject() const;
  const bool areAllMostOrSecondMostReliableObject() const;
};
}

#endif //ANALYSIS_OBJECTS_H
