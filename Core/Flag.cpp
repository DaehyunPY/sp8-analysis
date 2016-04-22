//
// Created by Daehyun You on 11/27/15.
//

#include <cmath>
#include <assert.h>
#include "Flag.h"

Analysis::Flag::Flag() {
  flag = 0;
}
Analysis::Flag::~Flag() {
}
const int Analysis::Flag::getSign(const int &f) const {
  if (f == 0) {
	return 0;
  } else if (f > 0) {
	return 1;
  } else { return -1; }
}
const int Analysis::Flag::get1stDigit(const int &f) const {
  const int unsignedFlag = abs(f);
  return unsignedFlag % 10;
}
const int Analysis::Flag::get2ndDigit(const int &f) const {
  const int unsignedFlag = abs(f);
  return (unsignedFlag / 10) % 10;
}
const int Analysis::Flag::get3rdDigit(const int &f) const {
  const int unsignedFlag = abs(f);
  return (unsignedFlag / 100) % 10;
}
void Analysis::Flag::setSign(const int &f1) {
  const int f0 = getSign();
  flag = flag * f0 * f1;
}
void Analysis::Flag::set1stDigit(const int &f1) {
  const int f0 = get1stDigit();
  flag = flag - f0 + f1;
}
void Analysis::Flag::set2ndDigit(const int &f1) {
  const int f0 = get2ndDigit();
  flag = flag + 10 * (-f0 + f1);
}
void Analysis::Flag::set3ndDigit(const int &f1) {
  const int f0 = get3rdDigit();
  flag = flag + 100 * (-f0 + f1);
}

void Analysis::Flag::setFlag(const int &f) {
  flag = f;
}

const int &Analysis::Flag::getFlag() const {
  return flag;
}
const int Analysis::Flag::get1stDigit() const {
  return get1stDigit(flag);
}
const int Analysis::Flag::get2ndDigit() const {
  return get2ndDigit(flag);
}
const int Analysis::Flag::get3rdDigit() const {
  return get3rdDigit(flag);
}
const int Analysis::Flag::getSign() const {
  return getSign(flag);
}
const unsigned int Analysis::Flag::getNthDigit(const unsigned int &nth,
                                               const int &f) const {
  assert(nth > 0);
  const unsigned int unsignedFlag = (const unsigned int) abs(f);
  return (unsignedFlag / (unsigned int) pow(10, nth - 1)) % 10;
}
const unsigned int Analysis::Flag::getNthDigit(const unsigned int &nth) const {
  return getNthDigit(nth, flag);
}
void Analysis::Flag::setNthDigit(const unsigned int &nth,
                                 const unsigned int &f1) {
  assert(nth > 0);
  const unsigned int f0 = getNthDigit(nth, f1);
  flag = flag + ((unsigned int) pow(10, nth)) * (-f0 + f1);
}

