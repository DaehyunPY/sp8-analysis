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
  Analysis::Object &setObjectMembers(const int &);
  Analysis::Object &setDummyObjectMembers(const int &);
  void resetEventData();
  const int &getNumberOfObjects() const;
  const int &getNumberOfRealOrDummyObjects() const;
  const Object &getObject(const int &) const;
  const Object &getDummyObject(const int &) const;
  const int __unused getNumberOfDummyObject() const;
  const double getLocationXOfCOM() const;
  const double getLocationYOfCOM() const;
  const double getTotalMomentumX() const;
  const double getTotalMomentumY() const;
  const double getTotalMomentumZ() const;
  const double getTotalMomentum() const;
  const double getTotalMomentumXY() const;
  const double getTotalEnergy() const;
  const double getTotalEnergyXY() const;
  const double __unused getDirectionXOfCOM() const;
  const double __unused getDirectionYOfCOM() const;
  const double __unused getDirectionZOfCOM() const;
  const double __unused getLocationXOfCOM(Unit &) const;
  const double __unused getLocationYOfCOM(Unit &) const;
  const double __unused getTotalMomentumX(Unit &) const;
  const double __unused getTotalMomentumY(Unit &) const;
  const double __unused getTotalMomentumZ(Unit &) const;
  const double __unused getTotalMomentum(Unit &) const;
  const double __unused getTotalMomentumXY(Unit &) const;
  const double __unused getTotalEnergy(Unit &) const;
  const double __unused getTotalEnergyXY(Unit &) const;
  const int getNumberOfDeadObjects() const;
  const int getNumberOfDeadDummyObjects() const;
  const int getNumberOfDeadRealOrDummyObjects() const;
  const bool isDummyObject(const int &) const;
  const bool isRealObject(const int &) const;
  const bool existDeadObject() const;
  const bool existDeadDummyObject() const;
  const bool __unused existDeadRealOrDummyObject() const;
  const bool __unused isAllDeadObjects() const;
  const bool __unused isAllDeadDummyObjects() const;
  const bool __unused isAllDeadRealAndDummyObjects() const;
  const bool __unused isAllWithinMasterRegion() const;
};
}

#endif //ANALYSIS_OBJECTS_H
