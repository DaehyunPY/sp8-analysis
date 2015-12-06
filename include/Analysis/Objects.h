//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H


#define MAXIMUM_OF_HITS 9
#include "Object.h"

namespace Analysis {
class Objects {
 protected:
  const int maximumOfHits = MAXIMUM_OF_HITS;
  const int numberOfHits;
  const int numberOfHitsUsed;
  Object *pObject[MAXIMUM_OF_HITS];

 public:
  Objects(const int &);
  Objects(const int &, const int &);
  virtual ~Objects();
  void setObject(const int &, Object &);
  void resetEventData();
  const int &getNumberOfHits() const;
  const int &getNumberOfHitsUsed() const;
  const Object &getObject(const int &) const;
  Analysis::Object &setObjectMembers(const int &);
  const double getLocationXOfCOM() const;
  const double getLocationYOfCOM() const;
  const double getTotalMomentumX() const;
  const double getTotalMomentumY() const;
  const double getTotalMomentumZ() const;
  const double getTotalAbsoluteMomentum() const;
  const double getTotalEnergy() const;
  const double getDirectionXOfCOM() const;
  const double getDirectionYOfCOM() const;
  const double getDirectionZOfCOM() const;
  const double getLocationXOfCOM(Unit &) const;
  const double getLocationYOfCOM(Unit &) const;
  const double getTotalMomentumX(Unit &) const;
  const double getTotalMomentumY(Unit &) const;
  const double getTotalMomentumZ(Unit &) const;
  const double getTotalAbsoluteMomentum(Unit &) const;
  const double getTotalEnergy(Unit &) const;
};
}

#endif //ANALYSIS_OBJECTS_H
