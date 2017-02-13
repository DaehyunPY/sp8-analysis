//
// Created by Daehyun You on 12/1/15.
//

#include "AnalysisCore/EventDataReader.h"

Analysis::EventDataReader::~EventDataReader() {
}
Analysis::EventDataReader::EventDataReader(const int maxNumOfIons, const int maxNumOfElecs)
    : maxIons(maxNumOfIons), maxElecs(maxNumOfElecs) {
  eventData.resize((unsigned long) (3*(maxNumOfIons+maxNumOfElecs)));
  flagData.resize((unsigned long) (maxNumOfIons+maxNumOfElecs));
  reset();
}
int Analysis::EventDataReader::getAdressAt(const TreeName name, const int i) const {
  int adress = 0;
  if(isAnyFlagTree(name)) {
    adress += i;
    if (name==IonFlag) {
      assert(0 <= i && i<maxIons);
    } else if (name==ElecFlag) {
      assert(0 <= i && i<maxElecs);
      adress += maxIons;
    } else {
      assert(false);
      return intDum;
    }
    return adress;
  } else if(isAnyEventTree(name)) {
    adress += 3*i;
    if (isAnyTTree(name)) {
      adress += 0;
    } else if (isAnyXTree(name)) {
      adress += 1;
    } else if (isAnyYTree(name)) {
      adress += 2;
    } else {
      assert(false);
      return intDum;
    }
    if (isAnyIonTree(name)) {
      assert(0 <= i && i<maxIons);
    } else if (isAnyElecTree(name)) {
      assert(0 <= i && i<maxElecs);
      adress += 3*maxIons;
    } else {
      assert(false);
      return intDum;
    }
    return adress;
  } else {
    assert(false);
    return intDum;
  }
}
double Analysis::EventDataReader::getEventDataAt(const TreeName name, const int i) const {
  if (!isAnyEventTree(name)) assert(false);
  if (returnDum(name, i)) return doubleDum;
  else return eventData[getAdressAt(name, i)];
}
double &Analysis::EventDataReader::setEventDataAt(const TreeName name, const int i) {
  if (!isAnyEventTree(name)) assert(false);
  return eventData[getAdressAt(name, i)];
}
int &Analysis::EventDataReader::setFlagDataAt(const TreeName name, const int i) {
  if (!isAnyFlagTree(name)) assert(false);
  return flagData[getAdressAt(name, i)];
}
int Analysis::EventDataReader::getFlagDataAt(const TreeName name, const int i) const {
  if (!isAnyFlagTree(name)) assert(false);
  if (returnDum(name, i)) return intDum;
  else return flagData[getAdressAt(name, i)];
}
void Analysis::EventDataReader::reset() {
  numIons = intDum;
  numElecs = intDum;
  std::fill(eventData.begin(), eventData.end(), doubleDum);
  std::fill(flagData.begin(), flagData.end(), intDum);
}
#define RETURN_VALNAME(NAME) case NAME: return #NAME ;
#define RETURN_VALNAMEWITHNUM(NAME) case NAME: return std::string(#NAME)+ch;
std::string Analysis::EventDataReader::getTreeName(const TreeName name, const int i) {
  char ch[2];
  if (i != -1) {
    sprintf(ch, "%01d", i);
  }
  switch(name) {
    RETURN_VALNAME(IonNum)
    RETURN_VALNAMEWITHNUM(IonT)
    RETURN_VALNAMEWITHNUM(IonX)
    RETURN_VALNAMEWITHNUM(IonY)
    RETURN_VALNAMEWITHNUM(IonFlag)
    RETURN_VALNAME(ElecNum)
    RETURN_VALNAMEWITHNUM(ElecT)
    RETURN_VALNAMEWITHNUM(ElecX)
    RETURN_VALNAMEWITHNUM(ElecY)
    RETURN_VALNAMEWITHNUM(ElecFlag)
    default:
      assert(false);
      return "";
  }
}
int &Analysis::EventDataReader::setNumObjs(const Analysis::EventDataReader::TreeName name) {
  if (!isAnyNumTree(name)) assert(false);
  if (name==IonNum) return numIons;
  else return numElecs; // name==ElecNum
}
int Analysis::EventDataReader::getNumObjs(const Analysis::EventDataReader::TreeName name) const {
  if (!isAnyNumTree(name)) assert(false);
  if (name==IonNum) {
    if (numIons==intDum) return maxIons;
    else return numIons;
  } else { // name==ElecNum
    if (numElecs==intDum) return maxElecs;
    else return numElecs;
  }
}
bool Analysis::EventDataReader::returnDum(const Analysis::EventDataReader::TreeName name, const int i) const {
  if (isAnyNumTree(name)) assert(false);
  if (!(isAnyEventTree(name)||isAnyFlagTree(name))) assert(false);
  if (i < 0) {
    assert(false);
    return true;
  }
  if (isAnyIonTree(name)) return i >= getNumObjs(IonNum);
  else return i >= getNumObjs(ElecNum); // isAnyElecTree(name)
}
bool Analysis::EventDataReader::isAnyIonTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonNum, IonT, IonX, IonY, IonFlag}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyElecTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {ElecNum, ElecT, ElecX, ElecY, ElecFlag}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyNumTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonNum, ElecNum}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyEventTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonT, IonX, IonY, ElecT, ElecX, ElecY}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyFlagTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonFlag, ElecFlag}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyTTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonT, ElecT}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyXTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonX, ElecX}) {
    if (name==n) return true;
  }
  return false;
}
bool Analysis::EventDataReader::isAnyYTree(const Analysis::EventDataReader::TreeName name) const {
  for (TreeName n : {IonY, ElecY}) {
    if (name==n) return true;
  }
  return false;
}
