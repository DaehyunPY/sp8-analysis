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

#include "AnalysisRun.h"

Analysis::AnalysisRun::AnalysisRun(const std::string configFilename): 
	Hist(false, numberOfHists) {
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
      for (std::string str : {"IonX", "IonY", "IonT", "ElecX", "ElecY", "ElecT"}) {
        sprintf(ch, "%01d", i);
        pEventChain->SetBranchAddress((str + ch).c_str(), &(pEventReader->setEventDataAt(i, str + ch)));
      }
      for (std::string str : {"IonFlag", "ElecFlag"}) {
        sprintf(ch, "%01d", i);
        pEventChain->SetBranchAddress((str + ch).c_str(), &(pEventReader->setFlagDataAt(i, str + ch)));
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
  std::cout << "open a root file... ";
  std::string rootFilename = "";
  rootFilename += pTools->getID();
  if (rootFilename != "") {
    rootFilename += "-";
  }
  rootFilename += pLogWriter->getID();
  rootFilename += ".root";
  openRootFile(rootFilename.c_str(), "NEW");
  createIonHists();
  createElecHists();
  createNatureHists();
  std::cout << "ok" << std::endl;

  // Initialization is done
  pLogWriter->write() << "Initialization is done." << std::endl;
  pLogWriter->write() << std::endl;
}

Analysis::AnalysisRun::~AnalysisRun() {
  // counter
  pLogWriter->write() << "Event count: " << pTools->getEventNumber() << std::endl;

  // flush ROOT file
  flushRootFile();

  // finalization is done
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

void Analysis::AnalysisRun::processEvent(const long raw) {
  // Setup event chain
  pEventChain->GetEntry(raw);

  // Count event
  pTools->loadEventCounter();

  // make sure ion and electron data is empty, and reset resortElecFlags
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

  // Fill hists for resort resortElecFlags
}

const long Analysis::AnalysisRun::getEntries() const {
  return (long) pEventChain->GetEntries();
}

void Analysis::AnalysisRun::createNatureHists() {
  create1d(SAMETITLEWITH(histID_1stHitIonTOF_under2ndAnd3rdHitIonAreNotDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfNatureHists);
  create2d(SAMETITLEWITH(histID_2ndAnd3rdHitIonTOF_under1stHitIonIsInMasterRegion),
                  "1st Hit Ion TOF [ns]", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfNatureHists);
  create2d(SAMETITLEWITH(histID_1stHitElecEAndSumOfIonTOFs_underMasterCondition),
                  "Energy [eV]", "Sum of TOFs [ns]",
                  H2_ELECTRON_ENERGY, H2_ION_SUMOFTOF(4),
                  dirNameOfNatureHists);
  create1d(SAMETITLEWITH(histID_1stHitElecE_underMasterCondition),
                  "Energy [eV]",
                  H1_ELECTRON_ENERGY,
                  dirNameOfNatureHists);
}

void Analysis::AnalysisRun::fillNatureHists() {
  const bool under2ndAnd3rdHitIonAreNotDead =
      (!pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead))
          && (!pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead));
  if (under2ndAnd3rdHitIonAreNotDead) {
    fill1d(histID_1stHitIonTOF_under2ndAnd3rdHitIonAreNotDead,
                  pIons->getRealOrDummyObject(0).outputTOF());
  }
  const bool under1stHitIonInMasterRegion =
      pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::WithinMasterRegion);
  if (under1stHitIonInMasterRegion) {
    fill2d(histID_2ndAnd3rdHitIonTOF_under1stHitIonIsInMasterRegion,
                  pIons->getRealOrDummyObject(1).outputTOF(),
                  pIons->getRealOrDummyObject(2).outputTOF());
  }
  const bool underMasterCondition =
      (pIons->areAllFlag(ObjectFlag::WithinMasterRegion)
          && pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion));
  if (underMasterCondition) {
    fill2d(histID_1stHitElecEAndSumOfIonTOFs_underMasterCondition,
                  pElectrons->getRealOrDummyObject(0).outputE(),
                  pIons->outputTotalTOF());
    fill1d(histID_1stHitElecE_underMasterCondition,
                  pElectrons->getRealOrDummyObject(0).outputE());
  }
}

