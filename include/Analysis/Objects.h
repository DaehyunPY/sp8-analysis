//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H


#include "Object.h"

namespace Analysis {
const int maximumOfHits = 9;
class Objects {
 protected:
  const int numberOfHits;
  Object *pObject[maximumOfHits];

 public:
  Objects(const int &n);
  virtual ~Objects();
  void setObject(const int &i, Object &object);
  const int &getNumberOfHits() const;
  const Object &getObject(const int &i) const;
  const double getLocationXOfCOM() const;
  const double getLocationYOfCOM() const;
  const double getMomentumXOfCOM() const;
  const double getMomentumYOfCOM() const;
  const double getMomentumZOfCOM() const;
  const double getMomentumOfCOM() const;
  const double getEnergyOfCOM() const;
  const double getDirectionXOfCOM() const;
  const double getDirectionYOfCOM() const;
  const double getDirectionZOfCOM() const;
};
}

#endif //ANALYSIS_OBJECTS_H
