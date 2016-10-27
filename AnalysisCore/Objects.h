//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H

#include <assert.h>
#include <iostream>
#include "Object.h"
#include "../Protocols/JSONReader.h"

namespace Analysis {
class Objects {
 private:
  const int maxNumOfHits;
  int masterNumOfHits;
//  int numOfHits;
  Object **ppObject;
  // todo: stop dividing objects real or dummy from `masterNumOfHits'
  const int getNumberOfDeadObjects() const;
  const int getNumberOfDeadDummyObjects() const;
  const int getNumberOfDeadRealOrDummyObjects() const;
 public:
  enum ObjsType { none, ions, elecs };
  bool isType(const ObjsType objsType) const;
 private:
  const ObjsType type;
 protected:
  void setObject(const int &, Object &);
  void setDummyObject(const int &, Object &);
 public:
  Objects(ObjsType tp, const int maxNum, const JSONReader &reader, const std::string prefix="");
  ~Objects();

 private:
  double frPx, toPx, frPy, toPy, frPz, toPz, frPr, toPr, frE, toE;
 public:
  bool isMomentumAndEnergyConserved() const;

public:
  // set & reset
  Object &setObjectMembers(const int &);
  Object &setDummyObjectMembers(const int &);
  Object &setRealOrDummyObjectMembers(const int &);
  void resetEventData();
  const int &getNumberOfObjects() const;
  const int &getNumberOfRealOrDummyObjects() const;
  const Object &getObject(const int &) const;
  const Object &getDummyObject(const int &) const;
  const Object &getRealOrDummyObject(const int &) const;

  // get
  const int getNumberOfDummyObject() const;
  const double getLocationX() const;
  const double getLocationY() const;
  const double getLocationXY() const;
  const double getLocation() const;
  const double getLocationalDirectionX() const;
  const double getLocationalDirectionY() const;
  const double getLocationalDirectionXY() const;
  const double getLocationalDirectionYX() const;
  const double getSumOfTOF() const;
  const double getSumOfTOF(const int &, const int &) const;
  const double getSumOfTOF(const int &, const int &, const int &) const;
  const double getDiffOfTOF(const int &, const int &) const;
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

  // output 
 public:
  double *const outputCOMLocX() const;
  double *const outputCOMLocY() const;
  double *const outputCOMLocR() const;
  double *const outputCOMLocDir() const;
  double *const outputTotalTOF() const;
  double *const outputSumOf2TOFs(const int i1, const int i2) const;
  double *const outputSumOf3TOFs(const int i1, const int i2, const int i3) const;
  double *const outputDiffOfTOFs(const int i1, const int i2) const;
  double *const outputPX() const;
  double *const outputPY() const;
  double *const outputPZ() const;
  double *const outputPXY() const;
  double *const outputPYZ() const;
  double *const outputPZX() const;
  double *const outputP() const;
  double *const outputPDirX() const;
  double *const outputPDirY() const;
  double *const outputPDirZ() const;
  double *const outputPDirXY() const;
  double *const outputPDirXZ() const;
  double *const outputPDirYX() const;
  double *const outputPDirYZ() const;
  double *const outputPDirZX() const;
  double *const outputPDirZY() const;
  double *const outputCosPDirX() const;
  double *const outputCosPDirY() const;
  double *const outputCosPDirZ() const;
  double *const outputE() const;

  // bools
  const bool isDummyObject(const int &) const;
  const bool isRealObject(const int &) const;
  const bool isRealOrDummyObject(const int &) const;
  enum OptName { Real, Dummy, RealOrDummy };
  const bool areAllFlag(const ObjectFlag::FlagName flagName, const OptName optName = Real) const;
  const bool existFlag(const ObjectFlag::FlagName flagName, const OptName optName = Real) const;
  void setAllFlag(const ObjectFlag::FlagName flagName, const OptName optName = Real);

 private:
  const std::string getStrNum(int i) const;
};
}

#endif //ANALYSIS_OBJECTS_H