void Analysis::AnalysisRun::createIonHists() {
  // Detector image
  create2d(SAMETITLEWITH(hist2ID_1stHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitIonLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_COMOfIonsLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitIonLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_COMOfIonsLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHists);
  // TOF
  create1d(SAMETITLEWITH(hist1ID_1stHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitIonTOF_notDead),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_1stHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitIonTOF_master),
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHists);
  // PIPICO
  create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitIonTOF_notDead),
                  "1st Hit Ion TOF [ns] ", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitIonTOF_notDead),
                  "2nd Hit Ion TOF [ns] ", "3rd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitIonTOF_notDead),
                  "3rd Hit Ion TOF [ns] ", "4th Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead),
                  "Sum of 1st and 2nd Hit Ion TOF [ns] ", "Diff of 1st and 2nd Hit Ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_DIFFOFTOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitIonTOF_rot45NotDead),
                  "Sum of 2nd and 3rd Hit Ion TOF [ns] ", "Diff of 2nd and 3rd Hit Ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_DIFFOFTOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitIonTOF_rot45NotDead),
                  "Sum of 3rd and 4th Hit Ion TOF [ns] ", "Diff of 3rd and 4th Hit Ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_DIFFOFTOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitIonTOF_master),
                  "1st Hit Ion TOF [ns] ", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitIonTOF_master),
                  "2nd Hit Ion TOF [ns] ", "3rd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitIonTOF_master),
                  "3rd Hit Ion TOF [ns] ", "4th Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHists);
  // PIPIPICO
  create2d(SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF),
                  "Sum of 1st and 2nd hit ion TOFs [ns]", "3rd hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs),
                  "1st hit ion TOF [ns]", "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF),
                  "Sum of 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs),
                  "2nd hit ion TOF [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF_masterCondit),
                  "Sum of 1st and 2nd hit ion TOFs [ns]", "3rd hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs_masterCondit),
                  "1st hit ion TOF [ns]", "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF_masterCondit),
                  "Sum of 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs_masterCondit),
                  "2nd hit ion TOF [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  // PIPIPIPICO
  create2d(SAMETITLEWITH(hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF),
                  "Sum of 1st, 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(3), H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF),
                  "Sum of 1st and 2nd hit ion TOFs [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_SUMOFTOF(2), H2_ION_SUMOFTOF(2),
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF),
                  "1st hit ion TOF [ns]", "Sum of 2nd, 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(3),
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF_masterCondit),
                  "Sum of 1st, 2nd and 3rd hit ion TOFs [ns]", "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(3), H2_ION_TOF,
                  dirNameOfIonHists);
  create2d(
      SAMETITLEWITH(hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF_masterCondit),
      "Sum of 1st and 2nd hit ion TOFs [ns]", "Sum of 3rd and 4th hit ion TOFs [ns]",
      H2_ION_SUMOFTOF(2), H2_ION_SUMOFTOF(2),
      dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF_masterCondit),
                  "1st hit ion TOF [ns]", "Sum of 2nd, 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF, H2_ION_SUMOFTOF(3),
                  dirNameOfIonHists);
  // Ion FISH
#define __ION_XYFISH_TITLE_BIN_REGION_DIR__ "TOF [ns]", "Location [mm]", 500, 0, 10000, 400, -50, 50, "IonFish"
#define __ION_RFISH_TITLE_BIN_REGION_DIR__ "TOF [ns]", "Location [mm]", 500, 0, 10000, 200, 0, 50, "IonFish"
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hXFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hXFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hXFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hYFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hYFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hYFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hRFish_always), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hRFish_notDead), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hRFish_master), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hXFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hXFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hXFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hYFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hYFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hYFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hRFish_always), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hRFish_notDead), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hRFish_master), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hXFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hXFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hXFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hYFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hYFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hYFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hRFish_always), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hRFish_notDead), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hRFish_master), __ION_RFISH_TITLE_BIN_REGION_DIR__);

  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hXFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hXFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hXFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hYFish_always), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hYFish_notDead), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hYFish_master), __ION_XYFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hRFish_always), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hRFish_notDead), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hRFish_master), __ION_RFISH_TITLE_BIN_REGION_DIR__);
  // Momentum
  create3d(SAMETITLEWITH(hist3ID_1stHitIonPxPyPz),
                  "Momentum X [au]", "Momentum Y [au]", "Momentum Z [au]",
                  H3_ION_MOMENTUM, H3_ION_MOMENTUM, H3_ION_MOMENTUM,
                  dirNameOfIonHists);
  create3d(SAMETITLEWITH(hist3ID_1stHitIonPxPyPz_underMasterCondition),
                  "Momentum X [au]", "Momentum Y [au]", "Momentum Z [au]",
                  H3_ION_MOMENTUM, H3_ION_MOMENTUM, H3_ION_MOMENTUM,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitIonPDirecXYAndCosPDirecZ),
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHists);
  // IonEnergy
