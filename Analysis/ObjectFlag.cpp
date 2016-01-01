//
// Created by Daehyun You on 11/27/15.
//

#include "ObjectFlag.h"

Analysis::ObjectFlag::ObjectFlag() : Flag() {
  resetFlag();
  return;
}
Analysis::ObjectFlag::~ObjectFlag() { }
void Analysis::ObjectFlag::resetFlag() {
  setFlag(initialFlag);
}

void Analysis::ObjectFlag::setWithinMasterRegionFlag() 
{
	if (get1stDigit() < withinMasterRegionFlagFor1stDigit)
	{
		set1stDigit(withinMasterRegionFlagFor1stDigit);
	}
}

void Analysis::ObjectFlag::setNotWithinMasterRegionFlag()
{
	if (get1stDigit() < notWithinMasterRegionFlagFor1stDigit)
	{
		set1stDigit(notWithinMasterRegionFlagFor1stDigit);
	}
}

void Analysis::ObjectFlag::setDeadFlag()
{
	if (get1stDigit() < deadFlagFor1stDigit)
	{
		set1stDigit(deadFlagFor1stDigit);
	}
}

void Analysis::ObjectFlag::setInFrameOfAllDataFlag()
{
	if (get2ndDigit() < inFrameOfAllDataFlagFor2ndDigit)
	{
		set2ndDigit(inFrameOfAllDataFlagFor2ndDigit);
	}
}

void Analysis::ObjectFlag::setOutOfFrameOfMomentumDataFlag()
{
	if (get2ndDigit() < outOfFrameOfMomentumDataFlagFor2ndDigit)
	{
		set2ndDigit(outOfFrameOfMomentumDataFlagFor2ndDigit);
	}
}

void Analysis::ObjectFlag::setOutOfFrameOfBasicDataFlag()
{
	if (get2ndDigit() < outOfFrameOfBasicDataFlagFor2ndDigit)
	{
		set2ndDigit(outOfFrameOfBasicDataFlagFor2ndDigit);
	}
}

const bool Analysis::ObjectFlag::isInitial() const {
  return getFlag() == initialFlag;
}
void Analysis::ObjectFlag::forceToSetOutOfFrameOfMomentumDataFlag() {
  set2ndDigit(outOfFrameOfMomentumDataFlagFor2ndDigit);
}

void Analysis::ObjectFlag::forceToSetInFrameOfAllDataFlag() {
  set2ndDigit(inFrameOfAllDataFlagFor2ndDigit);
}

void Analysis::ObjectFlag::forceToSetOutOfFrameOfBasicDataFlag() {
  set2ndDigit(outOfFrameOfBasicDataFlagFor2ndDigit);
}
const bool Analysis::ObjectFlag::isInFrameOfAllData() const {
  return get2ndDigit() == inFrameOfAllDataFlagFor2ndDigit;
}
void Analysis::ObjectFlag::forceToSetWithinMasterRegionFlag() {
  set1stDigit(withinMasterRegionFlagFor1stDigit);
}
void Analysis::ObjectFlag::forceToSetNotWithinMasterRegionFlag() {
  set1stDigit(notWithinMasterRegionFlagFor1stDigit);
}
const bool Analysis::ObjectFlag::isWithinMasterRegion() const {
  return get1stDigit() == withinMasterRegionFlagFor1stDigit;
}
void Analysis::ObjectFlag::forceToSetDeadFlag() {
  set1stDigit(deadFlagFor1stDigit);
}
const bool Analysis::ObjectFlag::isDead() const {
  return get1stDigit() == deadFlagFor1stDigit;
}
const bool Analysis::ObjectFlag::isOutOfFrameOfBasicData() const {
  return get2ndDigit() == outOfFrameOfBasicDataFlagFor2ndDigit;
}

const bool Analysis::ObjectFlag::isOutOfFrameOfMomentumData() const {
  return get2ndDigit() == outOfFrameOfMomentumDataFlagFor2ndDigit;
}

const bool Analysis::ObjectFlag::isNotWithinMasterRegion() const {
  return get1stDigit() == notWithinMasterRegionFlagFor1stDigit;
}
