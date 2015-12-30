//
// Created by Daehyun You on 12/30/15.
//

#include "Run.h"
BL17Analysis::Run::Run() {
  // set seed for random
  srand((unsigned int) time(nullptr));

  // setup reader & writer
  Analysis::JSONReader reader("Parameters.json"); // make json reader
  pLogWriter = new Analysis::LogWriter(reader);
  auto &writer = *pLogWriter;
  writer.logJSONReader(reader);

  // make unit helper
  pUnit = new Analysis::Unit;
  auto &unit = *pUnit;

  // make analysis tools, ions, and electrons
  pTools = new Analysis::AnalysisTools(unit, reader);
  auto &tools = *pTools;
  pIons = new Analysis::Ions(unit, reader, numberOfHitsUsed);
  auto &ions = *pIons;
  pElectrons = new Analysis::Electrons(unit, reader, numberOfHitsUsed);
  auto &electrons = *pElectrons;
  writer.logAnalysisTools(unit, tools, ions, electrons);

  // output option
  optionOfSendingOutOfFrame =
      reader.getBoolAt("output_options.send_out_of_frame");
  optionOfExportingElectronMomentum =
      reader.getBoolAt("output_options.export_electron_momentum");
  writer.write() << "Output Options: " << std::endl;
  writer.write() << "    Send Out of Frame: "
      << (optionOfSendingOutOfFrame ? "true" : "false") << std::endl;
  writer.write() << "    Export electron momentum: "
      << (optionOfExportingElectronMomentum ? "true" : "false") << std::endl;
  writer.write() << std::endl;

  // export electron momentum
  if (optionOfExportingElectronMomentum) {
    std::string filename;
    filename = tools.getID();
    if (!(filename == "")) {
      filename += "-";
    }
    filename += writer.getID();
    filename += ".csv";
    exportedFile.open(filename, std::fstream::out);
    exportedFile << "1st hit electron px" << ",";
    exportedFile << "1st hit electron py" << ",";
    exportedFile << "1st hit electron pz" << ",";
    exportedFile << "1st hit electron E" << ",";
    exportedFile << "2nd hit electron px" << ",";
    exportedFile << "2nd hit electron py" << ",";
    exportedFile << "2nd hit electron pz" << ",";
    exportedFile << "2nd hit electron E" << ",";
    exportedFile << "3rd hit electron px" << ",";
    exportedFile << "3rd hit electron py" << ",";
    exportedFile << "3rd hit electron pz" << ",";
    exportedFile << "3rd hit electron E" << ",";
    exportedFile << "4th hit electron px" << ",";
    exportedFile << "4th hit electron py" << ",";
    exportedFile << "4th hit electron pz" << ",";
    exportedFile << "4th hit electron E" << ",";
    exportedFile << std::endl;
  }

  // initialization is done
  writer.write() << "Initialization is done." << std::endl;
  writer.write() << std::endl;
}
BL17Analysis::Run::~Run() {
  // setup writer
  auto &writer = *pLogWriter;
  auto &tools = *pTools;

  // counter
  writer.write() << "Event count: " << tools.getEventNumber() << std::endl;

  // finalization is done
  delete pUnit;
  delete pTools;
  delete pIons;
  delete pElectrons;
  if (optionOfExportingElectronMomentum) {
    exportedFile.close();
  };
  writer.write() << "Finalization is done." << std::endl;
  writer.write() << std::endl;
  delete pLogWriter;
}
void BL17Analysis::Run::ProcessEvent(Analysis::EventDataReader &reader,
                                     int &ionFlag,
                                     int &electronFlag) {
  // setup
  auto &unit = *pUnit;
  auto &tools = *pTools;
  auto &ions = *pIons;
  auto &electrons = *pElectrons;

  // count event
  tools.loadEventCounter();

  // make sure ion and electron data is empty, and reset flags
  ions.resetEventData();
  electrons.resetEventData();
  ionFlag = 0;
  electronFlag = 0;

  // input event data
  tools.loadEventDataInputer(ions, unit, reader);
  tools.loadEventDataInputer(electrons, unit, reader);

  if (optionOfSendingOutOfFrame) {
    // dead data, don't plot basic data(x, y, TOF)
    { // ion
      const int &m = ions.getNumberOfRealOrDummyObjects();
      for (int i = 0; i < m; i++) {
        if (ions.getRealOrDummyIon(i).isDead()) {
          ions.setRealOrDummyObjectMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag();
        }
      }
    }
    { // electron
      const int &m = electrons.getNumberOfRealOrDummyObjects();
      for (int i = 0; i < m; i++) {
        if (electrons.getRealOrDummyObject(i).isDead()) {
          electrons.setRealOrDummyObjectMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag();
        }
      }
    }
    // dummy data, don't plot momentum
    // ion
    ions.setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag();
    // electron
    electrons.setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag();
  }

  // if all ion event data or all electron event data is dead, ignore the event
  const bool ionsAreAllDead = ions.areAllDeadRealAndDummyObjects();
  const bool electronsAreAllDead = electrons.areAllDeadRealAndDummyObjects();
  if (ionsAreAllDead || electronsAreAllDead) {
    // don't plot basic or momentum data
    if (optionOfSendingOutOfFrame) {
      ions.setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag();
      electrons.setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag();
    }
    if (ionsAreAllDead) {
      ionFlag = -11;
    } else {
      ionFlag = -12;
    }
    if (electronsAreAllDead) {
      electronFlag = -11;
    } else {
      electronFlag = -12;
    }
  } else { // calculate ion or electron momentum
    // ion
    // if all data is not within master region, don't calculate momentum
    const bool ionsAreAllWithinMasterRegion = ions.areAllWithinMasterRegion();
    if (!ionsAreAllWithinMasterRegion) {
      // don't plot momentum data
      if (optionOfSendingOutOfFrame) {
        ions.setAllOfObjectIsOutOfFrameOfMomentumDataFlag();
      }
      ionFlag = -20;
    } else { // calculate ion momentum
      tools.loadMomentumCalculator(ions);
      const bool ionsExistDeadObject = ions.existDeadObject();
      // if it couldn't calculate momentum, don't plot it
      if (ionsExistDeadObject) {
        if (optionOfSendingOutOfFrame) {
          ions.setAllOfObjectIsOutOfFrameOfMomentumDataFlag();
        }
        ionFlag = -30;
      } else {
        // succeed
        ionFlag = 1;
      }
    }

    // electron
    // if all data is not within master region, don't calculate momentum
    const bool electronsAreAllWithinMasterRegion =
        electrons.areAllWithinMasterRegion();
    if (!electronsAreAllWithinMasterRegion) {
      // don't plot momentum data
      if (optionOfSendingOutOfFrame) {
        electrons.setAllOfObjectIsOutOfFrameOfMomentumDataFlag();
      }
      electronFlag = -20;
    } else { // calculate electron momentum
      tools.loadMomentumCalculator(electrons);
      const bool electronsExistDeadObject = electrons.existDeadObject();
      // if it couldn't calculate momentum, don't plot it
      if (electronsExistDeadObject) {
        electronFlag = -30;
      } else {
        // succeed
        electronFlag = 1;
      }
    }
  }

  // export electron momentum
  if (optionOfExportingElectronMomentum) {
    if (ionFlag > 0 && electronFlag > 0) {
      const int &n = electrons.getNumberOfObjects();
      for (int i = 0; i < n; i++) {
        exportedFile << electrons.getElectron(i).getMomentumX(unit) << ",";
        exportedFile << electrons.getElectron(i).getMomentumY(unit) << ",";
        exportedFile << electrons.getElectron(i).getMomentumZ(unit) << ",";
        exportedFile << electrons.getElectron(i).getEnergy(unit) << ",";
      }
      exportedFile << std::endl;
    }
  }
}
const Analysis::Unit &BL17Analysis::Run::getUnit() const {
  return *pUnit;
}
const Analysis::Ions &BL17Analysis::Run::getIons() const {
  return *pIons;
}
const Analysis::Electrons &BL17Analysis::Run::getElectrons() const {
  return *pElectrons;
}
