//
// Created by Daehyun You on 11/29/15.
//

#include <iostream>
#include "JSONReader.h"
#include "IonParameters.h"
#include "ElectronParameters.h"
#include "EquipmentParameters.h"
#include "AnalysisTools.h"
#include "Ions.h"
#include "Electrons.h"


int main() {
  Analysis::Unit unit;
  Analysis::JSONReader reader
      ("/Users/Hyuna917/Documents/ClionProjects/Analysis/parameters.json");
  Analysis::AnalysisTools analysisTools(unit, reader);

//  double ionTOF = analysisTools.calculateTOF(ion, 0e0);
//  double electronTOF = analysisTools.calculateTOF(electron, 0e0);

//  Analysis::Ion ion(unit, reader, "1st_hit");
//  analysisTools.loadEventDataInputer(
//      ion, unit, 97689 / 1000, 102501 / 1000, 4.98977e+006 / 1000, 0);
//  analysisTools.loadMomentumCalculator(ion);

  Analysis::Ions ions(unit,reader);
  analysisTools.loadEventDataInputer(
      ions.setIonMembers(0),
      unit, 97689 / 1000, 102501 / 1000, 4.98977e+006 / 1000, 0);
  analysisTools.loadMomentumCalculator(ions.setIonMembers(0));

//  Analysis::Electron electron(unit, reader);
//  analysisTools.loadEventDataInputer(
//      electron, unit, 130503 / 1000, 129140 / 1000, 1.8279e+006 / 1000, 0);
//  analysisTools.loadMomentumCalculator(electron);

  Analysis::Electrons electrons(unit, reader);
  analysisTools.loadEventDataInputer(
      electrons.setElectronMembers(0),
      unit, 130503 / 1000, 129140 / 1000, 1.8279e+006 / 1000, 0);
  analysisTools.loadMomentumCalculator(electrons.setElectronMembers(0));

  std::cout << "hello world!" << std::endl;
  return 0;
}
