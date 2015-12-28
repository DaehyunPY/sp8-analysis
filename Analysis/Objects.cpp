//
// Created by Daehyun You on 12/4/15.
//

#include <assert.h>
#include "Objects.h"

#define ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1 if(existOutOfFrameOfBasicDataObject()) { return outOfFrame; }
#define ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2 if(existOutOfFrameOfBasicDataObject() || existOutOfFrameOfMomentumDataObject()) { return outOfFrame; }

Analysis::Objects::Objects(const int &n)
    : numberOfHits(n), numberOfHitsUsed(n) {
  assert(n > 0 && n <= maximumOfHits);
  return;
}
Analysis::Objects::Objects(const int &n, const int &m)
    : numberOfHits(n), numberOfHitsUsed(m) {
  assert(n <= m);
  assert(n > 0 && m <= maximumOfHits);
  for (int i = 0; i < getMaximumOfHits(); i++) {
    delete pObject[i];
  }
  return;
}
Analysis::Objects::~Objects() {
  for (int i = 0; i < getMaximumOfHits(); i++) {
    delete pObject[i];
  }
  return;
}
void Analysis::Objects::setObject(const int &i, Object &object) {
  assert(this->isRealObject(i));
  delete pObject[i];
  pObject[i] = &object;
  return;
}
void Analysis::Objects::setDummyObject(const int &i, Analysis::Object &object) {
  assert(this->isDummyObject(i));
  delete pObject[i];
  pObject[i] = &object;
}
const int &Analysis::Objects::getNumberOfObjects() const {
  return numberOfHits;
}
const double Analysis::Objects::getLocationX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  const int &n = getNumberOfObjects();
  long double sum1 = 0e0, sum2 = 0e0;
  for (int i = 0; i < n; i++) {
    const double tmp = getObject(i).getMass() / getObject(i).getTOF();
    sum1 += tmp;
    sum2 += tmp * getObject(i).getLocationX();
  }
  return double(sum2 / sum1);
}
const double Analysis::Objects::getLocationY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  const int &n = getNumberOfObjects();
  long double sum1 = 0e0, sum2 = 0e0;
  for (int i = 0; i < n; i++) {
    const double tmp = getObject(i).getMass() / getObject(i).getTOF();
    sum1 += tmp;
    sum2 += tmp * getObject(i).getLocationY();
  }
  return double(sum2 / sum1);
}
const double Analysis::Objects::getMomentumX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  const int &n = getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += getObject(i).getMomentumX();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getMomentumY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  const int &n = getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += getObject(i).getMomentumY();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getMomentumZ() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  const int &n = getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += getObject(i).getMomentumZ();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getEnergy() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  const int &n = getNumberOfObjects();
  long double sumOfEnergy = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfEnergy += getObject(i).getEnergy();
  }
  return double(sumOfEnergy);
}
const Analysis::Object &Analysis::Objects::getObject(const int &i) const {
  assert(isRealObject(i));
  return *pObject[i];
}
const Analysis::Object &Analysis::Objects::getDummyObject(const int &i) const {
  assert(isDummyObject(i));
  return *pObject[i];
}

const double Analysis::Objects::getMomentum() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0)
                 + pow(getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Objects::getMotionalDirectionX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return acos(getMomentumX() / getMomentum());
}
const double Analysis::Objects::getMotionalDirectionY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return acos(getMomentumY() / getMomentum());
}
const double Analysis::Objects::getMotionalDirectionZ() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return acos(getMomentumZ() / getMomentum());
}
void Analysis::Objects::resetEventData() {
  const int &n = this->getNumberOfObjects();
  const int &m = this->getNumberOfRealOrDummyObjects();
  for (int i = 0; i < n; i++) {
    this->setObjectMembers(i).resetEventData();
  }
  for (int i = n; i < m; i++) {
    this->setDummyObjectMembers(i).resetEventData();
  }
  return;
}
Analysis::Object &Analysis::Objects::setObjectMembers(const int &i) {
  assert(isRealObject(i));
  return *pObject[i];
}
Analysis::Object &Analysis::Objects::setDummyObjectMembers(const int &i) {
  assert(isDummyObject(i));
  return *pObject[i];
}
const double Analysis::Objects::getLocationX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeLength(getLocationX());
}
const double Analysis::Objects::getLocationY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeLength(getLocationY());
}
const double Analysis::Objects::getMomentumX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentumX());
}
const double Analysis::Objects::getMomentumY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentumY());
}
const double Analysis::Objects::getMomentumZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentumZ());
}
const double Analysis::Objects::getMomentum(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentum());
}
const double Analysis::Objects::getEnergy(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeEnergy(getEnergy());
}
const int &Analysis::Objects::getNumberOfRealOrDummyObjects() const {
  return numberOfHitsUsed;
}
const bool Analysis::Objects::isDummyObject(const int &i) const {
  const int &n = getNumberOfObjects();
  const int &m = getNumberOfRealOrDummyObjects();
  return (i >= n) && (i < m);
}
const bool Analysis::Objects::isRealObject(const int &i) const {
  const int &n = getNumberOfObjects();
  return (i >= 0) && (i < n);
}
const bool Analysis::Objects::existDeadRealOrDummyObject() const {
  return existDeadObject() || existDeadDummyObject();
}

