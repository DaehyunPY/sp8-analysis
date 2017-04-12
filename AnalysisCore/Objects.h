//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_OBJECTS_H
#define ANALYSIS_OBJECTS_H

#include <assert.h>
#include <iostream>
#include "Object.h"
#include "../Core/JSONReader.h"

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
  std::shared_ptr<double> outputCOMLocX() const;
  std::shared_ptr<double> outputCOMLocY() const;
  std::shared_ptr<double> outputCOMLocR() const;
  std::shared_ptr<double> outputCOMLocDir() const;
  std::shared_ptr<double> outputTotalTOF() const;
  std::shared_ptr<double> outputSumOf2TOFs(const int i1, const int i2) const;
  std::shared_ptr<double> outputSumOf3TOFs(const int i1, const int i2, const int i3) const;
  std::shared_ptr<double> outputDiffOfTOFs(const int i1, const int i2) const;
  std::shared_ptr<double> outputPX() const;
  std::shared_ptr<double> outputPY() const;
  std::shared_ptr<double> outputPZ() const;
  std::shared_ptr<double> outputPXY() const;
  std::shared_ptr<double> outputPYZ() const;
  std::shared_ptr<double> outputPZX() const;
  std::shared_ptr<double> outputP() const;
  std::shared_ptr<double> outputPDirX() const;
  std::shared_ptr<double> outputPDirY() const;
  std::shared_ptr<double> outputPDirZ() const;
  std::shared_ptr<double> outputPDirXY() const;
  std::shared_ptr<double> outputPDirXZ() const;
  std::shared_ptr<double> outputPDirYX() const;
  std::shared_ptr<double> outputPDirYZ() const;
  std::shared_ptr<double> outputPDirZX() const;
  std::shared_ptr<double> outputPDirZY() const;
  std::shared_ptr<double> outputCosPDirX() const;
  std::shared_ptr<double> outputCosPDirY() const;
  std::shared_ptr<double> outputCosPDirZ() const;
  std::shared_ptr<double> outputE() const;

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
