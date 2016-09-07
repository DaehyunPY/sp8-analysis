//
// Created by Daehyun You on 12/24/15.
//

#ifndef ANALYSIS_LOGWRITER_H
#define ANALYSIS_LOGWRITER_H


#ifdef _WIN32
#include <chrono>
#endif
#ifdef __linux__
#include <chrono>
#endif 
#include <iomanip>
#include "AnalysisTools.h"

namespace Analysis {
class LogWriter {
 private:
  std::string prefix;
  std::time_t timeStamp;
  std::string filename;
  std::fstream logFile;
  const std::string getObjectName(int i) const;

 public:
  LogWriter(const std::string prefix = "");
  ~LogWriter();
  const std::string getPrefix() const;
  const std::string getTimeStamp() const;
  const std::string getFilename() const;
  void logAnalysisTools(const Unit &,
                        const AnalysisTools &,
                        const Objects &ions,
                        const Objects &elecs);
  std::fstream &write();
};
}


#endif //BL17_ANALYSIS_LOGWRITER_H
