//
// Created by Daehyun You on 12/24/15.
//

//#ifdef _WIN32
//#define localtime localtime_s
//#endif

#include <iomanip>
#include <ctime>
#include <locale>
#include <chrono>
#include <string>

#include "LogWriter.h"
Analysis::LogWriter::LogWriter(const std::string &prefix) {
  std::time_t tt = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now());
  std::tm * ptm = std::localtime(&tt);
  ID = std::to_string(tt);
  filename = prefix;
  if(!(prefix == "")) { filename += "-"; }
  filename += ID;
  filename += ".log";
  logFile.open(filename, std::fstream::out);
  logFile << "It is written at " << std::put_time(ptm,"%c") << std::endl;
  logFile << "The path is set here." << std::endl;
  logFile << std::endl;
  return;
}
Analysis::LogWriter::~LogWriter() {
  std::time_t tt = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now());
  std::tm * ptm = std::localtime(&tt);
  logFile << "It is closed at " << std::put_time(ptm,"%c") << std::endl;
  logFile.close();
}
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
void Analysis::LogWriter::logResultOfLoadingJSONFile(const Analysis::JSONReader &reader) {
  if(reader.getFlag().fileIsClosedAndDataIsSaved()) {
    logFile << "The JSON file '" << reader.getFilename().c_str() << "' is loaded successfully." << std::endl;
  } else if(reader.getFlag().fileIsNotExist()) {
    logFile << "There is not the JSON file '" << reader.getFilename().c_str() << "'." << std::endl;
  }
  if(reader.getFlag().hasNoParseError()) {
    logFile << "The JSON file has no parse error." << std::endl;
  } else if(reader.getFlag().hasParseError()){
    logFile << "The JSON file has parse error." << std::endl;
  }
  logFile << std::endl;
}
Analysis::LogWriter::LogWriter(const Analysis::JSONReader &reader)
    : LogWriter(reader.hasMember("ID") ? reader.getStringAt("ID") : ""){
  return;
}
void Analysis::LogWriter::logAnalysisTools(const Analysis::Unit &unit,
                                           const Analysis::AnalysisTools &analysisTools,
                                           const Analysis::Objects &ions,
                                           const Analysis::Objects &elecs) {
  logFile << "Loaded Parameters: " << std::endl;
  logFile << "    ID: " << analysisTools.getID().c_str() << std::endl;
  logFile << "    Equipment Parameters: " << std::endl;
  logFile << "        Electric Potential of Electron Region: " << analysisTools.getEquipmentParameters().getElectricPotentialOfElectronRegion(unit) << std::endl;
  logFile << "        Electric Potential of Ion 1st: " << analysisTools.getEquipmentParameters().getElectricPotentialOfIon1st(unit) << std::endl;
  logFile << "        Electric Potential of Ion 2nd: " << analysisTools.getEquipmentParameters().getElectricPotentialOfIon2nd(unit) << std::endl;
  logFile << "        Electric Potential of Ion MCP: " << analysisTools.getEquipmentParameters().getElectricPotentialOfIonMCP(unit) << std::endl;
  logFile << "        Length of D2: " << analysisTools.getEquipmentParameters().getLengthOfD2(unit) << std::endl;
  logFile << "        Length of D1: " << analysisTools.getEquipmentParameters().getLengthOfD1(unit) << std::endl;
  logFile << "        Length of L1: " << analysisTools.getEquipmentParameters().getLengthOfL1(unit) << std::endl;
  logFile << "        Length of L2: " << analysisTools.getEquipmentParameters().getLengthOfL2(unit) << std::endl;
  logFile << "        Length of L3: " << analysisTools.getEquipmentParameters().getLengthOfL3(unit) << std::endl;
  logFile << "        Magnetic Filed: " << analysisTools.getEquipmentParameters().getMagneticFiled(unit) << std::endl;
  logFile << "    Ion Parameters: " << std::endl; 
  logFile << "        Angle of Detector: " << analysisTools.getIonParameters().getAngleOfDetector(unit) << std::endl;
  logFile << "        Pixel Size of x: " << analysisTools.getIonParameters().getPixelSizeOfX() << std::endl;
  logFile << "        Pixel Size of y: " << analysisTools.getIonParameters().getPixelSizeOfY() << std::endl;
  logFile << "        Dead Time: " << analysisTools.getIonParameters().getDeadTime(unit) << std::endl;
  logFile << "        X Zero of COM: " << analysisTools.getIonParameters().getXZeroOfCOM(unit) << std::endl;
  logFile << "        Y Zero of COM: " << analysisTools.getIonParameters().getYZeroOfCOM(unit) << std::endl;
  logFile << "        Time Zero of TOF: " << analysisTools.getIonParameters().getTimeZeroOfTOF(unit) << std::endl;
  logFile << "    Electron Parameters: " << std::endl;
  logFile << "        Angle of Detector: " << analysisTools.getElectronParameters().getAngleOfDetector(unit) << std::endl;
  logFile << "        Pixel Size of x: " << analysisTools.getElectronParameters().getPixelSizeOfX() << std::endl;
  logFile << "        Pixel Size of y: " << analysisTools.getElectronParameters().getPixelSizeOfY() << std::endl;
  logFile << "        Dead Time: " << analysisTools.getElectronParameters().getDeadTime(unit) << std::endl;
  logFile << "        X Zero of COM: " << analysisTools.getElectronParameters().getXZeroOfCOM(unit) << std::endl;
  logFile << "        Y Zero of COM: " << analysisTools.getElectronParameters().getYZeroOfCOM(unit) << std::endl;
  logFile << "        Time Zero of TOF: " << analysisTools.getElectronParameters().getTimeZeroOfTOF(unit) << std::endl;
  logFile << "    Ions: " << std::endl;
  {
    const int &n = ions.getNumberOfObjects();
    logFile << "        Number of Hits: " << n << std::endl;
    for (int i = 0; i < n; i++) {
      const std::string name = getObjectName(i);
      logFile << "        " << name.c_str() << ":" << std::endl;
      const double t1 = kUnit.writeNanoSec(analysisTools.calculateTOF(ions.getObject(i), 0e0));
      const double t2 = kUnit.writeNanoSec(analysisTools.calculatePeriodOfCycle(ions.getObject(i)));
      logFile << "            TOF of Stopped Object: " << t1 << std::endl;
      logFile << "            Period of Cycle: " << t2 << std::endl;
    }
  }
  logFile << "    Electrons: " << std::endl;
  {
    const int &n = elecs.getNumberOfObjects();
    logFile << "        Number of Hits: " << n << std::endl;
    const double t1 = kUnit.writeNanoSec(analysisTools.calculateTOF(Object(ObjectFlag::ElecObject), 0e0));
    const double t2 = kUnit.writeNanoSec(analysisTools.calculatePeriodOfCycle(Object(ObjectFlag::ElecObject)));
    logFile << "        TOF of Stopped Object: " << t1 << std::endl;
    logFile << "        Period of Cycle: " << t2 << std::endl;
  }
  logFile << std::endl;
}
const std::string Analysis::LogWriter::getObjectName(int i) const {
  i += 1;
  assert(i > 0);
  const int firstDigit = i % 10;
  const int secondDigit = (i / 10) % 10;
  std::string str = std::to_string(i);
  if (secondDigit == 1) { return str + "th Hit"; }
  else {
    if (firstDigit == 1) { return str + "st Hit"; }
    else if (firstDigit == 2) { return str + "nd Hit"; }
    else if (firstDigit == 3) { return str + "rd Hit"; }
    else { return str + "th Hit"; }
  }
}
std::fstream &Analysis::LogWriter::write() {
  return logFile;
}
const std::string Analysis::LogWriter::getFilename() const {
  return filename;
}
