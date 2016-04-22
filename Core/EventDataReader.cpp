//
// Created by Daehyun You on 12/1/15.
//

#include "EventDataReader.h"

Analysis::EventDataReader::EventDataReader(const int &nTDC,
                                           const int &nCH,
                                           const int &nHit)
    : numberOfTDCUsed(nTDC), numberOfChannelsUsed(nCH), numberOfHitsUsed(nHit) {
  pEventData = new double[getNumberOfTDCUsed() * getNumberOfChannelsUsed()
      * getNumberOfHitsUsed()];
}
Analysis::EventDataReader::~EventDataReader() {
  delete[] pEventData;
}
const double &Analysis::EventDataReader::getEventDataAt(const int &iTDC,
                                                        const int &iCH,
                                                        const int &iHit) const {
  return pEventData[getAdressAt(iTDC, iCH, iHit)];
}
const int &Analysis::EventDataReader::getNumberOfTDCUsed() const {
  return numberOfTDCUsed;
}
const int &Analysis::EventDataReader::getNumberOfChannelsUsed() const {
  return numberOfChannelsUsed;
}
const int &Analysis::EventDataReader::getNumberOfHitsUsed() const {
  return numberOfHitsUsed;
}

const int& Analysis::EventDataReader::getTmpFlag() const
{
  return tmpFlag;
}
void Analysis::EventDataReader::setEventDataAt(const int &iTDC,
                                               const int &iCH,
                                               const int &iHit,
                                               const double &d) {
  pEventData[getAdressAt(iTDC, iCH, iHit)] = d;
}
const int Analysis::EventDataReader::getAdressAt(const int &iTDC,
                                                 const int &iCH,
                                                 const int &iHit) const {
  const int &nTDC = getNumberOfTDCUsed();
  const int &nCH = getNumberOfChannelsUsed();
  const int &nHit = getNumberOfHitsUsed();
  assert(iTDC >= 0 && iTDC < nTDC);
  assert(iCH >= 0 && iCH < nCH);
  assert(iHit >= 0 && iHit < nHit);
  return iTDC + nTDC * iCH + nTDC * nCH * iHit;
}
