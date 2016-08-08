//
// Created by Daehyun You on 12/1/15.
//

#include "EventDataReader.h"

Analysis::EventDataReader::~EventDataReader() {
  if (pEventData) {
    delete[] pEventData;
    pEventData = nullptr;
  }
  if (pFlagData) {
    delete[] pFlagData;
    pFlagData = nullptr;
  }
}
Analysis::EventDataReader::EventDataReader(const int iNum, const int eNum)
    : maxNumOfIonHits(iNum), maxNumOfElecHits(eNum) {
  pEventData = new double[3*(iNum+eNum)];
  pFlagData = new int[iNum+eNum];
}
const int Analysis::EventDataReader::getAdressAt(const int i, const std::string str) const {
  int adress = 0;
  if(str.find("Flag") != std::string::npos) {
    adress += i;
    if (str.find("Ion") != std::string::npos) {
      assert(0 <= i && i<maxNumOfIonHits);
    } else if (str.find("Elec") != std::string::npos) {
      assert(0 <= i && i<maxNumOfElecHits);
      adress += maxNumOfIonHits;
    } else {
      assert(false);
    }
    return adress;
  } else {
    adress += 3*i;
    if (str.find("T") != std::string::npos) {
      adress += 0;
    } else if (str.find("X") != std::string::npos) {
      adress += 1;
    } else if (str.find("Y") != std::string::npos) {
      adress += 2;
    } else {
      assert(false);
    }
    if (str.find("Ion") != std::string::npos) {
      assert(0 <= i && i<maxNumOfIonHits);
    } else if (str.find("Elec") != std::string::npos) {
      assert(0 <= i && i<maxNumOfElecHits);
      adress += 3*maxNumOfIonHits;
    } else {
      assert(false);
    }
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
