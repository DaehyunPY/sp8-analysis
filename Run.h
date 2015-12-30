//
// Created by Daehyun You on 12/30/15.
//

#ifndef BL17ANALYSIS_RUN_H
#define BL17ANALYSIS_RUN_H


#include "Analysis/Analysis.h"
namespace BL17Analysis {
class Run {
  const int numberOfTDCUsed = 3;
  const int numberOfChannelsUsed = 2;
  const int numberOfHitsUsed = 4;
  Analysis::LogWriter *pLogWriter;
  Analysis::Unit *pUnit;
  Analysis::AnalysisTools *pTools;
  Analysis::Ions *pIons;
  Analysis::Electrons *pElectrons;

  bool optionOfSendingOutOfFrame;
  bool optionOfExportingElectronMomentum;
  std::fstream exportedFile;

 public:
  Run();
  ~Run();
  void ProcessEvent(Analysis::EventDataReader &reader,
                    int &ionFlag,
                    int &ElectronFlag);
  const Analysis::Unit &getUnit() const;
  const Analysis::Ions &getIons() const;
  const Analysis::Electrons &getElectrons() const;
  const int &getNumberOfTDCUsed() const; 
  const int &getNumberOfChannelsUsed() const; 
  const int &getNumberOfHitsUsed() const; 
};
}

#endif //BL17ANALYSIS_BL17ANALYSIS_H