const bool Analysis::Objects::existOutOfFrameOfBasicDataObject() const
{
	const int &n = getNumberOfObjects(); 
	bool b = false; 
	for (int i = 0; i < n; i++)
	{
		b = b || getObject(i).getFlag().isOutOfFrameOfBasicData(); 
	}
	return b; 
}

const bool Analysis::Objects::existOutOfFrameOfMomentumDataObject() const
{
	const int &n = getNumberOfObjects(); 
	bool b = false; 
	for (int i = 0; i < n; i++)
	{
		b = b || getObject(i).getFlag().isOutOfFrameOfMomentumData(); 
	}
	return b; 
}

const bool Analysis::Objects::existDeadObject() const {
  return getNumberOfDeadObjects() != 0;
}
const bool Analysis::Objects::areAllWithinMasterRegion() const {
  bool b = true;
  for(int i = 0; i < numberOfHits; i++) {
    b = b && getObject(i).getFlag().isWithinMasterRegion();
  }
  return b;
}
const int Analysis::Objects::getNumberOfDeadObjects() const {
  int n = 0;
  for (int i = 0; i < numberOfHits; i++) {
    if (getObject(i).getFlag().isDead()) {
      n += 1;
    }
  }
  return n;
}
const int Analysis::Objects::getNumberOfDeadDummyObjects() const {
  int n = 0;
  for (int i = numberOfHits; i < numberOfHitsUsed; i++) {
    if (getDummyObject(i).getFlag().isDead()) {
      n += 1;
    }
  }
  return n;
}
const int Analysis::Objects::getNumberOfDeadRealOrDummyObjects() const {
  return getNumberOfDeadObjects() + getNumberOfDeadDummyObjects();
}
const bool Analysis::Objects::existDeadDummyObject() const {
  return getNumberOfDeadDummyObjects() != 0;
}
const bool Analysis::Objects::areAllDeadObjects() const {
  return getNumberOfDeadObjects() == getNumberOfObjects();
}
const bool Analysis::Objects::areAllDeadDummyObjects() const {
  return getNumberOfDeadDummyObjects() == getNumberOfDeadObjects();
}
const bool Analysis::Objects::areAllDeadRealAndDummyObjects() const {
  return getNumberOfDeadRealOrDummyObjects() == getNumberOfRealOrDummyObjects();
}
const double Analysis::Objects::getMomentumXY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Objects::getMomentumXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentumXY());
}
const int Analysis::Objects::getNumberOfDummyObject() const {
  return getNumberOfRealOrDummyObjects()-getNumberOfObjects();
}
const double Analysis::Objects::getMomentumYZ() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumY(), 2e0) + pow(getMomentumZ(), 2e0),
             0.5e0);
}
const double Analysis::Objects::getMomentumZX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumZ(), 2e0) + pow(getMomentumX(), 2e0),
             0.5e0);
}
const double Analysis::Objects::getMotionalDirectionXY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumY(), getMomentumX());
}
const double Analysis::Objects::getMotionalDirectionYZ() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumZ(), getMomentumY());
}
const double Analysis::Objects::getMotionalDirectionZX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumX(), getMomentumZ());
}
const double Analysis::Objects::getMomentumYZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentumYZ());
}
const double Analysis::Objects::getMomentumZX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeMomentum(getMomentumZX());
}
const double Analysis::Objects::getMotionalDirectionX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionX());
}
const double Analysis::Objects::getMotionalDirectionY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionY());
}
const double Analysis::Objects::getMotionalDirectionZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionZ());
}
const double Analysis::Objects::getMotionalDirectionXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionXY());
}
const double Analysis::Objects::getMotionalDirectionYZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionYZ());
}
const double Analysis::Objects::getMotionalDirectionZX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree((getMotionalDirectionZX()));
}
const double Analysis::Objects::getSumOfTOF(const int &i1, const int &i2) const {
	if(getObject(i1).getFlag().isOutOfFrameOfBasicData() 
		|| getObject(i2).getFlag().isOutOfFrameOfBasicData()) 
	{
		return OUT_OF_FRAME; 
	}
  return getRealOrDummyObject(i1).getTOF() + getRealOrDummyObject(i2).getTOF();
}
const double Analysis::Objects::getSumOfTOF(const Analysis::Unit &unit,
                                            const int &i1,
                                            const int &i2) const {
	if(getObject(i1).getFlag().isOutOfFrameOfBasicData()
		|| getObject(i2).getFlag().isOutOfFrameOfBasicData())
	{
		return OUT_OF_FRAME; 
	}
  return unit.writeTime(getSumOfTOF(i1,i2));
}
const Analysis::Object &Analysis::Objects::getRealOrDummyObject(const int &i) const {
  const Object *pObject;
  if(isRealObject(i)) {
    pObject = &getObject(i);
  } else if(isDummyObject(i)) {
    pObject = &getDummyObject(i);
  } else {
    assert(false);
    pObject = new Object();
  }
  return *pObject;
}
Analysis::Object &Analysis::Objects::setRealOrDummyObjectMembers(const int &i) {
  Object *pObject;
  if(isRealObject(i)) {
    pObject = &setObjectMembers(i);
  } else if(isDummyObject(i)) {
    pObject = &setDummyObjectMembers(i);
  } else {
    assert(false);
    pObject = new Object();
  }
  return *pObject;
}

