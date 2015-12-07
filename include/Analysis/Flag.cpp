//
// Created by Daehyun You on 11/27/15.
//

#include "Flag.h"

Analysis::Flag::Flag() {
  return;
}
Analysis::Flag::~Flag() {
  return;
}
const int Analysis::Flag::getSign(const int &flag) const {
  if (flag == 0) { return 0; }
  else if (flag > 0) { return 1; }
  else { return -1; }
}
const int Analysis::Flag::get1stDigit(const int &flag) const {
  const int unsignedFlag = abs(flag);
  return unsignedFlag % 10;
}
const int Analysis::Flag::get2ndDigit(const int &flag) const {
  const int unsignedFlag = abs(flag);
  return (unsignedFlag / 10) % 10;
}
const int Analysis::Flag::get3rdDigit(const int &flag) const {
  const int unsignedFlag = abs(flag);
  return (unsignedFlag / 100) % 10;
}
void Analysis::Flag::setSign(int &flag, const int &f1) const {
  const int f0 = this->getSign(flag);
  flag = flag * f0 * f1;
  return;
}
void Analysis::Flag::set1stDigit(int &flag, const int &f1) const {
  const int f0 = this->get1stDigit(flag);
  flag = flag - f0 + f1;
  return;
}
void Analysis::Flag::set2ndDigit(int &flag, const int &f1) const {
  const int f0 = this->get2ndDigit(flag);
  flag = flag + 10 * (-f0 + f1);
  return;
}
void Analysis::Flag::set3ndDigit(int &flag, const int &f1) const {
  const int f0 = this->get3rdDigit(flag);
  flag = flag + 100 * (-f0 + f1);
  return;
}
