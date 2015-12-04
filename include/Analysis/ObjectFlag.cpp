//
// Created by Daehyun You on 11/27/15.
//

#include "ObjectFlag.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP

Analysis::ObjectFlag::ObjectFlag() {
  loadInitialFlager();
  return;
}

Analysis::ObjectFlag::~ObjectFlag() {
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOAD

const int Analysis::ObjectFlag::getFlag() const {
  return this->flag;
}

void Analysis::ObjectFlag::loadInitialFlager() {
  setSign(this->flag, initialFlagForSign);
}

const bool Analysis::ObjectFlag::isInitialFlag() const {
  return getSign(this->flag) == initialFlagForSign;
}

void Analysis::ObjectFlag::loadValidOnFlager() {
  setSign(this->flag, validOnFlagForSign);
  return;
}

void Analysis::ObjectFlag::loadValidOffFlager() {
  setSign(this->flag, validOffFlagForSign);
  return;
}

const bool Analysis::ObjectFlag::isValidOnFlag() const {
  return getSign(this->flag) == validOnFlagForSign;
}

void Analysis::ObjectFlag::loadMasterOnFlager() {
  set1stDigit(this->flag, masterOnFlagFor1stDigit);
  return;
}

void Analysis::ObjectFlag::loadMasterOffFlager() {
  set1stDigit(this->flag, masterOffFlagFor1stDigit);
  return;
}

const bool Analysis::ObjectFlag::isMasterOnFlag() const {
  return get1stDigit(this->flag) == masterOnFlagFor1stDigit;
}
