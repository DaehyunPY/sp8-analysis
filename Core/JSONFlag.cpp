//
// Created by Daehyun You on 12/24/15.
//

#include "JSONFlag.h"
Analysis::JSONFlag::JSONFlag() {
  isInitial();
}
Analysis::JSONFlag::~JSONFlag() {
}
void Analysis::JSONFlag::isInitial() {
  setSign(isInitial_flagForSign);
}
void Analysis::JSONFlag::fileIsOpen() {
  set1stDigit(fileIsOpen_flagFor1stDigit);
}
void Analysis::JSONFlag::hasNoParseError() {
  set2ndDigit(hasNoParseError_flagFor2ndDigit);
}
const bool Analysis::JSONFlag::isInitial() const {
  return getSign() == isInitial_flagForSign;
}
const bool Analysis::JSONFlag::fileIsOpen() const {
  return get1stDigit() == fileIsOpen_flagFor1stDigit;
}
const bool Analysis::JSONFlag::hasNoParseError() const {
  return get2ndDigit() == hasNoParseError_flagFor2ndDigit;
}
void Analysis::JSONFlag::fileIsNotExist() {
  set1stDigit(fileIsNotExist_flagFor1stDigit);
}
void Analysis::JSONFlag::fileIsClosedAndDataIsSaved() {
  set1stDigit(fileIsClosedAndDataIsSaved_flagFor1stDigit);
}
void Analysis::JSONFlag::hasParseError() {
  set2ndDigit(hasParseError_flagFor2ndDigit);
}
const bool Analysis::JSONFlag::fileIsNotExist() const {
  return get1stDigit() == fileIsNotExist_flagFor1stDigit;
}
const bool Analysis::JSONFlag::fileIsClosedAndDataIsSaved() const {
  return get1stDigit() == fileIsClosedAndDataIsSaved_flagFor1stDigit;
}
const bool Analysis::JSONFlag::hasParseError() const {
  return get2ndDigit() == hasParseError_flagFor2ndDigit;
}












