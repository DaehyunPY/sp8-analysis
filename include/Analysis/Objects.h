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
  const int getNumberOfDummyObject() const; 
  const double getLocationXOfCOM() const;
  const double getLocationYOfCOM() const;
  const double getTotalMomentumX() const;
  const double getTotalMomentumY() const;
  const double getTotalMomentumZ() const;
  const double getTotalMomentum() const;
  const double getTotalMomentumXY() const;
  const double getTotalEnergy() const;
  const double getTotalEnergyXY() const;
  const double getDirectionXOfCOM() const;
  const double getDirectionYOfCOM() const;
  const double getDirectionZOfCOM() const;
  const double getLocationXOfCOM(Unit &) const;
  const double getLocationYOfCOM(Unit &) const;
  const double getTotalMomentumX(Unit &) const;
  const double getTotalMomentumY(Unit &) const;
  const double getTotalMomentumZ(Unit &) const;
  const double getTotalMomentum(Unit &) const;
  const double getTotalMomentumXY(Unit &) const;
  const double getTotalEnergy(Unit &) const;
  const double getTotalEnergyXY(Unit &) const;
  const int getNumberOfDeadObjects() const;
  const int getNumberOfDeadDummyObjects() const;
  const int getNumberOfDeadRealOrDummyObjects() const;
  const bool isDummyObject(const int &) const;
  const bool isRealObject(const int &) const;
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
