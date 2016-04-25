//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_FLAG_H
#define ANALYSIS_FLAG_H


#include <stdlib.h>

namespace Analysis {
class Flag {
  int flag;

 protected:
  Flag();
  virtual ~Flag();
  const int getSign(const int f) const;
  const int get1stDigit(const int f) const;
  const int get2ndDigit(const int f) const;
  const int get3rdDigit(const int f) const;
  const unsigned int getNthDigit(const unsigned int nth, const int f) const;
  const unsigned int getNthNumDigit
      (const unsigned int nth, const int num, const int f) const;
  const int getSign() const;
  const int get1stDigit() const;
  const int get2ndDigit() const;
  const int get3rdDigit() const;
  const unsigned int getNthDigit(const unsigned int nth) const;
  const unsigned int getNthNumDigit(const unsigned int nth, const int num) const;
  void setSign(int f1);
  void set1stDigit(const unsigned int f1);
  void set2ndDigit(const unsigned int f1);
  void set3ndDigit(const unsigned int f1);
  void setNthDigit(const unsigned int nth, const unsigned int f1);
  void setNthNumDigit
      (const unsigned int nth, const int num, const int f1);
  void setFlag(const int f);

 public:
  const int &getFlag() const;
};
}

#endif //ANALYSIS_FLAG_H
