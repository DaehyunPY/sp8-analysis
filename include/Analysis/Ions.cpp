//
// Created by Daehyun You on 12/4/15.
//

#include "Ions.h"
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader,
                     const int &n) : Objects(n) {
  const std::string names[9]
      = {"1st_hit", "2nd_hit", "3rd_hit",
         "4th_hit", "5th_hit", "6th_hit",
         "7th_hit", "8th_hit", "9th_hit"};
  for(int i=0; i<n; i++) {
    this->pIon[i] = new Ion(unit,reader,names[i]);
    this->setObject(i, *(this->pIon[i]));
  }
  return;
}
const Analysis::Ion &Analysis::Ions::getIon(const int &i) const {
  assert(i < this->getNumberOfHits());
  return *(this->pIon[i]);
}
Analysis::Ions::~Ions() { return; }
Analysis::Ion &Analysis::Ions::setIonMembers(const int &i) {
  assert(i < this->getNumberOfHits());
  return *(this->pIon[i]);
}
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader)
    : Ions(unit, reader, reader.getInt("ions.number_of_hits")) { return; }
