//
// Created by Daehyun You on 12/24/15.
//

#ifndef ANALYSIS_JSONFLAG_H
#define ANALYSIS_JSONFLAG_H

#include "../Protocols/Flag.h"

namespace Analysis {
class JSONFlag : Flag {
 private:
  const int flagFor1stDigit_fileIsNotExist = 1;
  const int flagFor1stDigit_fileIsOpen = 2;
  const int flagFor1stDigit_fileIsClosedAndDataIsSaved = 3;
  const int flagFor2ndDigit_hasParseError = 1;
  const int flagFor2ndDigit_hasNoParseError = 2;
 public:
  JSONFlag();
  ~JSONFlag();
  void fileIsNotExist();
  void fileIsOpen();
  void fileIsClosedAndDataIsSaved();
  void hasParseError();
  void hasNoParseError();
  const bool fileIsNotExist() const;
  const bool fileIsOpen() const;
  const bool fileIsClosedAndDataIsSaved() const;
  const bool hasParseError() const;
  const bool hasNoParseError() const;
};
}


#endif //BL17_ANALYSIS_JSONFLAG_H
