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
  return this->_flag;
}

void Analysis::ObjectFlag::loadInitialFlager() {
  setSign(this->_flag, _initialFlagForSign);
}

const bool Analysis::ObjectFlag::isInitialFlag() const {
  return getSign(this->_flag) == _initialFlagForSign;
}

void Analysis::ObjectFlag::loadValidOnFlager() {
  setSign(this->_flag, _validOnFlagForSign);
  return;
}

void Analysis::ObjectFlag::loadValidOffFlager() {
  setSign(this->_flag, _validOffFlagForSign);
  return;
}

const bool Analysis::ObjectFlag::isValidOnFlag() const {
  return getSign(this->_flag) == _validOnFlagForSign;
}

void Analysis::ObjectFlag::loadMasterOnFlager() {
  set1stDigit(this->_flag, _masterOnFlagFor1stDigit);
  return;
}

void Analysis::ObjectFlag::loadMasterOffFlager() {
  set1stDigit(this->_flag, _masterOffFlagFor1stDigit);
  return;
}

const bool Analysis::ObjectFlag::isMasterOnFlag() const {
  return get1stDigit(this->_flag) == _masterOnFlagFor1stDigit;
}
