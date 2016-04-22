//
// Created by Daehyun You on 12/1/15.
//

#ifndef ANALYSIS_EVENTDATAREADER_H
#define ANALYSIS_EVENTDATAREADER_H

#include <assert.h>
#include "EventDataFlag.h"
namespace Analysis {
class EventDataReader {
 private:
  const int numberOfTDCUsed;
  const int numberOfChannelsUsed;
  const int numberOfHitsUsed;
  double *pEventData;
  EventDataFlag flag; // todo: make flag
  const int tmpFlag = 0; // todo: delete it
  const int getAdressAt(const int &iTDC, const int &iCH, const int &iHit) const;

 public:
  EventDataReader(const int &nTDC, const int &nCH, const int &nHit);
  ~EventDataReader();
  void setEventDataAt
      (const int &iTDC, const int &iCH, const int &iHit, const double &d);
  const double
      &getEventDataAt(const int &iTDC, const int &iCH, const int &iHit) const;
  const int &getNumberOfTDCUsed() const;
  const int &getNumberOfChannelsUsed() const;
  const int &getNumberOfHitsUsed() const;
  const int &getTmpFlag() const;
};
}

#endif //ANALYSIS_LMFREADER_H
