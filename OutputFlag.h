//
// Created by Daehyun You on 4/22/16.
//

#ifndef ANALYSIS_OUTPUTFLAG_H
#define ANALYSIS_OUTPUTFLAG_H


#include "Core/Flag.h"

namespace Analysis {
class OutputFlag: protected Flag {
  const int flagFor2ndDigit_sendingOutOfFrame = 1;
  const int flagFor3rdDigit_showingOnlyMasterRegionEvents = 1;

 public:
  OutputFlag();
  ~OutputFlag();
  void setSendingOutOfFrame();
  const bool isSendingOutOfFrame() const;
  void setShowingOnlyMasterRegionEvents();
  const bool isShowingOnlyMasterRegionEvents() const;
};
}


#endif //ANALYSIS_OUTPUTFLAG_H
