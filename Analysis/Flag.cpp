//
// Created by Daehyun You on 11/27/15.
//

#include "Flag.h"

Analysis::Flag::Flag() {
  flag = 0;
}
Analysis::Flag::~Flag() {
}
const int Analysis::Flag::getSign() const {
  if (flag == 0) { return 0; }
  else if (flag > 0) { return 1; }
  else { return -1; }
}
const int Analysis::Flag::get1stDigit() const {
  const int unsignedFlag = abs(flag);
  return unsignedFlag % 10;
}
const int Analysis::Flag::get2ndDigit() const {
  const int unsignedFlag = abs(flag);
  return (unsignedFlag / 10) % 10;
}
const int Analysis::Flag::get3rdDigit() const {
  const int unsignedFlag = abs(flag);
  return (unsignedFlag / 100) % 10;
}
void Analysis::Flag::setSign(const int &f1) {
  const int f0 = getSign();
  flag = flag * f0 * f1;
  return;
}
void Analysis::Flag::set1stDigit(const int &f1) {
  const int f0 = get1stDigit();
  flag = flag - f0 + f1;
  return;
}
void Analysis::Flag::set2ndDigit(const int &f1) {
  const int f0 = get2ndDigit();
  flag = flag + 10 * (-f0 + f1);
  return;
}
void Analysis::Flag::set3ndDigit(const int &f1) {
  const int f0 = get3rdDigit();
  flag = flag + 100 * (-f0 + f1);
  return;
}
const int &Analysis::Flag::getFlag() const {
  return flag;
}
