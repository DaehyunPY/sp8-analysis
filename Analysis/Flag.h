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
  const int getSign() const;
  const int get1stDigit() const;
  const int get2ndDigit() const;
  const int get3rdDigit() const;
  void setSign(const int &);
  void set1stDigit(const int &);
  void set2ndDigit(const int &);
  void set3ndDigit(const int &);
  void setFlag(const int &);

 public:
  const int &getFlag() const;
};
}

#endif //ANALYSIS_FLAG_H
