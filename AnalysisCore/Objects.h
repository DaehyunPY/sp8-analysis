//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H

#include <assert.h>
#include <iostream>
#include "Object.h"

namespace Analysis {
class Objects {
 private:
  const int numberOfHits;
  const int numberOfHitsUsed;
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
  const double getSumOfTOF() const; // could be out of frame (note: don't use macro)
  const double getSumOfTOF(const int &, const int &) const; // could be out of frame (note: don't use macro)
  const double getSumOfTOF(const int &, const int &, const int &) const; // could be out of frame (note: don't use macro)
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

  // output 
public:
	double * const outputCOMLocX() const;
	double * const outputCOMLocY() const;
	double * const outputCOMLocR() const;
	double * const outputCOMLocDir() const;
	double * const outputTotalTOF() const;
	double * const outputSumOf2TOFs(const int i1, const int i2) const;
	double * const outputSumOf3TOFs(const int i1, const int i2, const int i3) const;
	double * const outputDiffOfTOFs(const int i1, const int i2) const;
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

  // bools
  const bool isDummyObject(const int &) const;
  const bool isRealObject(const int &) const;
  const bool isRealOrDummyObject(const int &) const;
  enum OptName {
    Real, Dummy, RealOrDummy
  };
  const bool areAllFlag(const ObjectFlag::FlagName flagName, const OptName optName=Real) const;
  const bool existFlag(const ObjectFlag::FlagName flagName, const OptName optName=Real) const;
  void setAllFlag(const ObjectFlag::FlagName flagName, const OptName optName=Real);

  // Delete blew
  const bool existOutOfFrameOfBasicDataObject() const;
  const bool existOutOfFrameOfMomentumDataObject() const;
};
}

#endif //ANALYSIS_OBJECTS_H
