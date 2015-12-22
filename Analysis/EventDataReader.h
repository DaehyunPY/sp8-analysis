//
// Created by Daehyun You on 12/1/15.
//

#ifndef ANALYSIS_EVENTDATAREADER_H
#define ANALYSIS_EVENTDATAREADER_H

#define NUMBER_OF_TDC_USED 3
#define NUMBER_OF_CHANNELS_USED 2
#define NUMBER_OF_HITS_USED 4
#include <assert.h>

namespace Analysis {

struct EventData {
  const int d1 = NUMBER_OF_TDC_USED;
  const int d2 = NUMBER_OF_CHANNELS_USED;
  const int d3 = NUMBER_OF_HITS_USED;
  const double array[NUMBER_OF_TDC_USED][NUMBER_OF_CHANNELS_USED]
  [NUMBER_OF_HITS_USED];
};

class EventDataReader {
 private:
  const int numberOfTDCUsed = NUMBER_OF_TDC_USED;
  const int numberOfChannelsUsed = NUMBER_OF_CHANNELS_USED;
  const int numberOfHitsUsed = NUMBER_OF_HITS_USED;
  const EventData eventData;

 public:
  EventDataReader(const EventData &);
  ~EventDataReader();
  const double getAt(const int &iTDC, const int &iCH, const int &iHit) const;
  const int &getNumberOfTDCUsed() const;
  const int &getNumberOfChannelsUsed() const;
  const int &getNumberOfHitsUsed() const;
};
}

#endif //ANALYSIS_LMFREADER_H
