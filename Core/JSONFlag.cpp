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
  setSign(flagForSign_isInitial);
}
void Analysis::JSONFlag::fileIsOpen() {
  set1stDigit(flagFor1stDigit_fileIsOpen);
}
void Analysis::JSONFlag::hasNoParseError() {
  set2ndDigit(flagFor2ndDigit_hasNoParseError);
}
const bool Analysis::JSONFlag::isInitial() const {
  return getSign() == flagForSign_isInitial;
}
const bool Analysis::JSONFlag::fileIsOpen() const {
  return get1stDigit() == flagFor1stDigit_fileIsOpen;
}
const bool Analysis::JSONFlag::hasNoParseError() const {
  return get2ndDigit() == flagFor2ndDigit_hasNoParseError;
}
void Analysis::JSONFlag::fileIsNotExist() {
  set1stDigit(flagFor1stDigit_fileIsNotExist);
}
void Analysis::JSONFlag::fileIsClosedAndDataIsSaved() {
  set1stDigit(flagFor1stDigit_fileIsClosedAndDataIsSaved);
}
void Analysis::JSONFlag::hasParseError() {
  set2ndDigit(flagFor2ndDigit_hasParseError);
}
const bool Analysis::JSONFlag::fileIsNotExist() const {
  return get1stDigit() == flagFor1stDigit_fileIsNotExist;
}
const bool Analysis::JSONFlag::fileIsClosedAndDataIsSaved() const {
  return get1stDigit() == flagFor1stDigit_fileIsClosedAndDataIsSaved;
}
const bool Analysis::JSONFlag::hasParseError() const {
  return get2ndDigit() == flagFor2ndDigit_hasParseError;
}












