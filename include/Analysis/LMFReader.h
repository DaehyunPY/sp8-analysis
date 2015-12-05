//
// Created by Daehyun You on 12/1/15.
//

#ifndef ANALYSIS_LMFREADER_H
#define ANALYSIS_LMFREADER_H


#define NUMBER_OF_TDC_USED 3
#define NUMBER_OF_CHANNELS_USED 2
#define NUMBER_OF_HITS_USED 4
#include "CDan2002.h"
#include <assert.h>

namespace Analysis {
class LMFReader {
 private:
  const int numberOfTDCUsed = NUMBER_OF_TDC_USED;
  const int numberOfChannelsUsed = NUMBER_OF_CHANNELS_USED;
  const int numberOfHitsUsed = NUMBER_OF_HITS_USED;
  const CDoubleArray *pParameters;

 public:
  LMFReader(const CDoubleArray &array);
  const CDoubleArray &getParameters() const;
  const double &getAt(const int &iTDC, const int &iCH, const int &iHit) const;
  ~LMFReader();
};
}


#endif //ANALYSIS_LMFREADER_H
