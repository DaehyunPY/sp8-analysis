//
// Created by Daehyun You on 12/1/15.
//

#include "EventDataReader.h"

Analysis::EventDataReader::EventDataReader(const int n) : numberOfData(n) {
  pEventData = new double[numberOfData];
}
Analysis::EventDataReader::~EventDataReader() {
  delete[] pEventData;
}
const int& Analysis::EventDataReader::getTmpFlag() const
{
  return tmpFlag;
}
Analysis::EventDataReader::EventDataReader(const int numOfHit, const int numPerAHit)
    : EventDataReader(numPerAHit*numOfHit){
}
const int Analysis::EventDataReader::getAdressAt(const int i, const std::string str) const {
  int adress = 0;
  if(str.find("Elec") != std::string::npos) {
    adress += 3;
  } else if(str.find("Ion") != std::string::npos) {
    adress += 0;
  } else {
    assert(false);
  }
  if(str.find("T") != std::string::npos) {
    adress += 0;
  } else if(str.find("X") != std::string::npos) {
    adress += 1;
  } else if(str.find("Y") != std::string::npos) {
    adress += 2;
  } else {
    assert(false);
  }
  adress += 6*i;
  return adress;
}
const double &Analysis::EventDataReader::getEventDataAt(const int i,
                                                        const std::string str) const {
  return pEventData[getAdressAt(i, str)];
}
double &Analysis::EventDataReader::setEventDataAt(const int i,
                                                  std::string str) {
  return pEventData[getAdressAt(i, str)];
}




