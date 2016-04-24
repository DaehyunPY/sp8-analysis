//
// Created by Daehyun You on 12/1/15.
//

#include "EventDataReader.h"

Analysis::EventDataReader::~EventDataReader() {
  delete[] pEventData;
  delete[] pFlagData;
}
Analysis::EventDataReader::EventDataReader(const int numOfHit)
    : numberOfHit(numOfHit) {
  pEventData = new double[2*3*numOfHit];
  pFlagData = new int[2*numOfHit];
}
const int Analysis::EventDataReader::getAdressAt(const int i, const std::string str) const {
  int adress = 0;
  if(str.find("Flag") != std::string::npos) {
    adress += i;
    if (str.find("Elec") != std::string::npos) {
      adress *= 2;
    } else if (str.find("Ion") != std::string::npos) {
      adress *= 1;
    } else {
      assert(false);
    }
    return adress;
  } else {
    if (str.find("Elec") != std::string::npos) {
      adress += 3;
    } else if (str.find("Ion") != std::string::npos) {
      adress += 0;
    } else {
      assert(false);
    }
    if (str.find("T") != std::string::npos) {
      adress += 0;
    } else if (str.find("X") != std::string::npos) {
      adress += 1;
    } else if (str.find("Y") != std::string::npos) {
      adress += 2;
    } else {
      assert(false);
    }
    adress += 6 * i;
    return adress;
  }
}
const double &Analysis::EventDataReader::getEventDataAt(const int i,
                                                        const std::string str) const {
  return pEventData[getAdressAt(i, str)];
}
double &Analysis::EventDataReader::setEventDataAt(const int i,
                                                  std::string str) {
  return pEventData[getAdressAt(i, str)];
}
int &Analysis::EventDataReader::setFlagDataAt(const int i, std::string str) {
  return pFlagData[getAdressAt(i, str)];
}
const int &Analysis::EventDataReader::getFlagDataAt(const int i,
                                                    const std::string str) const {
  return pFlagData[getAdressAt(i, str)];
}
