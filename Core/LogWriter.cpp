//
// Created by Daehyun You on 12/24/15.
//

#include <iomanip>
#include "LogWriter.h"
Analysis::LogWriter::LogWriter(const std::string &prefix)
    : ID(getRandomID()) {
  auto now = std::time(nullptr);
  filename = prefix;
  if(!(prefix == "")) { filename += "-"; }
  filename += ID;
  filename += ".log";
  logFile.open(filename, std::fstream::out);
  logFile
      << "It is writen at "
      << std::put_time(std::localtime(&now), "%c %Z")
      << "."
      << std::endl;
  logFile << "The path is setten here." << std::endl;
  logFile << std::endl;
  return;
}
Analysis::LogWriter::~LogWriter() {
  auto now = std::time(nullptr);
  logFile
      << "It is closed at "
      << std::put_time(std::localtime(&now), "%c %Z")
      << "."
      << std::endl;
  logFile << std::endl;
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
  logFile << std::endl;
}
Analysis::LogWriter::LogWriter(const Analysis::JSONReader &reader)
    : LogWriter(reader.hasMember("ID") ? reader.getStringAt("ID") : ""){
  return;
}
void Analysis::LogWriter::logAnalysisTools(const Analysis::Unit &unit,
                                           const Analysis::AnalysisTools &analysisTools,
                                           const Analysis::Ions &ions,
                                           const Analysis::Electrons &electrons) {
  logFile << "Loaded Parameters: " << std::endl;
  logFile << "    ID: " << analysisTools.getID() << std::endl;
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
      logFile << "        " << name << ":" << std::endl;
      logFile << "            Mass: " << ions.getIon(i).getMass(unit) << std::endl;
      logFile << "            Charge: " << ions.getIon(i).getCharge(unit) << std::endl;
      logFile << "            Maximum of TOF: " << ions.getIon(i).getMaxOfTOF(unit) << std::endl;
      logFile << "            Minimum of TOF: " << ions.getIon(i).getMinOfTOF(unit) << std::endl;
      const double t1 = analysisTools.calculateTOF(unit, ions.getIon(i), 0e0);
      const double t2 = analysisTools.calculatePeriodOfCycle(unit, ions.getIon(0));
      logFile << "            TOF of Stopped Object: " << t1 << std::endl;
      logFile << "            Period of Cycle: " << t2 << std::endl;
    }
  }
  logFile << "    Electrons: " << std::endl;
  {
    const int &n = electrons.getNumberOfObjects();
    logFile << "        Number of Hits: " << n << std::endl;
    const double t1 = analysisTools.calculateTOF(unit, electrons.getElectron(0), 0e0);
    const double t2 = analysisTools.calculatePeriodOfCycle(unit, electrons.getElectron(0));
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
