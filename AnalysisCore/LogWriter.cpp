//
// Created by Daehyun You on 12/24/15.
//

//#ifdef _WIN32
//#define localtime localtime_s
//#endif

#include "LogWriter.h"

Analysis::LogWriter::LogWriter(const std::string prefix) : prefix(prefix) {
  timeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm *ptm = std::localtime(&timeStamp);
  filename = prefix;
  if(!(prefix == "")) { filename += "-"; }
  filename += getTimeStamp();
  logFile.open(filename + ".log", std::fstream::out);
#ifndef __linux__
  // todo: fix me 
  logFile << "It is written at " << std::put_time(ptm,"%c") << std::endl;
#endif 
  logFile << std::endl;
  return;
}
Analysis::LogWriter::~LogWriter() {
  std::time_t tt = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now());
  std::tm * ptm = std::localtime(&tt);
#ifndef __linux__
  // todo: fix me 
  logFile << "It is closed at " << std::put_time(ptm,"%c") << std::endl;
#endif 
  logFile.close();
}
const std::string Analysis::LogWriter::getTimeStamp() const {
  return std::to_string(timeStamp);
}
void Analysis::LogWriter::logAnalysisTools(const Analysis::Unit &unit,
                                           const Analysis::AnalysisTools &analysisTools,
                                           const Analysis::Objects &ions,
                                           const Analysis::Objects &elecs) {
  logFile << "Loaded Parameters: " << std::endl;
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
      logFile << "        " << getObjectName(i).c_str() << ":" << std::endl;
      auto ion = ions.getObject(i).getCopy();
      double p;
      bool info;
      logFile << "            Mass [au]: " << ion.getMass() << std::endl;
      ion.resetEventData();
      ion.setTOF(ion.getMinOfTOF());
      p = analysisTools.calculateMomentumZ(ion, info);
      logFile << "            Pz of the Object having min TOF [au]: ";
      if (info) logFile << p << std::endl;
      else logFile << "fail" << std::endl;
      ion.resetEventData();
      ion.setTOF(ion.getMaxOfTOF());
      p = analysisTools.calculateMomentumZ(ion, info);
      logFile << "            Pz of the Object having max TOF [au]: ";
      if (info) logFile << p << std::endl;
      else logFile << "fail" << std::endl;
      logFile << "            TOF of Pz=0 Object [ns]: "
          << kUnit.writeNanoSec(analysisTools.calculateTOF(ion, 0e0)) << std::endl;
      logFile << "            Period of Cycle [ns]: "
          << kUnit.writeNanoSec(analysisTools.calculatePeriodOfCycle(ion)) << std::endl;
    }
  }
  logFile << "    Electrons: " << std::endl;
  {
    const int &n = elecs.getNumberOfObjects();
    logFile << "        Number of Hits: " << n << std::endl;
    {
      const auto elec = Object(ObjectFlag::RealObject, ObjectFlag::ElecObject);
      logFile << "        TOF of Pz=0 Object [ns]: " <<
          kUnit.writeNanoSec(analysisTools.calculateTOF(elec, 0e0)) << std::endl;
      logFile << "        Period of Cycle [ns]: " <<
          kUnit.writeNanoSec(analysisTools.calculatePeriodOfCycle(elec)) << std::endl;
    }
    for (int i = 0; i < n; i++) {
      auto elec = elecs.getObject(i).getCopy();
      double p;
      bool info;
      elec.resetEventData();
      elec.setTOF(elec.getMinOfTOF());
      p = analysisTools.calculateMomentumZ(elec, info);
      logFile << "            Pz of the Object having min TOF [au]: ";
      if (info) logFile << p << std::endl;
      else logFile << "fail" << std::endl;
      elec.resetEventData();
      elec.setTOF(elec.getMaxOfTOF());
      p = analysisTools.calculateMomentumZ(elec, info);
      logFile << "            Pz of the Object having max TOF [au]: ";
      if (info) logFile << p << std::endl;
      else logFile << "fail" << std::endl;
    }
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
const std::string Analysis::LogWriter::getPrefix() const {
  return prefix;
}


