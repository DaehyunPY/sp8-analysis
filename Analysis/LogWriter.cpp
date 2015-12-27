//
// Created by Daehyun You on 12/24/15.
//

#include <iomanip>
#include "LogWriter.h"
Analysis::LogWriter::LogWriter(const std::string &prefix)
    : ID(getRandomID()) {
  auto now = std::time(nullptr);
  if(prefix == "") {
    filename = ID + ".log";
  } else {
    filename = prefix + "-" + ID + ".log";
  }
  logFile.open(filename, std::fstream::out);
  logFile
      << "It is writen at "
      << std::put_time(std::localtime(&now), "%c %Z")
      << "."
      << std::endl;
  logFile << "The path is setten here." << std::endl;
  return;
}
Analysis::LogWriter::~LogWriter() { return; }
const int Analysis::LogWriter::getRandomNumber() const {
  const char numbers[] = "0123456789";
  const int n = 10;
  return numbers[rand() %n];
}
const std::string Analysis::LogWriter::getRandomID() const {
  std::string ID = "";
  ID += getRandomNumber();
  ID += getRandomNumber();
  ID += getRandomNumber();
  ID += getRandomNumber();
  return ID;
}
const std::string Analysis::LogWriter::getID() const {
  return ID;
}
void Analysis::LogWriter::logJSONReader(const Analysis::JSONReader &reader) {
  reader.getFilename();
  if(reader.getFlag().isOpen()) {
    logFile
        << "JSON file '"
        << reader.getFilename() << "' is loaded." << std::endl;
  } else {
    logFile
        << "There is no JSON file '"
        << reader.getFilename() << "'." << std::endl;
  }
  if(reader.getFlag().hasNoParseError()) {
    logFile << "The JSON file has no parse error." << std::endl;
  } else {
    logFile
        << "The JSON file has parse error." << std::endl;
  }
}
Analysis::LogWriter::LogWriter(const Analysis::JSONReader &reader)
    : LogWriter(reader.hasMember("ID") ? reader.getStringAt("ID") : "0000"){
  return;
}
void Analysis::LogWriter::logAnalysisTools(const Analysis::Unit &unit,
                                           const Analysis::AnalysisTools &analysisTools,
                                           const Analysis::Ions &ions,
                                           const Analysis::Electrons &electrons) {
  logFile << "Parameters ID is " + analysisTools.getID() << "." << std::endl;
  const int &n = ions.getNumberOfObjects();
  for (int i = 0; i < n; i++) {
    const std::string name = getObjectName(i);
    const double t = analysisTools.calculateTOF(unit, ions.getIon(i), 0e0);
    logFile << "TOF of " + name + " ion: " << t << std::endl;
  }
  const double t1 = analysisTools.calculateTOF(unit, electrons.getElectron(0), 0e0);
  const double t2 = analysisTools.calculatePeriodOfCycle(unit, electrons.getElectron(0));
  logFile << "TOF of electron: " << t1 << std::endl;
  logFile << "Period of cycle of electron: " << t2 << std::endl;
}
const std::string Analysis::LogWriter::getObjectName(int i) const {
  i += 1;
  assert(i > 0);
  const int firstDigit = i % 10;
  const int secondDigit = (i / 10) % 10;
  std::string str = std::to_string(i);
  if (secondDigit == 1) { return str + "th hit"; }
  else {
    if (firstDigit == 1) { return str + "st hit"; }
    else if (firstDigit == 2) { return str + "nd hit"; }
    else if (firstDigit == 3) { return str + "rd hit"; }
    else { return str + "th hit"; }
  }
}
std::fstream &Analysis::LogWriter::write() {
  return logFile;
}
