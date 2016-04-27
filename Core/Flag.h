//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_FLAG_H
#define ANALYSIS_FLAG_H


#include <stdlib.h>

namespace Analysis {
class Flag {
  unsigned int flag;
  const unsigned int init = 0;

 protected:
  Flag();
  virtual ~Flag();
  void setInit();
  const unsigned int get1stDigit(const unsigned int f) const;
  const unsigned int get1stDigit() const;
  const unsigned int get2ndDigit(const unsigned int f) const;
  const unsigned int get2ndDigit() const;
  const unsigned int get3rdDigit(const unsigned int f) const;
  const unsigned int get3rdDigit() const;
  const unsigned int getNthDigit(const unsigned int nth, const unsigned int f) const;
  const unsigned int getNthDigit(const unsigned int nth) const;
  const unsigned int getNthNumDigit
      (const unsigned int nth, const unsigned int num, const unsigned int f) const;
  const unsigned int getNthNumDigit
      (const unsigned int nth, const unsigned int num) const;
  void set1stDigit(const unsigned int f1);
  void set2ndDigit(const unsigned int f1);
  void set3ndDigit(const unsigned int f1);
  void setNthDigit(const unsigned int nth, const unsigned int f1);
  void setNthNumDigit
      (const unsigned int nth, const unsigned int num, const unsigned int f1);
  void setFlag(const unsigned int f);

 public:
  const int &getFlag() const;
};
}

#endif //ANALYSIS_FLAG_H
