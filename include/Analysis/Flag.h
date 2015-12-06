//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_FLAG_H
#define ANALYSIS_FLAG_H


#include <stdlib.h>

namespace Analysis {
class Flag {
 protected:
  Flag();
  virtual ~Flag();
  const int getSign(const int &) const;
  const int get1stDigit(const int &) const;
  const int get2ndDigit(const int &) const;
  const int get3rdDigit(const int &) const;
  void setSign(int &, const int &) const;
  void set1stDigit(int &, const int &) const;
  void set2ndDigit(int &, const int &) const;
  void set3ndDigit(int &, const int &) const;
};
}

#endif //ANALYSIS_FLAG_H