#define __IONENERGY1__ "Energy [eV]", 1000, 0, 50, "IonEnergy"
#define __IONENERGY2__ "Energy 1 [eV]", "Energy 2 [eV]", 200, 0, 50, 200, 0, 50, "IonEnergy"
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hE), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hE_master), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hE), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hE_master), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hE), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hE_master), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hE), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hE_master), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTtE), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTtE_master), __IONENERGY1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hE_i2hE), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hE_i2hE_master), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hE_i3hE), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hE_i3hE_master), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hE_i4hE), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hE_i4hE_master), __IONENERGY2__);

// KER
#define __KER_TITLE_BIN_REGION_DIR__ "KER [eV]", "Electron Kinetic Energy [eV]", 1000, 0, 100, 500, 0, 50, "KER"
  create2d(SAME_TITLE_WITH_VALNAME(h2_iKER_e1hE), __KER_TITLE_BIN_REGION_DIR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iKER_e1hE_master), __KER_TITLE_BIN_REGION_DIR__);
}

void Analysis::AnalysisRun::fillIonHists() {
  const bool dead1 = pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::Dead);
  const bool dead2 = pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead);
  const bool dead3 = pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead);
  const bool dead4 = pIons->getRealOrDummyObject(3).isFlag(ObjectFlag::Dead);
  const bool isIonMaster = pIons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool isElecMaster = pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool isMaster = isIonMaster && isElecMaster;
  
  const double * const x1 = pIons->getRealOrDummyObject(0).outputLocX();
  const double * const y1 = pIons->getRealOrDummyObject(0).outputLocY();
  const double * const r1 = pIons->getRealOrDummyObject(0).outputLocR();
  const double * const t1 = pIons->getRealOrDummyObject(0).outputTOF();
  const double * const x2 = pIons->getRealOrDummyObject(1).outputLocX();
  const double * const y2 = pIons->getRealOrDummyObject(1).outputLocY();
  const double * const r2 = pIons->getRealOrDummyObject(1).outputLocR();
  const double * const t2 = pIons->getRealOrDummyObject(1).outputTOF();
  const double * const x3 = pIons->getRealOrDummyObject(2).outputLocX();
  const double * const y3 = pIons->getRealOrDummyObject(2).outputLocY();
  const double * const r3 = pIons->getRealOrDummyObject(2).outputLocR();
  const double * const t3 = pIons->getRealOrDummyObject(2).outputTOF();
  const double * const x4 = pIons->getRealOrDummyObject(3).outputLocX();
  const double * const y4 = pIons->getRealOrDummyObject(3).outputLocY();
  const double * const r4 = pIons->getRealOrDummyObject(3).outputLocR();
  const double * const t4 = pIons->getRealOrDummyObject(3).outputTOF();

  const double * const xCOM = pIons->outputCOMLocX();
  const double * const yCOM = pIons->outputCOMLocY();
  const double * const tSum12 = pIons->outputSumOf2TOFs(0, 1);
  const double * const tSum23 = pIons->outputSumOf2TOFs(1, 2);
  const double * const tSum34 = pIons->outputSumOf2TOFs(2, 3);
  const double * const tDiff12 = pIons->outputDiffOfTOFs(0, 1);
  const double * const tDiff23 = pIons->outputDiffOfTOFs(1, 2);
  const double * const tDiff34 = pIons->outputDiffOfTOFs(2, 3);

  const double * const px1 = pIons->getRealOrDummyObject(0).outputPX();
  const double * const py1 = pIons->getRealOrDummyObject(0).outputPY();
  const double * const pxy1 = pIons->getRealOrDummyObject(0).outputPXY();
  const double * const pz1 = pIons->getRealOrDummyObject(0).outputPZ();
  const double * const cpz1 = pIons->getRealOrDummyObject(0).outputCosPDirZ();
  const double * const px2 = pIons->getRealOrDummyObject(1).outputPX();
  const double * const py2 = pIons->getRealOrDummyObject(1).outputPY();
  const double * const pxy2 = pIons->getRealOrDummyObject(1).outputPXY();
  const double * const pz2 = pIons->getRealOrDummyObject(1).outputPZ();
  const double * const cpz2 = pIons->getRealOrDummyObject(1).outputCosPDirZ();
  const double * const px3 = pIons->getRealOrDummyObject(2).outputPX();
  const double * const py3 = pIons->getRealOrDummyObject(2).outputPY();
  const double * const pxy3 = pIons->getRealOrDummyObject(2).outputPXY();
  const double * const pz3 = pIons->getRealOrDummyObject(2).outputPZ();
  const double * const cpz3 = pIons->getRealOrDummyObject(2).outputCosPDirZ();
  const double * const px4 = pIons->getRealOrDummyObject(3).outputPX();
  const double * const py4 = pIons->getRealOrDummyObject(3).outputPY();
  const double * const pxy4 = pIons->getRealOrDummyObject(3).outputPXY();
  const double * const pz4 = pIons->getRealOrDummyObject(3).outputPZ();
  const double * const cpz4 = pIons->getRealOrDummyObject(3).outputCosPDirZ();

  const double * const e1 = pIons->getRealOrDummyIon(0).outputE();
  const double * const e2 = pIons->getRealOrDummyIon(1).outputE();
  const double * const e3 = pIons->getRealOrDummyIon(2).outputE();
  const double * const e4 = pIons->getRealOrDummyIon(3).outputE();
  const double * const eT = pIons->outputE();

  const double * const eE1 = pElectrons->getRealOrDummyElectron(0).outputE();

  // Detector image and TOF
  if (!dead1) {
    fill2d(hist2ID_1stHitIonLocXY_notDead, x1, y1);
    fill1d(hist1ID_1stHitIonTOF_notDead, t1);
  }
  if (!dead2) {
    fill2d(hist2ID_2ndHitIonLocXY_notDead, x2, y2);
    fill1d(hist1ID_2ndHitIonTOF_notDead, t2);
  }
  if (!dead3) {
	  fill2d(hist2ID_3rdHitIonLocXY_notDead, x3, y3);
	  fill1d(hist1ID_3rdHitIonTOF_notDead, t3);
  }
  if (!dead4) {
	  fill2d(hist2ID_4thHitIonLocXY_notDead, x4, y4);
	  fill1d(hist1ID_4thHitIonTOF_notDead, t4);
  }
  if (isIonMaster) {
	  fill2d(hist2ID_COMOfIonsLocXY_notDead, xCOM, yCOM);
  }
  if (isMaster) {
	  if (!dead1) {
		  fill2d(hist2ID_1stHitIonLocXY_master, x1, y1);
		  fill1d(hist1ID_1stHitIonTOF_master, t1);
	  }
	  if (!dead2) {
		  fill2d(hist2ID_2ndHitIonLocXY_master, x2, y2);
		  fill1d(hist1ID_2ndHitIonTOF_master, t2);
	  }
	  if (!dead3) {
		  fill2d(hist2ID_3rdHitIonLocXY_master, x3, y3);
		  fill1d(hist1ID_3rdHitIonTOF_master, t3);
	  }
	  if (!dead4) {
		  fill2d(hist2ID_4thHitIonLocXY_master, x4, y4);
		  fill1d(hist1ID_4thHitIonTOF_master, t4);
	  }
	  if (isIonMaster) {
		  fill2d(hist2ID_COMOfIonsLocXY_master, xCOM, yCOM);
	  }
  }
  // PIPICO 
  if (!dead1 && !dead2) {
	  fill2d(hist2ID_1stAnd2ndHitIonTOF_notDead, t1, t2);
    fill2d(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead, tSum12, tDiff12);
  }
  if (!dead2 && !dead3) {
	  fill2d(hist2ID_2ndAnd3rdHitIonTOF_notDead, t2, t3);
    fill2d(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead, tSum23, tDiff23);
  }
  if (!dead3 && !dead4) {
	  fill2d(hist2ID_3rdAnd4thHitIonTOF_notDead, t3, t4);
    fill2d(hist2ID_1stAnd2ndHitIonTOF_rot45NotDead, tSum34, tDiff34);
  }
  if (isMaster) {
	if (!dead1 && !dead2) {
		fill2d(hist2ID_1stAnd2ndHitIonTOF_master, t1, t2);
	}
	if (!dead2 && !dead3) {
		fill2d(hist2ID_2ndAnd3rdHitIonTOF_master, t2, t3);
	}
	if (!dead3 && !dead4) {
		fill2d(hist2ID_3rdAnd4thHitIonTOF_master, t3, t4);
	}
  }
  // FISH
  fill2d(h2_i1hXFish_always, t1, x1);
  if (!dead1) fill2d(h2_i1hXFish_notDead, t1, x1);
  if (!dead1 && isMaster) fill2d(h2_i1hXFish_master, t1, x1);
  fill2d(h2_i1hYFish_always, t1, y1);
  if (!dead1) fill2d(h2_i1hYFish_notDead, t1, y1);
  if (!dead1 && isMaster) fill2d(h2_i1hYFish_master, t1, y1);
  fill2d(h2_i1hRFish_always, t1, r1);
  if (!dead1) fill2d(h2_i1hRFish_notDead, t1, r1);
  if (!dead1 && isMaster) fill2d(h2_i1hRFish_master, t1, r1);

  fill2d(h2_i2hXFish_always, t2, x2);
  if (!dead2) fill2d(h2_i2hXFish_notDead, t2, x2);
  if (!dead2 && isMaster) fill2d(h2_i2hXFish_master, t2, x2);
  fill2d(h2_i2hYFish_always, t2, y2);
  if (!dead2) fill2d(h2_i2hYFish_notDead, t2, y2);
  if (!dead2 && isMaster) fill2d(h2_i2hYFish_master, t2, y2);
  fill2d(h2_i2hRFish_always, t2, r2);
  if (!dead2) fill2d(h2_i2hRFish_notDead, t2, r2);
  if (!dead2 && isMaster) fill2d(h2_i2hRFish_master, t2, r2);

  fill2d(h2_i3hXFish_always, t3, x3);
  if (!dead3) fill2d(h2_i3hXFish_notDead, t3, x3);
  if (!dead3 && isMaster) fill2d(h2_i3hXFish_master, t3, x3);
  fill2d(h2_i3hYFish_always, t3, y3);
  if (!dead3) fill2d(h2_i3hYFish_notDead, t3, y3);
  if (!dead3 && isMaster) fill2d(h2_i3hYFish_master, t3, y3);
  fill2d(h2_i3hRFish_always, t3, r3);
  if (!dead3) fill2d(h2_i3hRFish_notDead, t3, r3);
  if (!dead3 && isMaster) fill2d(h2_i3hRFish_master, t3, r3);

  fill2d(h2_i4hXFish_always, t4, x4);
  if (!dead4) fill2d(h2_i4hXFish_notDead, t4, x4);
  if (!dead4 && isMaster) fill2d(h2_i4hXFish_master, t4, x4);
  fill2d(h2_i4hYFish_always, t4, y4);
  if (!dead4) fill2d(h2_i4hYFish_notDead, t4, y4);
  if (!dead4 && isMaster) fill2d(h2_i4hYFish_master, t4, y4);
  fill2d(h2_i4hRFish_always, t4, r4);
  if (!dead4) fill2d(h2_i4hRFish_notDead, t4, r4);
  if (!dead4 && isMaster) fill2d(h2_i4hRFish_master, t4, r4);

  // Momentum 
  fill3d(hist3ID_1stHitIonPxPyPz, px1, py1, pz1);
  fill2d(hist2ID_1stHitIonPDirecXYAndCosPDirecZ, pxy1, cpz1);
  fill2d(hist2ID_2ndHitIonPDirecXYAndCosPDirecZ, pxy2, cpz2);
  fill2d(hist2ID_3rdHitIonPDirecXYAndCosPDirecZ, pxy3, cpz3);
  fill2d(hist2ID_4thHitIonPDirecXYAndCosPDirecZ, pxy4, cpz4);
  if (isMaster) fill3d(hist3ID_1stHitIonPxPyPz_underMasterCondition, px1, py2, pz1);

  // IonEnergy 
  fill1d(h1_i1hE, e1);
  fill1d(h1_i2hE, e2);
  fill1d(h1_i3hE, e3);
  fill1d(h1_i4hE, e4);
  fill1d(h1_iTtE, eT);
  fill2d(h2_i1hE_i2hE, e1, e2);
  fill2d(h2_i2hE_i3hE, e2, e3);
  fill2d(h2_i3hE_i4hE, e3, e4);
  if (isMaster) {
	  fill1d(h1_i1hE_master, e1);
	  fill1d(h1_i2hE_master, e2);
	  fill1d(h1_i3hE_master, e3);
	  fill1d(h1_i4hE_master, e4);
	  fill1d(h1_iTtE_master, eT);
  fill2d(h2_i1hE_i2hE_master, e1, e2);
  fill2d(h2_i2hE_i3hE_master, e2, e3);
  fill2d(h2_i3hE_i4hE_master, e3, e4);
  }
  
  // KER
  fill2d(h2_iKER_e1hE, eT, eE1);
  if (isMaster)
  {
	  fill2d(h2_iKER_e1hE_master, eT, eE1);
  }
}

