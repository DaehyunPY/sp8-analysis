//
// Created by Daehyun You on 12/1/15.
//

#ifdef FOR_COBOLDPC2002
Analysis::LMFReader::LMFReader(const CDoubleArray &array) :pParameters(&array) {
  return;
}
Analysis::LMFReader::~LMFReader() { return; }
const double &Analysis::LMFReader::getAt(const int &iTDC,
                                         const int &iCH,
                                         const int &iHit) const {
  const int &nTDC = this->numberOfTDCUsed;
  const int &nCH = this->numberOfChannelsUsed;
  const int &nHit = this->numberOfHitsUsed;
  assert(iTDC >= 0 && iTDC < nTDC);
  assert(iCH >= 0 && iCH < nTDC);
  assert(iHit >= 0 && iHit < nHit);
  return this->getParameters().GetAt(iHit+iCH*(nHit+1)+iTDC*(nHit+1)*nCH);
}
const CDoubleArray &Analysis::LMFReader::getParameters() const {
  return *(this->pParameters);
}
#endif
