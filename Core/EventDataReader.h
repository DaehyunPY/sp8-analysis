//
// Created by Daehyun You on 12/1/15.
//

#ifndef ANALYSIS_EVENTDATAREADER_H
#define ANALYSIS_EVENTDATAREADER_H

#include <assert.h>
#include <string>
#include "EventDataFlag.h"

namespace Analysis {
class EventDataReader {
 private:
  const int numberOfData;
  double *pEventData;
  EventDataFlag flag; // TODO: Make flag.
  const int tmpFlag = 0; // TODO: Delete it.
  const int getAdressAt(const int i, const std::string str) const;

 public:
  EventDataReader(const int n);
  EventDataReader(const int numOfHit, const int numPerAHit);
  ~EventDataReader();
  double &setEventDataAt(const int i, std::string str);
  const double &getEventDataAt(const int i, const std::string str) const;
  const int &getTmpFlag() const;
};
}

#endif //ANALYSIS_LMFREADER_H