void Analysis::AnalysisRun::createElecHists() {
  // Detector image
  create2d(SAMETITLEWITH(hist2ID_1stHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitElecLocXY_notDead),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitElecLocXY_master),
                  "Location X [mm]", "Location Y [mm]",
                  H2_ELECTRON_LOCATION, H2_ELECTRON_LOCATION,
                  dirNameOfElecHists);
   // TOF
  create1d(SAMETITLEWITH(hist1ID_1stHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitElecTOF_notDead),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_1stHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitElecTOF_master),
                  "TOF [ns]",
                  H1_ELECTRON_TOF,
                  dirNameOfElecHists);
  // PIPICO
  create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitElecTOF_notDead),
                  "1st Hit Elec TOF [ns] ", "2nd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitElecTOF_notDead),
                  "2nd Hit Elec TOF [ns] ", "3rd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitElecTOF_notDead),
                  "3rd Hit Elec TOF [ns] ", "4th Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stAnd2ndHitElecTOF_master),
                  "1st Hit Elec TOF [ns] ", "2nd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_2ndAnd3rdHitElecTOF_master),
                  "2nd Hit Elec TOF [ns] ", "3rd Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_3rdAnd4thHitElecTOF_master),
                  "3rd Hit Elec TOF [ns] ", "4th Hit Elec TOF [ns]",
                  H2_ELECTRON_TOF, H2_ELECTRON_TOF,
                  dirNameOfElecHists);
  // FISH
  create2d(SAMETITLEWITH(hist2ID_1stHitElecTOFAndLocXY_notDead),
	  "TOF [ns]", "Location_xy [mm]",
	  H2_ELECTRON_TOF, H2_ELECTRON_RADIUS,
	  dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecTOFAndLocXY_master),
	  "TOF [ns]", "Location_xy [mm]",
	  H2_ELECTRON_TOF, H2_ELECTRON_RADIUS,
	  dirNameOfElecHists);
  // Momentum 
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPxPy_notDead),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitElecPxPy_notDead),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitElecPxPy_notDead),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitElecPxPy_notDead),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_1stHitElecPz_notDead),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitElecPz_notDead),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitElecPz_notDead),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitElecPz_notDead),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPxPy_master),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_2ndHitElecPxPy_master),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_3rdHitElecPxPy_master),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_4thHitElecPxPy_master),
	  "P_x [au]", "P_y [au]",
	  H2_ELECTRON_MOMENTUM, H2_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_1stHitElecPz_master),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitElecPz_master),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitElecPz_master),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitElecPz_master),
	  "P_z [au]",
	  H1_ELECTRON_MOMENTUM,
    dirNameOfElecHists);
  // Momentum Direction
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirXYAndPXY_notDead),
	  "Direction_xy [degree]", "P_xy [au]",
	  H2_DEGREE, H1_ELECTRON_NORMOFMOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirYZAndPYZ_notDead),
	  "Direction_yz [degree]", "P_yz [au]",
	  H2_DEGREE, H1_ELECTRON_NORMOFMOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirZXAndPZX_notDead),
	  "Direction_zx [degree]", "P_zx [au]",
	  H2_DEGREE, H1_ELECTRON_NORMOFMOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirXYAndCosPDirZ_notDead),
	  "Direction_xy [degree]", "Cos Direction_z [1]",
	  H2_DEGREE, H2_SINCOS,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirXYAndPXY_master),
	  "Direction_xy [degree]", "P_xy [au]",
	  H2_DEGREE, H1_ELECTRON_NORMOFMOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirYZAndPYZ_master),
	  "Direction_yz [degree]", "P_yz [au]",
	  H2_DEGREE, H1_ELECTRON_NORMOFMOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirZXAndPZX_master),
	  "Direction_zx [degree]", "P_zx [au]",
	  H2_DEGREE, H1_ELECTRON_NORMOFMOMENTUM,
    dirNameOfElecHists);
  create2d(SAMETITLEWITH(hist2ID_1stHitElecPDirXYAndCosPDirZ_master),
	  "Direction_xy [degree]", "Cos Direction_z [1]",
	  H2_DEGREE, H2_SINCOS,
    dirNameOfElecHists);

  // Energy 
  create1d(SAMETITLEWITH(hist1ID_1stHitElecE),
	  "E [eV]",
	  H1_ELECTRON_ENERGY,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_2ndHitElecE),
	  "E [eV]",
	  H1_ELECTRON_ENERGY,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_3rdHitElecE),
	  "E [eV]",
	  H1_ELECTRON_ENERGY,
    dirNameOfElecHists);
  create1d(SAMETITLEWITH(hist1ID_4thHitElecE),
	  "E [eV]",
	  H1_ELECTRON_ENERGY,
    dirNameOfElecHists);
}

