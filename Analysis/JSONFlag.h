//
// Created by Daehyun You on 12/24/15.
//

#ifndef BL17_ANALYSIS_JSONFLAG_H
#define BL17_ANALYSIS_JSONFLAG_H


#include "Flag.h"
namespace Analysis {
class JSONFlag : Flag {
 private:
  const int initialFlagForSign = 0;
  const int isOpenFlagFor1stDigit = 1;
  const int hasNoParseErrorFor2ndDigit = 1;
 public:
  JSONFlag();
  ~JSONFlag();
  void loadInitialFlager();
  void loadIsOpenFlager();
  void loadHasNoParseErrorFlager();
  const bool isInitial() const;
  const bool isOpen() const;
  const bool hasNoParseError() const;
};
}


#endif //BL17_ANALYSIS_JSONFLAG_H
