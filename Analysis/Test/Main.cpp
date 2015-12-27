//
// Created by Daehyun You on 11/29/15.
//

#include <iostream>
#include "../AnalysisTools.h"
#include "../LogWriter.h"

int main() {
  Analysis::Unit unit;
  Analysis::JSONReader reader("/Users/Hyuna917/Documents/ClionProjects/BL17_Analysis/Example/Local/Parameters.json");
  Analysis::LogWriter writer(reader);
  writer.logJSONReader(reader);

  Analysis::AnalysisTools analysisTools(unit, reader);
  Analysis::Ions ions(unit, reader, 4);
  Analysis::Electrons electrons(unit, reader, 4);
  writer.logAnalysisTools(unit, analysisTools, ions, electrons);

  ions.resetEventData();
  analysisTools.loadEventDataInputer(
      ions.setIonMembers(0),
      unit,
      97689 / 1000, 102501 / 1000, 4.98977e+006 / 1000, 0);
  analysisTools.loadMomentumCalculator(ions.setIonMembers(0));
  writer.write() << ions.getIon(0).getLocationX(unit) << std::endl;
  writer.write() << ions.getIon(0).getLocationY(unit) << std::endl;
  writer.write() << ions.getIon(0).getTOF(unit) << std::endl;
  writer.write() << ions.getIon(0).getMomentumX(unit) << std::endl;
  writer.write() << ions.getIon(0).getMomentumY(unit) << std::endl;
  writer.write() << ions.getIon(0).getMomentumZ(unit) << std::endl;
  writer.write() << ions.getIon(0).getEnergy(unit) << std::endl;

  electrons.resetEventData();
  analysisTools.loadEventDataInputer(
      electrons.setElectronMembers(0),
      unit, 130503 / 1000, 129140 / 1000, 1.8279e+006 / 1000, 0);
  analysisTools.loadMomentumCalculator(electrons.setElectronMembers(0));
  writer.write() << electrons.getElectron(0).getLocationX(unit) << std::endl;
  writer.write() << electrons.getElectron(0).getLocationY(unit) << std::endl;
  writer.write() << electrons.getElectron(0).getTOF(unit) << std::endl;
  writer.write() << electrons.getElectron(0).getMomentumX(unit) << std::endl;
  writer.write() << electrons.getElectron(0).getMomentumY(unit) << std::endl;
  writer.write() << electrons.getElectron(0).getMomentumZ(unit) << std::endl;
  writer.write() << electrons.getElectron(0).getEnergy(unit) << std::endl;

  return 0;
}
