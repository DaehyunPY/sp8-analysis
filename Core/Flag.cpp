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
const int Analysis::Flag::getSign(const int f) const {
  if (f == 0) {
	return 0;
  } else if (f > 0) {
	return 1;
  } else { return -1; }
}
const int Analysis::Flag::get1stDigit(const int f) const {
  return getNthDigit(1, f);
}
const int Analysis::Flag::get2ndDigit(const int f) const {
  return getNthDigit(2, f);
}
const int Analysis::Flag::get3rdDigit(const int f) const {
  return getNthDigit(3, f);
}
void Analysis::Flag::setSign(int f1) {
  f1 = getSign(f1);
  const int f0 = getSign();
  flag *= f0 * f1;
}
void Analysis::Flag::set1stDigit(const unsigned int f1) {
  setNthDigit(1, f1);
}
void Analysis::Flag::set2ndDigit(const unsigned int f1) {
  setNthDigit(2, f1);
}
void Analysis::Flag::set3ndDigit(const unsigned int f1) {
  setNthDigit(3, f1);
}

void Analysis::Flag::setFlag(const int f) {
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
const unsigned int Analysis::Flag::getNthDigit(const unsigned int nth,
                                               const int f) const {
  assert(nth > 0);
  const unsigned int unsignedFlag = (unsigned int) abs(f);
  return (unsignedFlag / (unsigned int) pow(10, nth - 1)) % 10;
}
const unsigned int Analysis::Flag::getNthDigit(const unsigned int nth) const {
  return getNthDigit(nth, flag);
}
void Analysis::Flag::setNthDigit(const unsigned int nth,
                                 const unsigned int f1) {
  assert(nth > 0);
  assert(0 <= f1 && f1 < 10);
  const unsigned int f0 = getNthDigit(nth);
  int sign = getSign();
  if(sign == 0) { sign = 1; }
  flag += sign * ((unsigned int) pow(10, nth - 1)) * (-f0 + f1)
  ;
}
const unsigned int Analysis::Flag::getNthNumDigit(const unsigned int nth,
                                                  const int num,
                                                  const int f) const {
  assert(nth > 0);
  assert(num > 0);
  const unsigned int unsignedFlag = (const unsigned int) abs(f);
  return (unsignedFlag / (unsigned int) pow(10, nth - 1)) % ((unsigned int) pow(10, num));
}
const unsigned int Analysis::Flag::getNthNumDigit(const unsigned int nth,
                                                  const int num) const {
  return getNthNumDigit(nth, num, flag);
}
void Analysis::Flag::setNthNumDigit(const unsigned int nth,
                                    const int num,
                                    const int f1) {
  assert(nth > 0);
  assert(num > 0);
  assert(0 <= f1 && f1 < (unsigned int) pow(10, num));
  const unsigned int f0 = getNthNumDigit(nth, num);
  int sign = getSign();
  if(sign == 0) { sign = 1; }
  flag += sign * ((unsigned int) pow(10, nth - 1)) * (-f0 + f1);
}






