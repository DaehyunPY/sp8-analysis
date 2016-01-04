//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include "Flag.h"

namespace Analysis {
class ObjectFlag: protected Flag {
 private:
  const int initialFlag = 0;
 public:
  ObjectFlag();
  ~ObjectFlag();
  void resetFlag();
  const bool isInitial() const;

 private:
  const int flagFor1stDigit_withinMasterRegion = 1;
  const int flagFor1stDigit_outOfMasterRegion = 2;
  const int flagFor1stDigit_dead = 3;
//  ┌─whole data────────────────────────────────┐
//  │                                           │
//  │ ┌─master region─┐┌─out of master region─┐ │
//  │ │               ││                      │ │
//  │ │               ││         ┌─dead────┐  │ │
//  │ │               ││         │         │  │ │
//  │ │ flag #1       ││ flag #2 │ flag #3 │  │ │
//  │ │               ││         │         │  │ │
//  │ │               ││         └─────────┘  │ │
//  │ └───────────────┘└──────────────────────┘ │
//  │                                           │
//  │ initial data (not input TOF data)         │
//  │ flag #0                                   │
//  │                                           │
//  └───────────────────────────────────────────┘
 public:
  void setWithinMasterRegionFlag(); // only can set when it's initial flag(0)
  void setOutOfMasterRegionFlag(); // only can set when it's initial flag(0)
  void setDeadFlag();
 public:
  const bool isDead() const;
  const bool isOutOfMasterRegion() const;
  const bool isWithinMasterRegion() const;

 private:
  const int flagFor2ndDigit_inFrameOfAllData = 1;
  const int flagFor2ndDigit_outOfFrameOfMomentumData = 2;
  const int flagFor2ndDigit_outOfFrameOfBasicData = 3;
//  ┌─whole data───────────────────────────────────────────────────────┐
//  │┌─in frame of all data─┐┌─out of frame of momentum data──────────┐│
//  ││                      ││                 ~~~~~~~~~~~~~          ││
//  ││                      ││                 (Px,Py,Pz,E)           ││
//  ││                      ││         ┌─out of frame of basic data──┐││
//  ││                      ││         │                 ~~~~~~~~~~  │││
//  ││ flag #1              ││ flag #2 │ flag #3         (X,Y,TOF,   │││
//  ││                      ││         │                  Px,Py,Pz,E)│││
//  ││                      ││         └─────────────────────────────┘││
//  │└──────────────────────┘└────────────────────────────────────────┘│
//  │ initial data (not inputed)                                       │
//  │ flag #0                                                          │
//  └──────────────────────────────────────────────────────────────────┘
 public:
  void setInFrameOfAllDataFlag(); // only can set when it's initial flag(0)
  void setOutOfFrameOfMomentumDataFlag
      (); // only can set when it's in-frame-of-all-data flag(1)
  void setOutOfFrameOfBasicDataFlag
      (); // only can set when it's in-frame-of-all-data flag(1), or out-of-frame-of-momentum-data flag(2)
  const bool isInFrameOfAllData() const;
  const bool isOutOfFrameOfMomentumData() const;
  const bool isOutOfFrameOfBasicData() const;
};
}

#endif //ANALYSIS_FLAGLIST_H
