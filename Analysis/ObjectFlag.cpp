//
// Created by Daehyun You on 11/27/15.
//

#include "ObjectFlag.h"

Analysis::ObjectFlag::ObjectFlag() : Flag() {
  loadInitialFlager();
  return;
}
Analysis::ObjectFlag::~ObjectFlag() {
  return;
}
void Analysis::ObjectFlag::loadInitialFlager() {
  setSign(initialFlagForSign);
}
const bool Analysis::ObjectFlag::isInitial() const {
  return getSign() == initialFlagForSign;
}
void Analysis::ObjectFlag::loadValidFlager() {
  setSign(validFlagForSign);
  return;
}
void Analysis::ObjectFlag::loadInvalidFlager() {
  setSign(invalidFlagForSign);
  return;
}
const bool Analysis::ObjectFlag::isValid() const {
  return getSign() == validFlagForSign;
}
void Analysis::ObjectFlag::loadWithinMasterRegionFlager() {
  set1stDigit(withinMasterRegionFlagFor1stDigit);
  return;
}
void Analysis::ObjectFlag::loadNotWithinMasterRegionFlager() {
  set1stDigit(notWithinMasterRegionFlagFor1stDigit);
  return;
}
const bool Analysis::ObjectFlag::isWithinMasterRegion() const {
  return get1stDigit() == withinMasterRegionFlagFor1stDigit;
}
void Analysis::ObjectFlag::loadDeadFlager() {
  set1stDigit(deadFlagFor1stDigit);
  return;
}
const bool Analysis::ObjectFlag::isDead() const {
  return get1stDigit() == deadFlagFor1stDigit;
}
const bool Analysis::ObjectFlag::isInvalid() const {
  return getSign() == invalidFlagForSign;
}
const bool Analysis::ObjectFlag::isNotWithinMasterRegion() const {
  return get1stDigit() == notWithinMasterRegionFlagFor1stDigit;
}
