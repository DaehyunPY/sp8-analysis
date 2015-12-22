//
// Created by Daehyun You on 12/1/15.
//

#include "EventDataReader.h"

Analysis::EventDataReader::EventDataReader(const EventData &eventData)
    : eventData(eventData) {
  return;
}
Analysis::EventDataReader::~EventDataReader() { return; }
const double Analysis::EventDataReader::getAt(const int &iTDC,
                                              const int &iCH,
                                              const int &iHit) const {
  const int &nTDC = numberOfTDCUsed;
  const int &nCH = numberOfChannelsUsed;
  const int &nHit = numberOfHitsUsed;
  assert(iTDC >= 0 && iTDC < nTDC);
  assert(iCH >= 0 && iCH < nCH);
  assert(iHit >= 0 && iHit < nHit);
  return eventData.array[iTDC][iCH][iHit];
}
const int &Analysis::EventDataReader::getNumberOfTDCUsed() const {
  return this->numberOfTDCUsed;
}
const int &Analysis::EventDataReader::getNumberOfChannelsUsed() const {
  return this->numberOfChannelsUsed;
}
const int &Analysis::EventDataReader::getNumberOfHitsUsed() const {
  return this->numberOfHitsUsed;
}
