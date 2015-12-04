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
const int Analysis::Flag::getSign(const int &_flag) const {
  if (_flag == 0) { return 0; }
  else if (_flag > 0) { return 1; }
  else { return -1; }
}
const int Analysis::Flag::get1stDigit(const int &_flag) const {
  const int _abs = abs(_flag);
  return _abs % 10;
}
const int Analysis::Flag::get2ndDigit(const int &_flag) const {
  const int _abs = abs(_flag);
  return (_abs / 10) % 10;
}
const int Analysis::Flag::get3rdDigit(const int &_flag) const {
  const int _abs = abs(_flag);
  return (_abs / 100) % 10;
}
void Analysis::Flag::setSign(int &_flag, const int &_f1) const {
  const int _f0 = this->getSign(_flag);
  _flag = _flag * _f0 * _f1;
  return;
}
void Analysis::Flag::set1stDigit(int &_flag, const int &_f1) const {
  const int _f0 = this->get1stDigit(_flag);
  _flag = _flag - _f0 + _f1;
  return;
}
void Analysis::Flag::set2ndDigit(int &_flag, const int &_f1) const {
  const int _f0 = this->get2ndDigit(_flag);
  _flag = _flag + 10 * (-_f0 + _f1);
  return;
}
void Analysis::Flag::set3ndDigit(int &_flag, const int &_f1) const {
  const int _f0 = this->get3rdDigit(_flag);
  _flag = _flag + 100 * (-_f0 + _f1);
  return;
}
