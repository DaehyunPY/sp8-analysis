//
// Created by Daehyun You on 12/24/15.
//

#ifndef ANALYSIS_JSONFLAG_H
#define ANALYSIS_JSONFLAG_H


#include "Flag.h"
namespace Analysis {
class JSONFlag : Flag {
 private:
  const int isInitial_flagForSign = 0;
  const int fileIsNotExist_flagFor1stDigit = 1;
  const int fileIsOpen_flagFor1stDigit = 2;
  const int fileIsClosedAndDataIsSaved_flagFor1stDigit = 3;
  const int hasParseError_flagFor2ndDigit = 1;
  const int hasNoParseError_flagFor2ndDigit = 2;
 public:
  JSONFlag();
  ~JSONFlag();
  void isInitial();
  void fileIsNotExist();
  void fileIsOpen();
  void fileIsClosedAndDataIsSaved();
  void hasParseError();
  void hasNoParseError();
  const bool isInitial() const;
  const bool fileIsNotExist() const;
  const bool fileIsOpen() const;
  const bool fileIsClosedAndDataIsSaved() const;
  const bool hasParseError() const;
  const bool hasNoParseError() const;
};
}


#endif //BL17_ANALYSIS_JSONFLAG_H
