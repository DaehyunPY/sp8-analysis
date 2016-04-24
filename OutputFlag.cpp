//
// Created by Daehyun You on 4/22/16.
//

#include "OutputFlag.h"

Analysis::OutputFlag::OutputFlag() {
  setFlag(0);
}
Analysis::OutputFlag::~OutputFlag() {
}
void Analysis::OutputFlag::setSendingOutOfFrame() {
  setNthDigit(2, flagFor2ndDigit_sendingOutOfFrame);
}
const bool Analysis::OutputFlag::isSendingOutOfFrame() const {
  return getNthDigit(2) == flagFor2ndDigit_sendingOutOfFrame;
}
void Analysis::OutputFlag::setShowingOnlyMasterRegionEvents() {
  setNthDigit(3, flagFor3rdDigit_showingOnlyMasterRegionEvents);
}
const bool Analysis::OutputFlag::isShowingOnlyMasterRegionEvents() const {
  return getNthDigit(3) == flagFor3rdDigit_showingOnlyMasterRegionEvents;
}
