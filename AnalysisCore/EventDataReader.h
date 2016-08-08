//
// Created by Daehyun You on 12/1/15.
//

#ifndef ANALYSIS_EVENTDATAREADER_H
#define ANALYSIS_EVENTDATAREADER_H

#include <assert.h>
#include <string>

namespace Analysis {
class EventDataReader {
 private:
  const int maxNumOfIonHits;
  const int maxNumOfElecHits;
  double *pEventData;
  int *pFlagData;
  const int getAdressAt(const int i, const std::string str) const;

 public:
  EventDataReader(const int iNum, const int eNum);
  ~EventDataReader();
  double &setEventDataAt(const int i, std::string str);
  int &setFlagDataAt(const int i, std::string str);
  const double &getEventDataAt(const int i, const std::string str) const;
  const int &getFlagDataAt(const int i, const std::string str) const;
};
}

#endif //ANALYSIS_LMFREADER_H
