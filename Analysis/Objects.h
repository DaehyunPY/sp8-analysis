//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H


#define MAXIMUM_OF_HITS 9
#include "Object.h"

namespace Analysis {
class Objects {
 private:
  const int maximumOfHits = MAXIMUM_OF_HITS;
  const int numberOfHits;
  const int numberOfHitsUsed;
  Object *pObject[MAXIMUM_OF_HITS];

 protected:
  Objects(const int &);
  Objects(const int &, const int &);
  virtual ~Objects();
  void setObject(const int &, Object &);
  void setDummyObject(const int &, Object &);

 public:
  Object &setObjectMembers(const int &);
  Object &setDummyObjectMembers(const int &);
  Object &setRealOrDummyObjectMembers(const int &);
  void resetEventData();
  const int &getNumberOfObjects() const;
  const int &getNumberOfRealOrDummyObjects() const;
  const Object &getObject(const int &) const;
  const Object &getDummyObject(const int &) const;
  const Object &getRealOrDummyObject(const int &) const;
  const int getNumberOfDummyObject() const;
  const double getLocationX() const;
  const double getLocationY() const;
  const double getLocationXY() const;
  const double getLocation() const;
  const double getLocationalDirectionX() const;
  const double getLocationalDirectionY() const;
  const double getLocationalDirectionXY() const;
  const double getLocationalDirectionYX() const;
  const double getSumOfTOF(const int &, const int&) const;
  const double getMomentumX() const;
  const double getMomentumY() const;
  const double getMomentumZ() const;
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
  const double getLocationDirectionX(const Unit &) const;
  const double getLocaitonDirectionY(const Unit &) const;
  const double getLocationDirectionXY(const Unit &) const;
  const double getLocationDirectionYX(const Unit &) const;
  const double getSumOfTOF(const Unit &, const int &, const int &) const;
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
  const int getNumberOfDeadObjects() const;
  const int getNumberOfDeadDummyObjects() const;
  const int getNumberOfDeadRealOrDummyObjects() const;
  const bool isDummyObject(const int &) const;
  const bool isRealObject(const int &) const;
  const bool isRealOrDummyObject(const int &) const;
  const bool existDeadObject() const;
  const bool existDeadDummyObject() const;
  const bool existDeadRealOrDummyObject() const;
  const bool isAllDeadObjects() const;
  const bool isAllDeadDummyObjects() const;
  const bool isAllDeadRealAndDummyObjects() const;
  const bool isAllWithinMasterRegion() const;
};
}

#endif //ANALYSIS_OBJECTS_H
