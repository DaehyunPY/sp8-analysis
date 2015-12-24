//
// Created by Daehyun You on 12/24/15.
//

#ifndef ANALYSIS_LOGWRITER_H
#define ANALYSIS_LOGWRITER_H


#include "AnalysisTools.h"

namespace Analysis {
class LogWriter {
 private:
  const std::string ID;
  std::string filename;
  std::fstream logFile;
  const int getRandomNumber() const;
  const std::string getRandomID() const;
  const std::string getObjectName(int) const;

 public:
  LogWriter(const std::string &prefix = "");
  LogWriter(const Analysis::JSONReader &);
  ~LogWriter();
  const std::string getID() const;
  void logJSONReader(const JSONReader &);
  void logAnalysisTools(const Unit &,
                        const AnalysisTools &,
                        const Ions &,
                        const Electrons &);
  std::fstream &write();
};
}


#endif //BL17_ANALYSIS_LOGWRITER_H