void Analysis::Objects::setAllOfObjectIsOutOfFrameOfBasicDataFlag()
{
	const int &n = getNumberOfObjects(); 
	for (int i = 0; i < n; i++)
	{
		setObjectMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag(); 
	}
}

void Analysis::Objects::setAllOfObjectIsOutOfFrameOfMomentumDatatFlag()
{
	const int &n = getNumberOfObjects(); 
	for (int i = 0; i < n; i++)
	{
		setObjectMembers(i).setFlagMembers().setOutOfFrameOfMomentumDataFlag(); 
	}
}

void Analysis::Objects::setAllOfDummyObjectIsOutOfFrameOfBasicDataFlag()
{
	const int &n = getNumberOfObjects(); 
	const int &m = getNumberOfDeadRealOrDummyObjects(); 
	for (int i = n; i < m; i++)
	{
		setDummyObjectMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag(); 
	}
}

void Analysis::Objects::setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag()
{
	const int &n = getNumberOfObjects(); 
	const int &m = getNumberOfDeadRealOrDummyObjects(); 
	for (int i = n; i < m; i++)
	{
		setDummyObjectMembers(i).setFlagMembers().setOutOfFrameOfMomentumDataFlag(); 
	}
}

void Analysis::Objects::setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag()
{
	setAllOfObjectIsOutOfFrameOfBasicDataFlag(); 
	setAllOfDummyObjectIsOutOfFrameOfBasicDataFlag(); 
}

void Analysis::Objects::setAllOfRealOrDummyObjectIsOutOfFrameOfMomentumDataFlag()
{
	setAllOfObjectIsOutOfFrameOfMomentumDatatFlag(); 
	setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag();
}

const bool Analysis::Objects::isRealOrDummyObject(const int &i) const {
  return isRealObject(i) || isDummyObject(i);
}
const double Analysis::Objects::getLocationXY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return pow(pow(getLocationX(),2e0) + pow(getLocationY(),2e0),0.5e0);
}
const double Analysis::Objects::getLocation() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return getLocationXY();
}
const double Analysis::Objects::getLocationalDirectionX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return acos(getLocationX()/getLocation());
}
const double Analysis::Objects::getLocationalDirectionY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return acos(getLocationY()/getLocation());
}
const double Analysis::Objects::getLocationalDirectionXY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return atan2(getLocationY(), getLocationX());
}
const double Analysis::Objects::getLocationalDirectionX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionX());
}
const double Analysis::Objects::getLocaitonalDirectionY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionY());
}
const double Analysis::Objects::getLocationalDirectionXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionXY());
}
const double Analysis::Objects::getLocationalDirectionYX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return atan2(getLocationX(), getLocationY());
}
const double Analysis::Objects::getLocationDirectionYX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionYX());
}
const double Analysis::Objects::getMotionalDirectionXZ() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumZ(), getMomentumX());
}
const double Analysis::Objects::getMotionalDirectionYX() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumX(), getMomentumY());
}
const double Analysis::Objects::getMotionalDirectionZY() const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumY(), getMomentumZ());
}
const double Analysis::Objects::getLocationXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeLength(getLocationXY());
}
const double Analysis::Objects::getLocation(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeLength(getLocation());
}
const double Analysis::Objects::getMotionalDirectionXZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionXZ());
}
const double Analysis::Objects::getMotionalDirectionYX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionYX());
}
const double Analysis::Objects::getMotionalDirectionZY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECTS_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionZY());
}
const int &Analysis::Objects::getMaximumOfHits() const {
  return maximumOfHits;
}
const int &Analysis::Objects::getNumberOfHits() const {
  return numberOfHits;
}
const int &Analysis::Objects::getNumberOfHitsUsed() const {
  return numberOfHitsUsed;
}
