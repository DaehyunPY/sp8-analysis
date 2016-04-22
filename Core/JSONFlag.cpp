//
// Created by Daehyun You on 12/24/15.
//

#include "JSONFlag.h"
Analysis::JSONFlag::JSONFlag() {
  loadInitialFlager();
  return;
}
Analysis::JSONFlag::~JSONFlag() {
  return;
}
void Analysis::JSONFlag::loadInitialFlager() {
  setSign(initialFlagForSign);
  return;
}
void Analysis::JSONFlag::loadIsOpenFlager() {
  set1stDigit(isOpenFlagFor1stDigit);
  return;
}
void Analysis::JSONFlag::loadHasNoParseErrorFlager() {
  set2ndDigit(hasNoParseErrorFor2ndDigit);
  return;
}
const bool Analysis::JSONFlag::isInitial() const {
  return getSign() == initialFlagForSign;
}
const bool Analysis::JSONFlag::isOpen() const {
  return get1stDigit() == isOpenFlagFor1stDigit;
}
const bool Analysis::JSONFlag::hasNoParseError() const {
  return get2ndDigit() == hasNoParseErrorFor2ndDigit;
}
