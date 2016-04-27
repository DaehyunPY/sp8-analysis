//
// Created by Daehyun You on 11/27/15.
//

#include <cmath>
#include <assert.h>
#include "Flag.h"

Analysis::Flag::Flag() {
  flag = init;
}
Analysis::Flag::~Flag() {
}
const unsigned int Analysis::Flag::get1stDigit(const unsigned int f) const {
  return getNthDigit(1, f);
}
const unsigned int Analysis::Flag::get2ndDigit(const unsigned int f) const {
  return getNthDigit(2, f);
}
const unsigned int Analysis::Flag::get3rdDigit(const unsigned int f) const {
  return getNthDigit(3, f);
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

void Analysis::Flag::setFlag(const unsigned int f) {
  flag = f;
}

const int &Analysis::Flag::getFlag() const {
  return flag;
}
const unsigned int Analysis::Flag::getNthDigit(const unsigned int nth,
                                               const unsigned int f) const {
  assert(nth > 0);
  return (f / (unsigned int) pow(10, nth - 1)) % 10;
}
void Analysis::Flag::setNthDigit(const unsigned int nth,
                                 const unsigned int f1) {
  assert(nth > 0);
  assert(0 <= f1 && f1 < 10);
  const unsigned int f0 = getNthDigit(nth);
  flag += ((unsigned int) pow(10, nth - 1)) * (-f0 + f1);
}
const unsigned int Analysis::Flag::getNthNumDigit(const unsigned int nth,
                                                  const unsigned int num,
                                                  const unsigned int f) const {
  assert(nth > 0);
  assert(num > 0);
  return (f / (unsigned int) pow(10, nth - 1)) % ((unsigned int) pow(10, num));
}
void Analysis::Flag::setNthNumDigit(const unsigned int nth,
                                    const unsigned int num,
                                    const unsigned int f1) {
  assert(nth > 0);
  assert(num > 0);
  assert(0 <= f1 && f1 < (unsigned int) pow(10, num));
  const unsigned int f0 = getNthNumDigit(nth, num);
  flag += ((unsigned int) pow(10, nth - 1)) * (-f0 + f1);
}
const unsigned int Analysis::Flag::get1stDigit() const {
  return getNthDigit(1, flag);
}
const unsigned int Analysis::Flag::get2ndDigit() const {
  return getNthDigit(2, flag);
}
const unsigned int Analysis::Flag::get3rdDigit() const {
  return getNthDigit(3, flag);
}
const unsigned int Analysis::Flag::getNthDigit(const unsigned int nth) const {
  return getNthDigit(nth, flag);
}
const unsigned int Analysis::Flag::getNthNumDigit(const unsigned int nth,
                                                  const unsigned int num) const {
  return getNthNumDigit(nth, num, flag);
}
void Analysis::Flag::setInit() {
  flag = init;
}