void Analysis::AnalysisRun::fillElecHists() {
	const bool dead1 = pElectrons->getRealOrDummyObject(0).isFlag(ObjectFlag::Dead);
	const bool dead2 = pElectrons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead);
	const bool dead3 = pElectrons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead);
	const bool dead4 = pElectrons->getRealOrDummyObject(3).isFlag(ObjectFlag::Dead);
	const bool isIonMaster = pIons->areAllFlag(ObjectFlag::WithinMasterRegion);
	const bool isElecMaster = pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion);
	const bool isMaster = isIonMaster && isElecMaster;

	const double * const x1 = pElectrons->getRealOrDummyObject(0).outputLocX();
	const double * const y1 = pElectrons->getRealOrDummyObject(0).outputLocY();
	const double * const r1 = pElectrons->getRealOrDummyObject(0).outputLocR();
	const double * const t1 = pElectrons->getRealOrDummyObject(0).outputTOF();
	const double * const x2 = pElectrons->getRealOrDummyObject(1).outputLocX();
	const double * const y2 = pElectrons->getRealOrDummyObject(1).outputLocY();
  const double * const r2 = pElectrons->getRealOrDummyObject(1).outputLocR();
	const double * const t2 = pElectrons->getRealOrDummyObject(1).outputTOF();
	const double * const x3 = pElectrons->getRealOrDummyObject(2).outputLocX();
	const double * const y3 = pElectrons->getRealOrDummyObject(2).outputLocY();
  const double * const r3 = pElectrons->getRealOrDummyObject(2).outputLocR();
	const double * const t3 = pElectrons->getRealOrDummyObject(2).outputTOF();
	const double * const x4 = pElectrons->getRealOrDummyObject(3).outputLocX();
	const double * const y4 = pElectrons->getRealOrDummyObject(3).outputLocY();
  const double * const r4 = pElectrons->getRealOrDummyObject(3).outputLocR();
	const double * const t4 = pElectrons->getRealOrDummyObject(3).outputTOF();

	const double * const xCOM = pElectrons->outputCOMLocX();
	const double * const yCOM = pElectrons->outputCOMLocY();
	const double * const tSum12 = pElectrons->outputSumOf2TOFs(0, 1);
	const double * const tSum23 = pElectrons->outputSumOf2TOFs(1, 2);
	const double * const tSum34 = pElectrons->outputSumOf2TOFs(2, 3);
	const double * const tDiff12 = pElectrons->outputDiffOfTOFs(0, 1);
	const double * const tDiff23 = pElectrons->outputDiffOfTOFs(1, 2);
	const double * const tDiff34 = pElectrons->outputDiffOfTOFs(2, 3);

	const double * const px1 = pElectrons->getRealOrDummyObject(0).outputPX();
	const double * const py1 = pElectrons->getRealOrDummyObject(0).outputPY();
	const double * const pz1 = pElectrons->getRealOrDummyObject(0).outputPZ();
	const double * const pxy1 = pElectrons->getRealOrDummyObject(0).outputPXY();
	const double * const pyz1 = pElectrons->getRealOrDummyObject(0).outputPYZ();
	const double * const pzx1 = pElectrons->getRealOrDummyObject(0).outputPZX();

	const double * const pDirXY1 = pElectrons->getRealOrDummyObject(0).outputPDirX();
	const double * const pDirYZ1 = pElectrons->getRealOrDummyObject(0).outputPDirYZ();
	const double * const pDirZX1 = pElectrons->getRealOrDummyObject(0).outputPDirZX();
	const double * const cpz1 = pElectrons->getRealOrDummyObject(0).outputCosPDirZ();
	
	const double * const e1 = pElectrons->getRealOrDummyObject(0).outputE();
  const double * const e2 = pElectrons->getRealOrDummyObject(1).outputE();
  const double * const e3 = pElectrons->getRealOrDummyObject(2).outputE();
  const double * const e4 = pElectrons->getRealOrDummyObject(3).outputE();

	// Detector image and TOF
	if (!dead1) {
		fill2d(hist2ID_1stHitElecLocXY_notDead, x1, y1);
		fill1d(hist1ID_1stHitElecTOF_notDead, t1);
	}
	if (!dead2) {
		fill2d(hist2ID_2ndHitElecLocXY_notDead, x2, y2);
		fill1d(hist1ID_2ndHitElecTOF_notDead, t2);
	}
	if (!dead3) {
		fill2d(hist2ID_3rdHitElecLocXY_notDead, x3, y3);
		fill1d(hist1ID_3rdHitElecTOF_notDead, t3);
	}
	if (!dead4) {
		fill2d(hist2ID_4thHitElecLocXY_notDead, x4, y4);
		fill1d(hist1ID_4thHitElecTOF_notDead, t4);
	}
	if (isMaster) {
		if (!dead1) {
			fill2d(hist2ID_1stHitElecLocXY_master, x1, y1);
			fill1d(hist1ID_1stHitElecTOF_master, t1);
		}
		if (!dead2) {
			fill2d(hist2ID_2ndHitElecLocXY_master, x2, y2);
			fill1d(hist1ID_2ndHitElecTOF_master, t2);
		}
		if (!dead3) {
			fill2d(hist2ID_3rdHitElecLocXY_master, x3, y3);
			fill1d(hist1ID_3rdHitElecTOF_master, t3);
		}
		if (!dead4) {
			fill2d(hist2ID_4thHitElecLocXY_master, x4, y4);
			fill1d(hist1ID_4thHitElecTOF_master, t4);
		}
	}
	// PIPICO 
	if (!dead1 && !dead2) {
		fill2d(hist2ID_1stAnd2ndHitElecTOF_notDead, t1, t2);
	}
	if (!dead2 && !dead3) {
		fill2d(hist2ID_2ndAnd3rdHitElecTOF_notDead, t2, t3);
	}
	if (!dead3 && !dead4) {
		fill2d(hist2ID_3rdAnd4thHitElecTOF_notDead, t3, t4);
	}
	if (isMaster) { 
		if (!dead1 && !dead2) {
			fill2d(hist2ID_1stAnd2ndHitElecTOF_master, t1, t2);
		}
		if (!dead2 && !dead3) {
			fill2d(hist2ID_2ndAnd3rdHitElecTOF_master, t2, t3);
		}
		if (!dead3 && !dead4) {
			fill2d(hist2ID_3rdAnd4thHitElecTOF_master, t3, t4);
		}
	}
	// FISH 
	if (!dead1) {
		fill2d(hist2ID_1stHitElecTOFAndLocXY_notDead, t1, r1);
	}
	if (isMaster) {
		if (!dead1) {
			fill2d(hist2ID_1stHitElecTOFAndLocXY_master, t1, r1);
		}
	}
	// Momentum 
		fill2d(hist2ID_1stHitElecPxPy_notDead, px1, py1);
		fill1d(hist1ID_1stHitElecPz_notDead, pz1);
	if (isMaster) {
			fill2d(hist2ID_1stHitElecPxPy_master, px1, py1);
			fill1d(hist1ID_1stHitElecPz_master, pz1);
	}
	// Momentum Direction 
		fill2d(hist2ID_1stHitElecPDirXYAndPXY_notDead, pDirXY1, pxy1);
		fill2d(hist2ID_1stHitElecPDirYZAndPYZ_notDead, pDirYZ1, pyz1);
		fill2d(hist2ID_1stHitElecPDirZXAndPZX_notDead, pDirZX1, pzx1);
		fill2d(hist2ID_1stHitElecPDirXYAndCosPDirZ_notDead, pDirXY1, cpz1);
	if (isMaster) {
			fill2d(hist2ID_1stHitElecPDirXYAndPXY_master, pDirXY1, pxy1);
			fill2d(hist2ID_1stHitElecPDirYZAndPYZ_master, pDirYZ1, pyz1);
			fill2d(hist2ID_1stHitElecPDirZXAndPZX_master, pDirZX1, pzx1);
			fill2d(hist2ID_1stHitElecPDirXYAndCosPDirZ_master, pDirXY1, cpz1);
	}
	// Energy 
		fill1d(hist1ID_1stHitElecE, e1);
}