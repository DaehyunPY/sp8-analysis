//
// Created by Daehyun You on 11/29/15.
//

#include <iostream>
#include "AnalysisTools.h"

int main() {
  Analysis::Unit unit;
  Analysis::JSONReader reader
      ("/Users/Hyuna917/.local/Celler/ueda-lab/BL17_Analysis/include/Analysis/parameters.json");
  Analysis::AnalysisTools analysisTools(unit, reader);

//  double ionTOF = analysisTools.calculateTOF(ion, 0e0);
//  double electronTOF = analysisTools.calculateTOF(electron, 0e0);

//  Analysis::Ion ion(unit, reader, "1st_hit");
//  analysisTools.loadEventDataInputer(
//      ion, unit, 97689 / 1000, 102501 / 1000, 4.98977e+006 / 1000, 0);
//  analysisTools.loadMomentumCalculator(ion);

  Analysis::Ions ions(unit, reader, 4);
  ions.resetEventData();
  analysisTools.loadEventDataInputer(
      ions.setIonMembers(0),
      unit, 97689 / 1000, 102501 / 1000, 4.98977e+006 / 1000, 0);
  analysisTools.loadMomentumCalculator(ions.setIonMembers(0));
  std::cout << ions.getIon(0).getLocationX(unit) << std::endl;
  std::cout << ions.getIon(0).getLocationY(unit) << std::endl;
  std::cout << ions.getIon(0).getMomentumX() << std::endl;
  std::cout << ions.getIon(0).getMomentumY() << std::endl;
  std::cout << ions.getIon(0).getMomentumZ() << std::endl;
  std::cout << ions.getIon(0).getEnergy(unit) << std::endl;

//  Analysis::Electron electron(unit, reader);
//  analysisTools.loadEventDataInputer(
//      electron, unit, 130503 / 1000, 129140 / 1000, 1.8279e+006 / 1000, 0);
//  analysisTools.loadMomentumCalculator(electron);

  Analysis::Electrons electrons(unit, reader, 4);
  electrons.resetEventData();
  analysisTools.loadEventDataInputer(
      electrons.setElectronMembers(0),
      unit, 130503 / 1000, 129140 / 1000, 1.8279e+006 / 1000, 0);
  analysisTools.loadMomentumCalculator(electrons.setElectronMembers(0));
  std::cout << electrons.getElectron(0).getLocationX(unit) << std::endl;
  std::cout << electrons.getElectron(0).getLocationY(unit) << std::endl;
  std::cout << electrons.getElectron(0).getMomentumX(unit) << std::endl;
  std::cout << electrons.getElectron(0).getMomentumY(unit) << std::endl;
  std::cout << electrons.getElectron(0).getMomentumZ(unit) << std::endl;
  std::cout << electrons.getElectron(0).getEnergy(unit) << std::endl;

  std::cout << "hello world!" << std::endl;
  return 0;
}
