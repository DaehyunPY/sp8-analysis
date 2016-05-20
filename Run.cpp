//
// Created by Daehyun You on 12/30/15.
//

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#else
#include <unistd.h>
#endif

#include <ctime>
#include <TChain.h>
#include <RooDataProjBinding.h>

#include "RUN.h"
Analysis::Run::Run(const std::string configFilename) {
  // Setup json file reader
  Analysis::JSONReader configReader(configFilename);

  // Change the working directory
  chdir(configReader.getStringAt("working_directory").c_str());

  // Setup writer
  pLogWriter = new Analysis::LogWriter(configReader);
  pLogWriter->logResultOfLoadingJSONFile(configReader);

  // Setup ROOT files
  if (configReader.getStringAt("setup_input.type") == "ROOT") {
    pEventChain =
        new TChain(configReader.getStringAt("setup_input.tree_name").c_str());
    pEventChain->Add(configReader.getStringAt("setup_input.filenames").c_str());
    pLogWriter->write() << "Input file type is ROOT. " << std::endl;
    pLogWriter->write() << "    Filenames: "
        << configReader.getStringAt("setup_input.filenames").c_str()
        << std::endl;
    pLogWriter->write() << "    Tree Name: "
        << configReader.getStringAt("setup_input.tree_name").c_str()
        << std::endl;
    pLogWriter->write() << std::endl;

    // Setup event reader
    numberOfHits = configReader.getIntAt("setup_input.number_of_hits");
    pEventReader = new Analysis::EventDataReader(numberOfHits);
    char ch[2];
    for (int i = 0; i < numberOfHits; i++) {
      for (std::string str : {"IonX", "IonY", "IonT", "ElecX", "ElecY",
                              "ElecT"}) {
        sprintf(ch, "%01d", i);
        pEventChain->SetBranchAddress((str + ch).c_str(),
                                      &(pEventReader->setEventDataAt(i,
                                                                     str
                                                                         + ch)));
      }
      for (std::string str : {"IonFlag", "ElecFlag"}) {
        sprintf(ch, "%01d", i);
        pEventChain->SetBranchAddress((str + ch).c_str(),
                                      &(pEventReader->setFlagDataAt(i,
                                                                    str + ch)));
      }
    }
  }

  // Setup unit helper
  pUnit = new Analysis::Unit;

  // Make analysis tools, ions, and electrons
  pTools = new Analysis::AnalysisTools(*pUnit, configReader);
  pIons = new Analysis::Ions(*pUnit, configReader, numberOfHits);
  pElectrons = new Analysis::Electrons(*pUnit, configReader, numberOfHits);
  pLogWriter->logAnalysisTools(*pUnit, *pTools, *pIons, *pElectrons);

  // Read output option
  if (configReader.getBoolAt("setup_output.send_out_of_frame")) {
    flag.setSendingOutOfFrame();
  }
  if (configReader.getBoolAt("setup_output.show_only_master_region_events")) {
    flag.setShowingOnlyMasterRegionEvents();
  }
  pLogWriter->write() << "Output Options: " << std::endl;
  pLogWriter->write() << "    Send Out of Frame: "
      << (flag.isSendingOutOfFrame() ? "true" : "false") << std::endl;
  pLogWriter->write() << "    Show Only Master Region Events: "
      << (flag.isShowingOnlyMasterRegionEvents() ? "true" : "false")
      << std::endl;
  pLogWriter->write() << std::endl;

  // Open ROOT file
  std::string rootFilename = "";
  rootFilename += pTools->getID();
  if (rootFilename != "") {
    rootFilename += "-";
  }
  rootFilename += pLogWriter->getID();
  rootFilename += ".root";
  pRootFile = new TFile(rootFilename.c_str(), "update");
  pHist = new OutputHist(false, numberOfHists);
  pHist->linkRootFile(*pRootFile);
  createIonHists();
  createElecHists();
  createNatureHists();

  // Initialization is done
  pLogWriter->write() << "Initialization is done." << std::endl;
  pLogWriter->write() << std::endl;
}
Analysis::Run::~Run() {
  // counter
  pLogWriter->write() << "Event count: " << pTools->getEventNumber()
      << std::endl;

  // root
  flushHist();
  pRootFile->Close();

  // finalization is done
  delete pHist;
  delete pRootFile;
  delete pElectrons;
  delete pIons;
  delete pTools;
  delete pUnit;
  delete pEventReader;
  delete pEventChain;

  pLogWriter->write() << "Finalization is done." << std::endl;
  pLogWriter->write() << std::endl;
  delete pLogWriter;
}
void Analysis::Run::processEvent(const long raw) {
  // Setup event chain
  pEventChain->GetEntry(raw);

  // Count event
  pTools->loadEventCounter();

  // make sure ion and electron data is empty, and reset flags
  pIons->resetEventData();
  pElectrons->resetEventData();

  // input event data
  pTools->loadEventDataInputer(*pIons, *pUnit, *pEventReader);
  pTools->loadEventDataInputer(*pElectrons, *pUnit, *pEventReader);

  // resort option
  if (pIons->areAllFlag(ObjectFlag::MostOrSecondMostReliable)
      && pElectrons->areAllFlag(ObjectFlag::MostOrSecondMostReliable)) {

    // if all ion event data or all electron event data is dead, ignore the event
    const bool ionsAreAllDead =
        pIons->areAllFlag(ObjectFlag::Dead, Objects::RealOrDummy);
    const bool elecAreAllDead =
        pElectrons->areAllFlag(ObjectFlag::Dead, Objects::RealOrDummy);

    // If all objects are dead, don't calculate momentum
    if (!ionsAreAllDead) {
      for (int i = 0; i < pIons->getNumberOfObjects(); i++) {
        pTools->loadMomentumCalculator(*pIons);
      }
    }

    // If all objects are dead, don't calculate momentum
    if (!elecAreAllDead) {
      for (int i = 0; i < pElectrons->getNumberOfObjects(); i++) {
        pTools->loadMomentumCalculator(*pElectrons);
      }
    }

    // Fill histograms
    fillIonHists();
	fillElecHists();
    fillNatureHists();
  }

  // Fill hists for resort flags
}
const long Analysis::Run::getEntries() const {
  return (long) pEventChain->GetEntries();
}
void Analysis::Run::createNatureHists() {
  pHist->create1d(SAMETITLEWITH(histID_1stHitIonTOF_under2ndAnd3rdHitIonAreNotDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfNatureHists);
  pHist->create2d(SAMETITLEWITH(histID_2ndAnd3rdHitIonTOF_under1stHitIonIsInMasterRegion),
                  "1st Hit Ion TOF [ns]", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfNatureHists);
  pHist->create2d(SAMETITLEWITH(histID_1stHitElecEAndSumOfIonTOFs_underMasterCondition),
                  "Energy [eV]", "Sum of TOFs [ns]",
                  H2_ELECTRON_ENERGY, H2_ION_SUMOFTOF(4),
                  dirNameOfNatureHists);
  pHist->create1d(SAMETITLEWITH(histID_1stHitElecE_underMasterCondition),
                  "Energy [eV]",
                  H1_ELECTRON_ENERGY,
                  dirNameOfNatureHists);
}
void Analysis::Run::fillNatureHists() {
  const bool under2ndAnd3rdHitIonAreNotDead =
      (!pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead))
          && (!pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead));
  if (under2ndAnd3rdHitIonAreNotDead) {
    pHist->fill1d(histID_1stHitIonTOF_under2ndAnd3rdHitIonAreNotDead,
                  pIons->getRealOrDummyObject(0).getTOF(*pUnit));
  }
  const bool under1stHitIonInMasterRegion =
      pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::WithinMasterRegion);
  if (under1stHitIonInMasterRegion) {
    pHist->fill2d(histID_2ndAnd3rdHitIonTOF_under1stHitIonIsInMasterRegion,
                  pIons->getRealOrDummyObject(1).getTOF(*pUnit),
                  pIons->getRealOrDummyObject(2).getTOF(*pUnit));
  }
  const bool underMasterCondition =
      (pIons->areAllFlag(ObjectFlag::WithinMasterRegion)
          && pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion));
  if (underMasterCondition) {
    pHist->fill2d(histID_1stHitElecEAndSumOfIonTOFs_underMasterCondition,
                  pElectrons->getRealOrDummyObject(0).getEnergy(*pUnit),
                  pIons->getSumOfTOF(*pUnit));
    pHist->fill1d(histID_1stHitElecE_underMasterCondition,
                  pElectrons->getRealOrDummyObject(0).getEnergy(*pUnit));
  }
}
void Analysis::Run::flushHist() {
  pHist->flushRootFile();
}
void Analysis::Run::createIonHists() {
  // Detector image
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_4thHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_COMOfIonsLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_4thHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_COMOfIonsLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  // TOF
  pHist->create1d(SAMETITLEWITH(hist1ID_1stHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_2ndHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_3rdHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_4thHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_1stHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_2ndHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_3rdHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_4thHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  // PIPICO
  pHist->create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitIonTOF_notDead),
                  "1st Hit Ion TOF [ns] ", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitIonTOF_notDead),
                  "2nd Hit Ion TOF [ns] ", "3rd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitIonTOF_notDead),
                  "3rd Hit Ion TOF [ns] ", "4th Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead),
                  "Sum of 1st and 2nd Hit Ion TOF [ns] ", "Diff of 1st and 2nd Hit Ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_DIFFOFTOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitIonTOF_rot45NotDead),
                  "Sum of 2nd and 3rd Hit Ion TOF [ns] ", "Diff of 2nd and 3rd Hit Ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_DIFFOFTOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitIonTOF_rot45NotDead),
                  "Sum of 3rd and 4th Hit Ion TOF [ns] ", "Diff of 3rd and 4th Hit Ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_DIFFOFTOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitIonTOF_master),
                  "1st Hit Ion TOF [ns] ", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitIonTOF_master),
                  "2nd Hit Ion TOF [ns] ", "3rd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitIonTOF_master),
                  "3rd Hit Ion TOF [ns] ", "4th Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  // PIPIPICO
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF),
                  "Sum of 1st and 2nd hit ion TOFs [ns]", "3rd hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs),
                  "1st hit ion TOF [ns]", "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF),
                  "Sum of 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs),
                  "2nd hit ion TOF [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF_masterCondit),
                  "Sum of 1st and 2nd hit ion TOFs [ns]", "3rd hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs_masterCondit),
                  "1st hit ion TOF [ns]", "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF_masterCondit),
                  "Sum of 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs_masterCondit),
                  "2nd hit ion TOF [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  // PIPIPIPICO
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF),
                  "Sum of 1st, 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(3), H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF),
                  "Sum of 1st and 2nd hit ion TOFs [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF),
                  "1st hit ion TOF [ns]", "Sum of 2nd, 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(3),
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF_masterCondit),
                  "Sum of 1st, 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(3), H2_ION_TOF,
                  dirNameOfIonHists);
  pHist->create2d(
      SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF_masterCondit),
      "Sum of 1st and 2nd hit ion TOFs [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
      H2_ION_SUMOFTOF(2), H2_ION_SUMOFTOF(2),
      dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF_masterCondit),
                  "1st hit ion TOF [ns]", "Sum of 2nd, 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(3),
                  dirNameOfIonHists);
  //
  pHist->create3d(SAMETITLEWITH(hist3ID_1stHitIonPxPyPz),
                  "Momentum X [au]", "Momentum Y [au]", "Momentum Z [au]",
                  H3_ION_MOMENTUM, H3_ION_MOMENTUM, H3_ION_MOMENTUM,
                  dirNameOfIonHists);
  pHist->create3d(SAMETITLEWITH(hist3ID_1stHitIonPxPyPz_underMasterCondition),
                  "Momentum X [au]", "Momentum Y [au]", "Momentum Z [au]",
                  H3_ION_MOMENTUM, H3_ION_MOMENTUM, H3_ION_MOMENTUM,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_4thHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
}
void Analysis::Run::fillIonHists() {
  const bool dead1 = pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::Dead);
  const bool dead2 = pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead);
  const bool dead3 = pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead);
  const bool dead4 = pIons->getRealOrDummyObject(3).isFlag(ObjectFlag::Dead);
  const bool properP1 =
      pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::HavingProperPzData);
  const bool properP2 =
      pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::HavingProperPzData);
  const bool properP3 =
      pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::HavingProperPzData);
  const bool properP4 =
      pIons->getRealOrDummyObject(3).isFlag(ObjectFlag::HavingProperPzData);

  const bool isIonMaster = pIons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool isElecMaster = pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool isMaster = isIonMaster && isElecMaster;
  
  const double x1 = pIons->getRealOrDummyObject(0).getLocationX(*pUnit);
  const double y1 = pIons->getRealOrDummyObject(0).getLocationY(*pUnit);
  const double t1 = pIons->getRealOrDummyObject(0).getTOF(*pUnit);
  const double x2 = pIons->getRealOrDummyObject(1).getLocationX(*pUnit);
  const double y2 = pIons->getRealOrDummyObject(1).getLocationY(*pUnit);
  const double t2 = pIons->getRealOrDummyObject(1).getTOF(*pUnit);
  const double x3 = pIons->getRealOrDummyObject(2).getLocationX(*pUnit);
  const double y3 = pIons->getRealOrDummyObject(2).getLocationY(*pUnit);
  const double t3 = pIons->getRealOrDummyObject(2).getTOF(*pUnit);
  const double x4 = pIons->getRealOrDummyObject(3).getLocationX(*pUnit);
  const double y4 = pIons->getRealOrDummyObject(3).getLocationY(*pUnit);
  const double t4 = pIons->getRealOrDummyObject(3).getTOF(*pUnit);

  const double xCOM = pIons->getLocationX(*pUnit);
  const double yCOM = pIons->getLocationY(*pUnit);
  const double tSum12 = pIons->getSumOfTOF(*pUnit, 1, 2);
  const double tSum23 = pIons->getSumOfTOF(*pUnit, 2, 3);
  const double tSum34 = pIons->getSumOfTOF(*pUnit, 3, 4);
  const double tDiff12 = pIons->getDiffOfTOF(*pUnit, 1, 2);
  const double tDiff23 = pIons->getDiffOfTOF(*pUnit, 2, 3);
  const double tDiff34 = pIons->getDiffOfTOF(*pUnit, 3, 4);

  // Detector image and TOF
  if (!dead1) {
    pHist->fill2d(hist2ID_1stHitIonLocXY_notDead, x1, y1);
    pHist->fill1d(hist1ID_1stHitIonTOF_notDead, t1);
  }
  if (!dead2) {
    pHist->fill2d(hist2ID_2ndHitIonLocXY_notDead, x2, y2);
    pHist->fill1d(hist1ID_2ndHitIonTOF_notDead, t2);
  }
  if (!dead3) {
	  pHist->fill2d(hist2ID_3rdHitIonLocXY_notDead, x3, y3);
	  pHist->fill1d(hist1ID_3rdHitIonTOF_notDead, t3);
  }
  if (!dead4) {
	  pHist->fill2d(hist2ID_4thHitIonLocXY_notDead, x4, y4);
	  pHist->fill1d(hist1ID_4thHitIonTOF_notDead, t4);
  }
  if (isIonMaster) {
	  pHist->fill2d(hist2ID_COMOfIonsLocXY_notDead, xCOM, yCOM);
  }
  // PIPICO 
  if (!dead1 && !dead2) {
	  pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF_notDead, t1, t2);
    pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead, tSum12, tDiff12);
  }
  if (!dead2 && !dead3) {
	  pHist->fill2d(hist2ID_2ndAnd3rdHitIonTOF_notDead, t2, t3);
    pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead, tSum23, tDiff23);
  }
  if (!dead3 && !dead4) {
	  pHist->fill2d(hist2ID_3rdAnd4thHitIonTOF_notDead, t3, t4);
    pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead, tSum34, tDiff34);
  }
  // Under master condition 
  if (isMaster) {
	// Detector image and TOF
    if (!dead1) {
      pHist->fill2d(hist2ID_1stHitIonLocXY_master, x1, y1);
      pHist->fill1d(hist1ID_1stHitIonTOF_master, t1);
    }
    if (!dead2) {
      pHist->fill2d(hist2ID_2ndHitIonLocXY_master, x2, y2);
      pHist->fill1d(hist1ID_2ndHitIonTOF_master, t2);
    }
	if (!dead3) {
		pHist->fill2d(hist2ID_3rdHitIonLocXY_master, x3, y3);
		pHist->fill1d(hist1ID_3rdHitIonTOF_master, t3);
	}
	if (!dead4) {
		pHist->fill2d(hist2ID_4thHitIonLocXY_master, x4, y4);
		pHist->fill1d(hist1ID_4thHitIonTOF_master, t4);
	}
	// COM
	if (isIonMaster) {
		pHist->fill2d(hist2ID_COMOfIonsLocXY_master, xCOM, yCOM);
	}
	// PIPICO 
	if (!dead1 && !dead2) {
		pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF_master, t1, t2);
	}
	if (!dead2 && !dead3) {
		pHist->fill2d(hist2ID_2ndAnd3rdHitIonTOF_master, t2, t3);
	}
	if (!dead3 && !dead4) {
		pHist->fill2d(hist2ID_3rdAnd4thHitIonTOF_master, t3, t4);
	}
  }

  //
  if (properP1) {
    pHist->fill3d(hist3ID_1stHitIonPxPyPz,
                  pIons->getIon(0).getMomentumX(*pUnit),
                  pIons->getIon(0).getMomentumY(*pUnit),
                  pIons->getIon(0).getMomentumZ(*pUnit));
    pHist->fill2d(hist2ID_1stHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(0).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(0).getMotionalDirectionZ()));
  }
  if (properP2) {
    pHist->fill2d(hist2ID_2ndHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(1).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(1).getMotionalDirectionZ()));
  }
  if (properP3) {
    pHist->fill2d(hist2ID_3rdHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(2).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(2).getMotionalDirectionZ()));
  }
  if (properP4) {
    pHist->fill2d
        (hist2ID_4thHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(3).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(3).getMotionalDirectionZ()));
  }
  // Under master condition
  if (isMaster) {
    if (properP1) {
      pHist->fill3d(hist3ID_1stHitIonPxPyPz_underMasterCondition,
                    pIons->getIon(0).getMomentumX(*pUnit),
                    pIons->getIon(0).getMomentumY(*pUnit),
                    pIons->getIon(0).getMomentumZ(*pUnit));
    }
  }
}
void Analysis::Run::createElecHists() {
  // Detector image
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_4thHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_COMOfElecsLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_4thHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_COMOfElecsLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
   // TOF
  pHist->create1d(SAMETITLEWITH(hist1ID_1stHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_2ndHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_3rdHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_4thHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_1stHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_2ndHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_3rdHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create1d(SAMETITLEWITH(hist1ID_4thHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  // PIPICO
  pHist->create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitElecTOF_notDead),
                  "1st Hit Elec TOF [ns] ", "2nd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitElecTOF_notDead),
                  "2nd Hit Elec TOF [ns] ", "3rd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitElecTOF_notDead),
                  "3rd Hit Elec TOF [ns] ", "4th Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitElecTOF_master),
                  "1st Hit Elec TOF [ns] ", "2nd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitElecTOF_master),
                  "2nd Hit Elec TOF [ns] ", "3rd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  pHist->create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitElecTOF_master),
                  "3rd Hit Elec TOF [ns] ", "4th Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
}
void Analysis::Run::fillElecHists() {
	const bool dead1 = pElectrons->getRealOrDummyObject(0).isFlag(ObjectFlag::Dead);
	const bool dead2 = pElectrons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead);
	const bool dead3 = pElectrons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead);
	const bool dead4 = pElectrons->getRealOrDummyObject(3).isFlag(ObjectFlag::Dead);
	const bool properP1 =
		pElectrons->getRealOrDummyObject(0).isFlag(ObjectFlag::HavingProperPzData);
	const bool properP2 =
		pElectrons->getRealOrDummyObject(1).isFlag(ObjectFlag::HavingProperPzData);
	const bool properP3 =
		pElectrons->getRealOrDummyObject(2).isFlag(ObjectFlag::HavingProperPzData);
	const bool properP4 =
		pElectrons->getRealOrDummyObject(3).isFlag(ObjectFlag::HavingProperPzData);

	const bool isIonMaster = pIons->areAllFlag(ObjectFlag::WithinMasterRegion);
	const bool isElecMaster = pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion);
	const bool isMaster = isIonMaster && isElecMaster;

	const double x1 = pElectrons->getRealOrDummyObject(0).getLocationX(*pUnit);
	const double y1 = pElectrons->getRealOrDummyObject(0).getLocationY(*pUnit);
	const double t1 = pElectrons->getRealOrDummyObject(0).getTOF(*pUnit);
	const double x2 = pElectrons->getRealOrDummyObject(1).getLocationX(*pUnit);
	const double y2 = pElectrons->getRealOrDummyObject(1).getLocationY(*pUnit);
	const double t2 = pElectrons->getRealOrDummyObject(1).getTOF(*pUnit);
	const double x3 = pElectrons->getRealOrDummyObject(2).getLocationX(*pUnit);
	const double y3 = pElectrons->getRealOrDummyObject(2).getLocationY(*pUnit);
	const double t3 = pElectrons->getRealOrDummyObject(2).getTOF(*pUnit);
	const double x4 = pElectrons->getRealOrDummyObject(3).getLocationX(*pUnit);
	const double y4 = pElectrons->getRealOrDummyObject(3).getLocationY(*pUnit);
	const double t4 = pElectrons->getRealOrDummyObject(3).getTOF(*pUnit);

	const double xCOM = pElectrons->getLocationX(*pUnit);
	const double yCOM = pElectrons->getLocationY(*pUnit);
	const double tSum12 = pElectrons->getSumOfTOF(*pUnit, 1, 2);
	const double tSum23 = pElectrons->getSumOfTOF(*pUnit, 2, 3);
	const double tSum34 = pElectrons->getSumOfTOF(*pUnit, 3, 4);
	const double tDiff12 = pElectrons->getDiffOfTOF(*pUnit, 1, 2);
	const double tDiff23 = pElectrons->getDiffOfTOF(*pUnit, 2, 3);
	const double tDiff34 = pElectrons->getDiffOfTOF(*pUnit, 3, 4);

	// Detector image and TOF
	if (!dead1) {
		pHist->fill2d(hist2ID_1stHitElecLocXY_notDead, x1, y1);
		pHist->fill1d(hist1ID_1stHitElecTOF_notDead, t1);
	}
	if (!dead2) {
		pHist->fill2d(hist2ID_2ndHitElecLocXY_notDead, x2, y2);
		pHist->fill1d(hist1ID_2ndHitElecTOF_notDead, t2);
	}
	if (!dead3) {
		pHist->fill2d(hist2ID_3rdHitElecLocXY_notDead, x3, y3);
		pHist->fill1d(hist1ID_3rdHitElecTOF_notDead, t3);
	}
	if (!dead4) {
		pHist->fill2d(hist2ID_4thHitElecLocXY_notDead, x4, y4);
		pHist->fill1d(hist1ID_4thHitElecTOF_notDead, t4);
	}
	if (isElecMaster) {
		pHist->fill2d(hist2ID_COMOfElecsLocXY_notDead, xCOM, yCOM);
	}
	// PIPICO 
	if (!dead1 && !dead2) {
		pHist->fill2d(hist2ID_1stAnd2ndHitElecTOF_notDead, t1, t2);
	}
	if (!dead2 && !dead3) {
		pHist->fill2d(hist2ID_2ndAnd3rdHitElecTOF_notDead, t2, t3);
	}
	if (!dead3 && !dead4) {
		pHist->fill2d(hist2ID_3rdAnd4thHitElecTOF_notDead, t3, t4);
	}
	// Under master condition 
	if (isMaster) {
		// Detector image and TOF
		if (!dead1) {
			pHist->fill2d(hist2ID_1stHitElecLocXY_master, x1, y1);
			pHist->fill1d(hist1ID_1stHitElecTOF_master, t1);
		}
		if (!dead2) {
			pHist->fill2d(hist2ID_2ndHitElecLocXY_master, x2, y2);
			pHist->fill1d(hist1ID_2ndHitElecTOF_master, t2);
		}
		if (!dead3) {
			pHist->fill2d(hist2ID_3rdHitElecLocXY_master, x3, y3);
			pHist->fill1d(hist1ID_3rdHitElecTOF_master, t3);
		}
		if (!dead4) {
			pHist->fill2d(hist2ID_4thHitElecLocXY_master, x4, y4);
			pHist->fill1d(hist1ID_4thHitElecTOF_master, t4);
		}
		// COM
		if (isElecMaster) {
			pHist->fill2d(hist2ID_COMOfElecsLocXY_master, xCOM, yCOM);
		}
		// PIPICO 
		if (!dead1 && !dead2) {
			pHist->fill2d(hist2ID_1stAnd2ndHitElecTOF_master, t1, t2);
		}
		if (!dead2 && !dead3) {
			pHist->fill2d(hist2ID_2ndAnd3rdHitElecTOF_master, t2, t3);
		}
		if (!dead3 && !dead4) {
			pHist->fill2d(hist2ID_3rdAnd4thHitElecTOF_master, t3, t4);
		}
	}
}