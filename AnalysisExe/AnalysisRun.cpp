//
// Created by Daehyun You on 12/30/15.
//

#include "AnalysisRun.h"

Analysis::AnalysisRun::AnalysisRun(const std::string configFilename)
    : Hist(false, numberOfHists) {
  // Setup json file reader
  Analysis::JSONReader configReader(configFilename);

  // Change the working directory
  chdir(configReader.getStringAt("working_directory").c_str());

  // Setup writer
  pLogWriter = new Analysis::LogWriter(configReader);
  pLogWriter->logResultOfLoadingJSONFile(configReader);

  // Setup input ROOT files
  std::cout << "Setting up input root files... ";
  pEventChain =
      new TChain(configReader.getStringAt("setup_input.tree_name").c_str());
  pEventChain->Add(configReader.getStringAt("setup_input.filenames").c_str());
  pLogWriter->write() << "Filenames: "
                      << configReader.getStringAt("setup_input.filenames").c_str()
                      << std::endl;
  maxNumOfIonHits =
      configReader.getIntAt("setup_input.max_number_of_ion_hits");
  maxNumOfElecHits =
      configReader.getIntAt("setup_input.max_number_of_electron_hits");
  pEventReader =
      new Analysis::EventDataReader(maxNumOfIonHits, maxNumOfElecHits);
  char ch[2];
  for (int i = 0; i < maxNumOfIonHits; i++) {
    for (std::string str : {"IonX", "IonY", "IonT"}) {
      sprintf(ch, "%01d", i);
      pEventChain->SetBranchAddress((str + ch).c_str(),
                                    &(pEventReader->setEventDataAt(i,
                                                                   str + ch)));
    }
    for (std::string str : {"IonFlag"}) {
      sprintf(ch, "%01d", i);
      pEventChain->SetBranchAddress((str + ch).c_str(),
                                    &(pEventReader->setFlagDataAt(i,
                                                                  str + ch)));
    }
  }
  for (int i = 0; i < maxNumOfElecHits; i++) {
    for (std::string str : {"ElecX", "ElecY", "ElecT"}) {
      sprintf(ch, "%01d", i);
      pEventChain->SetBranchAddress((str + ch).c_str(),
                                    &(pEventReader->setEventDataAt(i,
                                                                   str + ch)));
    }
    for (std::string str : {"ElecFlag"}) {
      sprintf(ch, "%01d", i);
      pEventChain->SetBranchAddress((str + ch).c_str(),
                                    &(pEventReader->setFlagDataAt(i,
                                                                  str + ch)));
    }
  }
  std::cout << "ok" << std::endl;

  // Make analysis tools, ions, and electrons
  pTools = new Analysis::AnalysisTools(kUnit, configReader);
  pIons = new Analysis::Objects(Objects::ions, maxNumOfIonHits, configReader);
  pElectrons = new Analysis::Objects(Objects::elecs, maxNumOfElecHits, configReader);
  pLogWriter->logAnalysisTools(kUnit, *pTools, *pIons, *pElectrons);

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
  createHists();
  std::cout << "ok" << std::endl;

  // Initialization is done
  pLogWriter->write() << "Initialization is done." << std::endl;
  pLogWriter->write() << std::endl;
}

Analysis::AnalysisRun::~AnalysisRun() {
  // counter
  pLogWriter->write() << "Event count: " << pTools->getEventNumber()
                      << std::endl;

  // flush ROOT file
  flushRootFile();

  // finalization is done
  if (pElectrons) {
    delete pElectrons;
    pElectrons = nullptr;
  }
  if (pIons) {
    delete pIons;
    pIons = nullptr;
  }
  if (pTools) {
    delete pTools;
    pTools = nullptr;
  }
  if (pEventReader) {
    delete pEventReader;
    pEventReader = nullptr;
  }
  if (pEventChain) {
    delete pEventChain;
    pEventChain = nullptr;
  }

  pLogWriter->write() << "Finalization is done." << std::endl;
  pLogWriter->write() << std::endl;
  if (pLogWriter) {
    delete pLogWriter;
    pLogWriter = nullptr;
  }
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
  pTools->loadEventDataInputer(*pIons, kUnit, *pEventReader);
  pTools->loadEventDataInputer(*pElectrons, kUnit, *pEventReader);

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
    fillHists();
  }

  // Fill hists for resort resortElecFlags
}

const long Analysis::AnalysisRun::getEntries() const {
  return (long) pEventChain->GetEntries();
}

void Analysis::AnalysisRun::createHists() {
  // IonImage
#define __IONIMAGE1__ "Location X [mm]", "Location Y [mm]", 400, -50, 50, 400, -50, 50, "IonImage"
#define __IONIMAGE2__ "Direction [degree]", "Location R [mm]", 360, -180, 180, 200, 0, 50, "IonImage"
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hImage), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hImage_master), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hImage), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hImage_master), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hImage), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hImage_master), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hImage), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hImage_master), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iCOMImage), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iCOMImage_master), __IONIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hImageDirDist), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hImageDirDist_master), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hImageDirDist), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hImageDirDist_master), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hImageDirDist), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hImageDirDist_master), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hImageDirDist), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hImageDirDist_master), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iCOMImageDirDist), __IONIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iCOMImageDirDist_master), __IONIMAGE2__);

  // IonTOF
#define __IONTOF1__ "TOF [ns]", 2000, 0, 10000, "IonTOF"
#define __IONTOF2__ "TOF [ns]", 4000, 0, 20000, "IonTOF"
#define __IONTOF3__ "TOF [ns]", "TOF [ns]", 500, 0, 10000, 500, 0, 10000, "IonTOF"
#define __IONTOF4__ "Sum of TOFs [ns]", "Diff of TOFs [ns]", 1000, 0, 20000, 500, 0, 10000, "IonTOF"
#define __IONTOF5__ "TOF [ns]", "TOF [ns]", 1000, 0, 20000, 500, 0, 10000, "IonTOF"
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hTOF), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hTOF_master), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hTOF), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hTOF_master), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hTOF), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hTOF_master), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hTOF), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hTOF_master), __IONTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalTOF), __IONTOF2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalTOF_master), __IONTOF2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hPIPICO), __IONTOF3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hPIPICO_master), __IONTOF3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hPIPICO), __IONTOF3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hPIPICO_master), __IONTOF3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hPIPICO), __IONTOF3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hPIPICO_master), __IONTOF3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hRotPIPICO), __IONTOF4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hRotPIPICO_master), __IONTOF4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hRotPIPICO), __IONTOF4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hRotPIPICO_master), __IONTOF4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hRotPIPICO), __IONTOF4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hRotPIPICO_master), __IONTOF4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2h3h3PICO), __IONTOF5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2h3h3PICO_master), __IONTOF5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3h4h3PICO), __IONTOF5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3h4h3PICO_master), __IONTOF5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2h3h4h4PICO), __IONTOF5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2h3h4h4PICO_master), __IONTOF5__);

  // IonFISH
#define __IONFISH1__ "TOF [ns]", "Location [mm]", 500, 0, 10000, 400, -50, 50, "IonFish"
#define __IONFISH2__ "TOF [ns]", "Location [mm]", 500, 0, 10000, 200, 0, 50, "IonFish"
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hXFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hXFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hYFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hYFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hRFish), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hRFish_master), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hXFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hXFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hYFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hYFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hRFish), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hRFish_master), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hXFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hXFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hYFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hYFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hRFish), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hRFish_master), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hXFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hXFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hYFish), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hYFish_master), __IONFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hRFish), __IONFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hRFish_master), __IONFISH2__);

  // IonMomentum
#define __IONMOMENTUM1__ "Momentum [au]", 4000, -500, 500, "IonMomentum"
#define __IONMOMENTUM2__ "Momentum [au]", 2000, 0, 500, "IonMomentum"
#define __IONMOMENTUM3__ "Direction [degree]", "Momentum [au]", 360, -180, 180, 400, 0, 500, "IonMomentum"
#define __IONMOMENTUM4__ "Direction XY [degree]", "Cos Direction Z [1]", 360, -180, 180, 400, -1, 1, "IonMomentum"
#define __IONMOMENTUM5__ "Momentum [au]", "Momentum [au]",  400, 0, 500, 400, 0, 500, "IonMomentum"
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hPX), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hPX_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hPY), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hPY_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hPZ), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hPZ_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hP), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hP_master), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hPX), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hPX_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hPY), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hPY_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hPZ), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hPZ_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hP), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i2hP_master), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hPX), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hPX_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hPY), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hPY_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hPZ), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hPZ_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hP), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i3hP_master), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hPX), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hPX_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hPY), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hPY_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hPZ), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hPZ_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hP), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_i4hP_master), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalPX), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalPX_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalPY), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalPY_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalPZ), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalPZ_master), __IONMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalP), __IONMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalP_master), __IONMOMENTUM2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDistXY), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDistXY_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDistYZ), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDistYZ_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDistZX), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDistZX_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDist), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1hPDirDist_master), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDistXY), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDistXY_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDistYZ), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDistYZ_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDistZX), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDistZX_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDist), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2hPDirDist_master), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDistXY), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDistXY_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDistYZ), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDistYZ_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDistZX), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDistZX_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDist), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3hPDirDist_master), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDistXY), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDistXY_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDistYZ), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDistYZ_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDistZX), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDistZX_master), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDist), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i4hPDirDist_master), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDistXY), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDistXY_master),
           __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDistYZ), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDistYZ_master),
           __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDistZX), __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDistZX_master),
           __IONMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDist), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iTotalPDirDist_master), __IONMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hP), __IONMOMENTUM5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hP_master), __IONMOMENTUM5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hP), __IONMOMENTUM5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hP_master), __IONMOMENTUM5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hP), __IONMOMENTUM5__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hP_master), __IONMOMENTUM5__);

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
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalE), __IONENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_iTotalE_master), __IONENERGY1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hE), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i1h2hE_master), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hE), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hE_master), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hE), __IONENERGY2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i3h4hE_master), __IONENERGY2__);

  // ElecImage
#define __ELECIMAGE1__ "Location X [mm]", "Location Y [mm]", 600, -75, 75, 600, -75, 75, "ElecImage"
#define __ELECIMAGE2__ "Direction [degree]", "Location R [mm]", 360, -180, 180, 300, 0, 75, "ElecImage"
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hImage), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hImage_master), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hImage), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hImage_master), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hImage), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hImage_master), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hImage), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hImage_master), __ELECIMAGE1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hImageDirDist), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hImageDirDist_master), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hImageDirDist), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hImageDirDist_master), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hImageDirDist), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hImageDirDist_master), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hImageDirDist), __ELECIMAGE2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hImageDirDist_master), __ELECIMAGE2__);

  // ElecTOF
#define __ELECTOF1__ "TOF [ns]", 1000, 0, 100, "ElecTOF"
#define __ELECTOF2__ "TOF [ns]", "TOF [ns]", 400, 0, 100, 400, 0, 100, "ElecTOF"
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hTOF), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hTOF_master), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hTOF), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hTOF_master), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hTOF), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hTOF_master), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hTOF), __ELECTOF1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hTOF_master), __ELECTOF1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1h2hPEPECO), __ELECTOF2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1h2hPEPECO_master), __ELECTOF2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2h3hPEPECO), __ELECTOF2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2h3hPEPECO_master), __ELECTOF2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3h4hPEPECO), __ELECTOF2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3h4hPEPECO_master), __ELECTOF2__);

  // ElecFish
#define __ELECFISH1__ "TOF [ns]", "Location [mm]", 400, 0, 100, 600, -75, 75, "ElecFish"
#define __ELECFISH2__ "TOF [ns]", "Location [mm]", 400, 0, 100, 300, 0, 75, "ElecFish"
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hXFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hXFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hYFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hYFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hRFish), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hRFish_master), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hXFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hXFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hYFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hYFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hRFish), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hRFish_master), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hXFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hXFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hYFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hYFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hRFish), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hRFish_master), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hXFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hXFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hYFish), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hYFish_master), __ELECFISH1__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hRFish), __ELECFISH2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hRFish_master), __ELECFISH2__);

  // ElecMomentum
#define __ELECMOMENTUM1__ "Momentum [au]", 4000, -4, 4, "ElecMomentum"
#define __ELECMOMENTUM2__ "Momentum [au]", 2000, 0, 4, "ElecMomentum"
#define __ELECMOMENTUM3__ "Direction [degree]", "Momentum [au]", 360, -180, 180, 500, 0, 4, "ElecMomentum"
#define __ELECMOMENTUM4__ "Direction XY [degree]", "Direction Z [1]", 360, -180, 180, 400, -1, 1, "ElecMomentum"
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hPX), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hPX_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hPY), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hPY_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hPZ), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hPZ_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hP), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hP_master), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hPX), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hPX_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hPY), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hPY_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hPZ), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hPZ_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hP), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hP_master), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hPX), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hPX_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hPY), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hPY_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hPZ), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hPZ_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hP), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hP_master), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hPX), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hPX_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hPY), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hPY_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hPZ), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hPZ_master), __ELECMOMENTUM1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hP), __ELECMOMENTUM2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hP_master), __ELECMOMENTUM2__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDistXY), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDistXY_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDistYZ), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDistYZ_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDistZX), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDistZX_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDist), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hPDirDist_master), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDistXY), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDistXY_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDistYZ), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDistYZ_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDistZX), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDistZX_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDist), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e2hPDirDist_master), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDistXY), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDistXY_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDistYZ), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDistYZ_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDistZX), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDistZX_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDist), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e3hPDirDist_master), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDistXY), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDistXY_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDistYZ), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDistYZ_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDistZX), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDistZX_master), __ELECMOMENTUM3__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDist), __ELECMOMENTUM4__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e4hPDirDist_master), __ELECMOMENTUM4__);

  // ElecEnergy
#define __ELECENERGY1__ "Energy [eV]", 1000, 0, 50, "ElecEnergy"
#define __ELECENERGY2__ "Energy [eV]", 2000, 0, 100, "ElecEnergy"
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hE), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e1hE_master), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hE), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e2hE_master), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hE), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e3hE_master), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hE), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_e4hE_master), __ELECENERGY1__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_eTotalE), __ELECENERGY2__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_eTotalE_master), __ELECENERGY2__);

// IonElecCorr
#define __IONELECCORR__ "KER [eV]", "Electron Kinetic Energy [eV]", 400, 0, 100, 500, 0, 50, "IonElecCorr"
  create2d(SAME_TITLE_WITH_VALNAME(h2_iKER_e1hE), __IONELECCORR__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_iKER_e1hE_master), __IONELECCORR__);

  // Others
#define __OTHERS__ "Others"
  create1d(SAME_TITLE_WITH_VALNAME(h1_i1hTOF_i2h3hNotDead),
           "TOF [ns]", 2000, 0, 10000, __OTHERS__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_i2h3hPIPICO_i1hMaster),
           "TOF 1 [ns]",
           "TOF 2 [ns]",
           500,
           0,
           10000,
           500,
           0,
           10000,
           __OTHERS__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_e1hE_iTotalTOF_master),
           "Energy [eV]",
           "Sum of TOFs [ns]",
           500,
           0,
           50,
           1000,
           0,
           20000,
           __OTHERS__);
}

void Analysis::AnalysisRun::fillHists() {
  const bool iDead1 = pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::Dead);
  const bool iDead2 = pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead);
  const bool iDead3 = pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead);
  const bool iDead4 = pIons->getRealOrDummyObject(3).isFlag(ObjectFlag::Dead);
  const bool
      eDead1 = pElectrons->getRealOrDummyObject(0).isFlag(ObjectFlag::Dead);
  const bool
      eDead2 = pElectrons->getRealOrDummyObject(1).isFlag(ObjectFlag::Dead);
  const bool
      eDead3 = pElectrons->getRealOrDummyObject(2).isFlag(ObjectFlag::Dead);
  const bool
      eDead4 = pElectrons->getRealOrDummyObject(3).isFlag(ObjectFlag::Dead);

  const bool iMaster1 =
      pIons->getRealOrDummyObject(0).isFlag(ObjectFlag::WithinMasterRegion);
  const bool iMaster2 =
      pIons->getRealOrDummyObject(1).isFlag(ObjectFlag::WithinMasterRegion);
  const bool iMaster3 =
      pIons->getRealOrDummyObject(2).isFlag(ObjectFlag::WithinMasterRegion);
  const bool iMaster4 =
      pIons->getRealOrDummyObject(3).isFlag(ObjectFlag::WithinMasterRegion);
  const bool iMaster = pIons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool eMaster = pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool master = iMaster && eMaster;

  const double *const iX1 = pIons->getRealOrDummyObject(0).outputLocX();
  const double *const iY1 = pIons->getRealOrDummyObject(0).outputLocY();
  const double *const iR1 = pIons->getRealOrDummyObject(0).outputLocR();
  const double *const iDir1 = pIons->getRealOrDummyObject(0).outputLocDir();
  const double *const iT1 = pIons->getRealOrDummyObject(0).outputTOF();
  const double *const iX2 = pIons->getRealOrDummyObject(1).outputLocX();
  const double *const iY2 = pIons->getRealOrDummyObject(1).outputLocY();
  const double *const iDir2 = pIons->getRealOrDummyObject(1).outputLocDir();
  const double *const iR2 = pIons->getRealOrDummyObject(1).outputLocR();
  const double *const iT2 = pIons->getRealOrDummyObject(1).outputTOF();
  const double *const iX3 = pIons->getRealOrDummyObject(2).outputLocX();
  const double *const iY3 = pIons->getRealOrDummyObject(2).outputLocY();
  const double *const iDir3 = pIons->getRealOrDummyObject(2).outputLocDir();
  const double *const iR3 = pIons->getRealOrDummyObject(2).outputLocR();
  const double *const iT3 = pIons->getRealOrDummyObject(2).outputTOF();
  const double *const iX4 = pIons->getRealOrDummyObject(3).outputLocX();
  const double *const iY4 = pIons->getRealOrDummyObject(3).outputLocY();
  const double *const iDir4 = pIons->getRealOrDummyObject(3).outputLocDir();
  const double *const iR4 = pIons->getRealOrDummyObject(3).outputLocR();
  const double *const iT4 = pIons->getRealOrDummyObject(3).outputTOF();

  const double *const iCOMX = pIons->outputCOMLocX();
  const double *const iCOMY = pIons->outputCOMLocY();
  const double *const iCOMR = pIons->outputCOMLocR();
  const double *const iCOMDir = pIons->outputCOMLocDir();
  const double *const iTTotal = pIons->outputTotalTOF();
  const double *const iT12 = pIons->outputSumOf2TOFs(0, 1);
  const double *const iT23 = pIons->outputSumOf2TOFs(1, 2);
  const double *const iT34 = pIons->outputSumOf2TOFs(2, 3);
  const double *const iTDiff12 = pIons->outputDiffOfTOFs(0, 1);
  const double *const iTDiff23 = pIons->outputDiffOfTOFs(1, 2);
  const double *const iTDiff34 = pIons->outputDiffOfTOFs(2, 3);

  const double *const eX1 = pElectrons->getRealOrDummyObject(0).outputLocX();
  const double *const eY1 = pElectrons->getRealOrDummyObject(0).outputLocY();
  const double *const eR1 = pElectrons->getRealOrDummyObject(0).outputLocR();
  const double
      *const eDir1 = pElectrons->getRealOrDummyObject(0).outputLocDir();
  const double *const eT1 = pElectrons->getRealOrDummyObject(0).outputTOF();
  const double *const eX2 = pElectrons->getRealOrDummyObject(1).outputLocX();
  const double *const eY2 = pElectrons->getRealOrDummyObject(1).outputLocY();
  const double *const eR2 = pElectrons->getRealOrDummyObject(1).outputLocR();
  const double
      *const eDir2 = pElectrons->getRealOrDummyObject(1).outputLocDir();
  const double *const eT2 = pElectrons->getRealOrDummyObject(1).outputTOF();
  const double *const eX3 = pElectrons->getRealOrDummyObject(2).outputLocX();
  const double *const eY3 = pElectrons->getRealOrDummyObject(2).outputLocY();
  const double *const eR3 = pElectrons->getRealOrDummyObject(2).outputLocR();
  const double
      *const eDir3 = pElectrons->getRealOrDummyObject(2).outputLocDir();
  const double *const eT3 = pElectrons->getRealOrDummyObject(2).outputTOF();
  const double *const eX4 = pElectrons->getRealOrDummyObject(3).outputLocX();
  const double *const eY4 = pElectrons->getRealOrDummyObject(3).outputLocY();
  const double *const eR4 = pElectrons->getRealOrDummyObject(3).outputLocR();
  const double
      *const eDir4 = pElectrons->getRealOrDummyObject(3).outputLocDir();
  const double *const eT4 = pElectrons->getRealOrDummyObject(3).outputTOF();

  const double *const eCOMX = pElectrons->outputCOMLocX();
  const double *const eCOMY = pElectrons->outputCOMLocY();
  const double *const eT12 = pElectrons->outputSumOf2TOFs(0, 1);
  const double *const eT23 = pElectrons->outputSumOf2TOFs(1, 2);
  const double *const eT34 = pElectrons->outputSumOf2TOFs(2, 3);
  const double *const eTDiff12 = pElectrons->outputDiffOfTOFs(0, 1);
  const double *const eTDiff23 = pElectrons->outputDiffOfTOFs(1, 2);
  const double *const eTDiff34 = pElectrons->outputDiffOfTOFs(2, 3);

  const double *const iPX1 = pIons->getRealOrDummyObject(0).outputPX();
  const double *const iPY1 = pIons->getRealOrDummyObject(0).outputPY();
  const double *const iPZ1 = pIons->getRealOrDummyObject(0).outputPZ();
  const double *const iPXY1 = pIons->getRealOrDummyObject(0).outputPXY();
  const double *const iPYZ1 = pIons->getRealOrDummyObject(0).outputPYZ();
  const double *const iPZX1 = pIons->getRealOrDummyObject(0).outputPZX();
  const double *const iP1 = pIons->getRealOrDummyObject(0).outputP();
  const double *const iPDirXY1 = pIons->getRealOrDummyObject(0).outputPDirXY();
  const double *const iPDirYZ1 = pIons->getRealOrDummyObject(0).outputPDirYZ();
  const double *const iPDirZX1 = pIons->getRealOrDummyObject(0).outputPDirZX();
  const double
      *const iCosPDirZ1 = pIons->getRealOrDummyObject(0).outputCosPDirZ();
  const double *const iPX2 = pIons->getRealOrDummyObject(1).outputPX();
  const double *const iPY2 = pIons->getRealOrDummyObject(1).outputPY();
  const double *const iPZ2 = pIons->getRealOrDummyObject(1).outputPZ();
  const double *const iPXY2 = pIons->getRealOrDummyObject(1).outputPXY();
  const double *const iPYZ2 = pIons->getRealOrDummyObject(1).outputPYZ();
  const double *const iPZX2 = pIons->getRealOrDummyObject(1).outputPZX();
  const double *const iP2 = pIons->getRealOrDummyObject(1).outputP();
  const double *const iPDirXY2 = pIons->getRealOrDummyObject(1).outputPDirXY();
  const double *const iPDirYZ2 = pIons->getRealOrDummyObject(1).outputPDirYZ();
  const double *const iPDirZX2 = pIons->getRealOrDummyObject(1).outputPDirZX();
  const double
      *const iCosPDirZ2 = pIons->getRealOrDummyObject(1).outputCosPDirZ();
  const double *const iPX3 = pIons->getRealOrDummyObject(2).outputPX();
  const double *const iPY3 = pIons->getRealOrDummyObject(2).outputPY();
  const double *const iPZ3 = pIons->getRealOrDummyObject(2).outputPZ();
  const double *const iPXY3 = pIons->getRealOrDummyObject(2).outputPXY();
  const double *const iPYZ3 = pIons->getRealOrDummyObject(2).outputPYZ();
  const double *const iPZX3 = pIons->getRealOrDummyObject(2).outputPZX();
  const double *const iP3 = pIons->getRealOrDummyObject(2).outputP();
  const double *const iPDirXY3 = pIons->getRealOrDummyObject(2).outputPDirXY();
  const double *const iPDirYZ3 = pIons->getRealOrDummyObject(2).outputPDirYZ();
  const double *const iPDirZX3 = pIons->getRealOrDummyObject(2).outputPDirZX();
  const double
      *const iCosPDirZ3 = pIons->getRealOrDummyObject(2).outputCosPDirZ();
  const double *const iPX4 = pIons->getRealOrDummyObject(3).outputPX();
  const double *const iPY4 = pIons->getRealOrDummyObject(3).outputPY();
  const double *const iPZ4 = pIons->getRealOrDummyObject(3).outputPZ();
  const double *const iPXY4 = pIons->getRealOrDummyObject(3).outputPXY();
  const double *const iPYZ4 = pIons->getRealOrDummyObject(3).outputPYZ();
  const double *const iPZX4 = pIons->getRealOrDummyObject(3).outputPZX();
  const double *const iP4 = pIons->getRealOrDummyObject(3).outputP();
  const double *const iPDirXY4 = pIons->getRealOrDummyObject(3).outputPDirXY();
  const double *const iPDirYZ4 = pIons->getRealOrDummyObject(3).outputPDirYZ();
  const double *const iPDirZX4 = pIons->getRealOrDummyObject(3).outputPDirZX();
  const double
      *const iCosPDirZ4 = pIons->getRealOrDummyObject(3).outputCosPDirZ();
  const double *const iPXTotal = pIons->outputPX();
  const double *const iPYTotal = pIons->outputPY();
  const double *const iPZTotal = pIons->outputPZ();
  const double *const iPXYTotal = pIons->outputPXY();
  const double *const iPYZTotal = pIons->outputPYZ();
  const double *const iPZXTotal = pIons->outputPZX();
  const double *const iPTotal = pIons->outputP();
  const double *const iPDirXYTotal = pIons->outputPDirXY();
  const double *const iPDirYZTotal = pIons->outputPDirYZ();
  const double *const iPDirZXTotal = pIons->outputPDirZX();
  const double *const iCosPDirZTotal = pIons->outputCosPDirZ();

  const double *const ePX1 = pElectrons->getRealOrDummyObject(0).outputPX();
  const double *const ePY1 = pElectrons->getRealOrDummyObject(0).outputPY();
  const double *const ePZ1 = pElectrons->getRealOrDummyObject(0).outputPZ();
  const double *const ePXY1 = pElectrons->getRealOrDummyObject(0).outputPXY();
  const double *const ePYZ1 = pElectrons->getRealOrDummyObject(0).outputPYZ();
  const double *const ePZX1 = pElectrons->getRealOrDummyObject(0).outputPZX();
  const double *const eP1 = pElectrons->getRealOrDummyObject(0).outputP();
  const double
      *const ePDirXY1 = pElectrons->getRealOrDummyObject(0).outputPDirXY();
  const double
      *const ePDirYZ1 = pElectrons->getRealOrDummyObject(0).outputPDirYZ();
  const double
      *const ePDirZX1 = pElectrons->getRealOrDummyObject(0).outputPDirZX();
  const double
      *const eCosPDirZ1 = pElectrons->getRealOrDummyObject(0).outputCosPDirZ();
  const double *const ePX2 = pElectrons->getRealOrDummyObject(1).outputPX();
  const double *const ePY2 = pElectrons->getRealOrDummyObject(1).outputPY();
  const double *const ePZ2 = pElectrons->getRealOrDummyObject(1).outputPZ();
  const double *const ePXY2 = pElectrons->getRealOrDummyObject(1).outputPXY();
  const double *const ePYZ2 = pElectrons->getRealOrDummyObject(1).outputPYZ();
  const double *const ePZX2 = pElectrons->getRealOrDummyObject(1).outputPZX();
  const double *const eP2 = pElectrons->getRealOrDummyObject(1).outputP();
  const double
      *const ePDirXY2 = pElectrons->getRealOrDummyObject(1).outputPDirXY();
  const double
      *const ePDirYZ2 = pElectrons->getRealOrDummyObject(1).outputPDirYZ();
  const double
      *const ePDirZX2 = pElectrons->getRealOrDummyObject(1).outputPDirZX();
  const double
      *const eCosPDirZ2 = pElectrons->getRealOrDummyObject(1).outputCosPDirZ();
  const double *const ePX3 = pElectrons->getRealOrDummyObject(2).outputPX();
  const double *const ePY3 = pElectrons->getRealOrDummyObject(2).outputPY();
  const double *const ePZ3 = pElectrons->getRealOrDummyObject(2).outputPZ();
  const double *const ePXY3 = pElectrons->getRealOrDummyObject(2).outputPXY();
  const double *const ePYZ3 = pElectrons->getRealOrDummyObject(2).outputPYZ();
  const double *const ePZX3 = pElectrons->getRealOrDummyObject(2).outputPZX();
  const double *const eP3 = pElectrons->getRealOrDummyObject(2).outputP();
  const double
      *const ePDirXY3 = pElectrons->getRealOrDummyObject(2).outputPDirXY();
  const double
      *const ePDirYZ3 = pElectrons->getRealOrDummyObject(2).outputPDirYZ();
  const double
      *const ePDirZX3 = pElectrons->getRealOrDummyObject(2).outputPDirZX();
  const double
      *const eCosPDirZ3 = pElectrons->getRealOrDummyObject(2).outputCosPDirZ();
  const double *const ePX4 = pElectrons->getRealOrDummyObject(3).outputPX();
  const double *const ePY4 = pElectrons->getRealOrDummyObject(3).outputPY();
  const double *const ePZ4 = pElectrons->getRealOrDummyObject(3).outputPZ();
  const double *const ePXY4 = pElectrons->getRealOrDummyObject(3).outputPXY();
  const double *const ePYZ4 = pElectrons->getRealOrDummyObject(3).outputPYZ();
  const double *const ePZX4 = pElectrons->getRealOrDummyObject(3).outputPZX();
  const double *const eP4 = pElectrons->getRealOrDummyObject(3).outputP();
  const double
      *const ePDirXY4 = pElectrons->getRealOrDummyObject(3).outputPDirXY();
  const double
      *const ePDirYZ4 = pElectrons->getRealOrDummyObject(3).outputPDirYZ();
  const double
      *const ePDirZX4 = pElectrons->getRealOrDummyObject(3).outputPDirZX();
  const double
      *const eCosPDirZ4 = pElectrons->getRealOrDummyObject(3).outputCosPDirZ();

  const double *const iE1 = pIons->getRealOrDummyObject(0).outputE();
  const double *const iE2 = pIons->getRealOrDummyObject(1).outputE();
  const double *const iE3 = pIons->getRealOrDummyObject(2).outputE();
  const double *const iE4 = pIons->getRealOrDummyObject(3).outputE();
  const double *const iETotal = pIons->outputE();

  const double *const eE1 = pElectrons->getRealOrDummyObject(0).outputE();
  const double *const eE2 = pElectrons->getRealOrDummyObject(1).outputE();
  const double *const eE3 = pElectrons->getRealOrDummyObject(2).outputE();
  const double *const eE4 = pElectrons->getRealOrDummyObject(3).outputE();
  const double *const eETotal = pElectrons->outputE();

  // IonImage
  fill2d(h2_i1hImage, iX1, iY1);
  fill2d(h2_i2hImage, iX2, iY2);
  fill2d(h2_i3hImage, iX3, iY3);
  fill2d(h2_i4hImage, iX4, iY4);
  fill2d(h2_iCOMImage, iCOMX, iCOMY);
  fill2d(h2_i1hImageDirDist, iDir1, iR1);
  fill2d(h2_i2hImageDirDist, iDir2, iR2);
  fill2d(h2_i3hImageDirDist, iDir3, iR3);
  fill2d(h2_i4hImageDirDist, iDir4, iR4);
  fill2d(h2_iCOMImageDirDist, iCOMDir, iCOMDir);
  if (master) {
    fill2d(h2_i1hImage_master, iX1, iY1);
    fill2d(h2_i2hImage_master, iX2, iY2);
    fill2d(h2_i3hImage_master, iX3, iY3);
    fill2d(h2_i4hImage_master, iX4, iY4);
    fill2d(h2_iCOMImage_master, iCOMX, iCOMY);
    fill2d(h2_i1hImageDirDist_master, iDir1, iR1);
    fill2d(h2_i2hImageDirDist_master, iDir2, iR2);
    fill2d(h2_i3hImageDirDist_master, iDir3, iR3);
    fill2d(h2_i4hImageDirDist_master, iDir4, iR4);
    fill2d(h2_iCOMImageDirDist_master, iCOMDir, iCOMDir);
  }

  // IonTOF
  fill1d(h1_i1hTOF, iT1);
  fill1d(h1_i2hTOF, iT2);
  fill1d(h1_i3hTOF, iT3);
  fill1d(h1_i4hTOF, iT4);
  fill1d(h1_iTotalTOF, iTTotal);
  fill2d(h2_i1h2hPIPICO, iT1, iT2);
  fill2d(h2_i2h3hPIPICO, iT2, iT3);
  fill2d(h2_i3h4hPIPICO, iT3, iT4);
  fill2d(h2_i1h2hRotPIPICO, iT12, iTDiff12);
  fill2d(h2_i1h2hRotPIPICO, iT23, iTDiff23);
  fill2d(h2_i1h2hRotPIPICO, iT34, iTDiff34);
  if (master) {
    fill1d(h1_i1hTOF_master, iT1);
    fill1d(h1_i2hTOF_master, iT2);
    fill1d(h1_i3hTOF_master, iT3);
    fill1d(h1_i4hTOF_master, iT4);
    fill1d(h1_iTotalTOF_master, iTTotal);
    fill2d(h2_i1h2hPIPICO_master, iT1, iT2);
    fill2d(h2_i2h3hPIPICO_master, iT2, iT3);
    fill2d(h2_i3h4hPIPICO_master, iT3, iT4);
    fill2d(h2_i1h2hRotPIPICO_master, iT12, iTDiff12);
    fill2d(h2_i1h2hRotPIPICO_master, iT23, iTDiff23);
    fill2d(h2_i1h2hRotPIPICO_master, iT34, iTDiff34);
  }

// IonFish
  fill2d(h2_i1hXFish, iT1, iX1);
  fill2d(h2_i1hYFish, iT1, iY1);
  fill2d(h2_i1hRFish, iT1, iR1);
  fill2d(h2_i2hXFish, iT2, iX2);
  fill2d(h2_i2hYFish, iT2, iY2);
  fill2d(h2_i2hRFish, iT2, iR2);
  fill2d(h2_i3hXFish, iT3, iX3);
  fill2d(h2_i3hYFish, iT3, iY3);
  fill2d(h2_i3hRFish, iT3, iR3);
  fill2d(h2_i4hXFish, iT4, iX4);
  fill2d(h2_i4hYFish, iT4, iY4);
  fill2d(h2_i4hRFish, iT4, iR4);
  if (master) {
    fill2d(h2_i1hXFish_master, iT1, iX1);
    fill2d(h2_i1hYFish_master, iT1, iY1);
    fill2d(h2_i1hRFish_master, iT1, iR1);
    fill2d(h2_i2hXFish_master, iT2, iX2);
    fill2d(h2_i2hYFish_master, iT2, iY2);
    fill2d(h2_i2hRFish_master, iT2, iR2);
    fill2d(h2_i3hXFish_master, iT3, iX3);
    fill2d(h2_i3hYFish_master, iT3, iY3);
    fill2d(h2_i3hRFish_master, iT3, iR3);
    fill2d(h2_i4hXFish_master, iT4, iX4);
    fill2d(h2_i4hYFish_master, iT4, iY4);
    fill2d(h2_i4hRFish_master, iT4, iR4);
  }

// IonMomentum
  fill1d(h1_i1hPX, iPX1);
  fill1d(h1_i1hPY, iPY1);
  fill1d(h1_i1hPZ, iPZ1);
  fill1d(h1_i1hP, iP1);
  fill1d(h1_i2hPX, iPX2);
  fill1d(h1_i2hPY, iPY2);
  fill1d(h1_i2hPZ, iPZ2);
  fill1d(h1_i2hP, iP2);
  fill1d(h1_i3hPX, iPX3);
  fill1d(h1_i3hPY, iPY3);
  fill1d(h1_i3hPZ, iPZ3);
  fill1d(h1_i3hP, iP3);
  fill1d(h1_i4hPX, iPX4);
  fill1d(h1_i4hPY, iPY4);
  fill1d(h1_i4hPZ, iPZ4);
  fill1d(h1_i4hP, iP4);
  fill1d(h1_iTotalPX, iPXTotal);
  fill1d(h1_iTotalPY, iPYTotal);
  fill1d(h1_iTotalPZ, iPZTotal);
  fill1d(h1_iTotalP, iPTotal);
  fill2d(h2_i1hPDirDistXY, iPDirXY1, iPXY1);
  fill2d(h2_i1hPDirDistYZ, iPDirYZ1, iPYZ1);
  fill2d(h2_i1hPDirDistZX, iPDirZX1, iPZX1);
  fill2d(h2_i1hPDirDist, iPDirXY1, iCosPDirZ1);
  fill2d(h2_i2hPDirDistXY, iPDirXY2, iPXY2);
  fill2d(h2_i2hPDirDistYZ, iPDirYZ2, iPYZ2);
  fill2d(h2_i2hPDirDistZX, iPDirZX2, iPZX2);
  fill2d(h2_i2hPDirDist, iPDirXY2, iCosPDirZ2);
  fill2d(h2_i3hPDirDistXY, iPDirXY3, iPXY3);
  fill2d(h2_i3hPDirDistYZ, iPDirYZ3, iPYZ3);
  fill2d(h2_i3hPDirDistZX, iPDirZX3, iPZX3);
  fill2d(h2_i3hPDirDist, iPDirXY3, iCosPDirZ3);
  fill2d(h2_i4hPDirDistXY, iPDirXY4, iPXY4);
  fill2d(h2_i4hPDirDistYZ, iPDirYZ4, iPYZ4);
  fill2d(h2_i4hPDirDistZX, iPDirZX4, iPZX4);
  fill2d(h2_i4hPDirDist, iPDirXY4, iCosPDirZ4);
  fill2d(h2_iTotalPDirDistXY, iPDirXYTotal, iPXYTotal);
  fill2d(h2_iTotalPDirDistYZ, iPDirYZTotal, iPYZTotal);
  fill2d(h2_iTotalPDirDistZX, iPDirZXTotal, iPZXTotal);
  fill2d(h2_iTotalPDirDist, iPDirXYTotal, iCosPDirZTotal);
  fill2d(h2_i1h2hP, iP1, iP2);
  fill2d(h2_i2h3hP, iP2, iP3);
  fill2d(h2_i3h4hP, iP3, iP4);
  if (master) {
    fill1d(h1_i1hPX_master, iPX1);
    fill1d(h1_i1hPY_master, iPY1);
    fill1d(h1_i1hPZ_master, iPZ1);
    fill1d(h1_i1hP_master, iP1);
    fill1d(h1_i2hPX_master, iPX2);
    fill1d(h1_i2hPY_master, iPY2);
    fill1d(h1_i2hPZ_master, iPZ2);
    fill1d(h1_i2hP_master, iP2);
    fill1d(h1_i3hPX_master, iPX3);
    fill1d(h1_i3hPY_master, iPY3);
    fill1d(h1_i3hPZ_master, iPZ3);
    fill1d(h1_i3hP_master, iP3);
    fill1d(h1_i4hPX_master, iPX4);
    fill1d(h1_i4hPY_master, iPY4);
    fill1d(h1_i4hPZ_master, iPZ4);
    fill1d(h1_i4hP_master, iP4);
    fill1d(h1_iTotalPX_master, iPXTotal);
    fill1d(h1_iTotalPY_master, iPYTotal);
    fill1d(h1_iTotalPZ_master, iPZTotal);
    fill1d(h1_iTotalP_master, iPTotal);
    fill2d(h2_i1hPDirDistXY_master, iPDirXY1, iPXY1);
    fill2d(h2_i1hPDirDistYZ_master, iPDirYZ1, iPYZ1);
    fill2d(h2_i1hPDirDistZX_master, iPDirZX1, iPZX1);
    fill2d(h2_i1hPDirDist_master, iPDirXY1, iCosPDirZ1);
    fill2d(h2_i2hPDirDistXY_master, iPDirXY2, iPXY2);
    fill2d(h2_i2hPDirDistYZ_master, iPDirYZ2, iPYZ2);
    fill2d(h2_i2hPDirDistZX_master, iPDirZX2, iPZX2);
    fill2d(h2_i2hPDirDist_master, iPDirXY2, iCosPDirZ2);
    fill2d(h2_i3hPDirDistXY_master, iPDirXY3, iPXY3);
    fill2d(h2_i3hPDirDistYZ_master, iPDirYZ3, iPYZ3);
    fill2d(h2_i3hPDirDistZX_master, iPDirZX3, iPZX3);
    fill2d(h2_i3hPDirDist_master, iPDirXY3, iCosPDirZ3);
    fill2d(h2_i4hPDirDistXY_master, iPDirXY4, iPXY4);
    fill2d(h2_i4hPDirDistYZ_master, iPDirYZ4, iPYZ4);
    fill2d(h2_i4hPDirDistZX_master, iPDirZX4, iPZX4);
    fill2d(h2_i4hPDirDist_master, iPDirXY4, iCosPDirZ4);
    fill2d(h2_iTotalPDirDistXY_master, iPDirXYTotal, iPXYTotal);
    fill2d(h2_iTotalPDirDistYZ_master, iPDirYZTotal, iPYZTotal);
    fill2d(h2_iTotalPDirDistZX_master, iPDirZXTotal, iPZXTotal);
    fill2d(h2_iTotalPDirDist_master, iPDirXYTotal, iCosPDirZTotal);
    fill2d(h2_i1h2hP_master, iP1, iP2);
    fill2d(h2_i2h3hP_master, iP2, iP3);
    fill2d(h2_i3h4hP_master, iP3, iP4);
  }

// IonEnergy
  fill1d(h1_i1hE, iE1);
  fill1d(h1_i2hE, iE2);
  fill1d(h1_i3hE, iE3);
  fill1d(h1_i4hE, iE4);
  fill1d(h1_iTotalE, iETotal);
  fill2d(h2_i1h2hE, iE1, iE2);
  fill2d(h2_i2h3hE, iE2, iE3);
  fill2d(h2_i3h4hE, iE3, iE4);
  if (master) {
    fill1d(h1_i1hE_master, iE1);
    fill1d(h1_i2hE_master, iE2);
    fill1d(h1_i3hE_master, iE3);
    fill1d(h1_i4hE_master, iE4);
    fill1d(h1_iTotalE_master, iETotal);
    fill2d(h2_i1h2hE_master, iE1, iE2);
    fill2d(h2_i2h3hE_master, iE2, iE3);
    fill2d(h2_i3h4hE_master, iE3, iE4);
  }

  // ElecImage
  fill2d(h2_e1hImage, eX1, eY1);
  fill2d(h2_e2hImage, eX2, eY2);
  fill2d(h2_e3hImage, eX3, eY3);
  fill2d(h2_e4hImage, eX4, eY4);
  fill2d(h2_e1hImageDirDist, eDir1, eR1);
  fill2d(h2_e2hImageDirDist, eDir2, eR2);
  fill2d(h2_e3hImageDirDist, eDir3, eR3);
  fill2d(h2_e4hImageDirDist, eDir4, eR4);
  if (master) {
    fill2d(h2_e1hImage_master, eX1, eY1);
    fill2d(h2_e2hImage_master, eX2, eY2);
    fill2d(h2_e3hImage_master, eX3, eY3);
    fill2d(h2_e4hImage_master, eX4, eY4);
    fill2d(h2_e1hImageDirDist_master, eDir1, eR1);
    fill2d(h2_e2hImageDirDist_master, eDir2, eR2);
    fill2d(h2_e3hImageDirDist_master, eDir3, eR3);
    fill2d(h2_e4hImageDirDist_master, eDir4, eR4);
  }

  // ElecTOF
  fill1d(h1_e1hTOF, eT1);
  fill1d(h1_e2hTOF, eT2);
  fill1d(h1_e3hTOF, eT3);
  fill1d(h1_e4hTOF, eT4);
  fill2d(h2_e1h2hPEPECO, eT1, eT2);
  fill2d(h2_e2h3hPEPECO, eT2, eT3);
  fill2d(h2_e3h4hPEPECO, eT3, eT4);
  if (master) {
    fill1d(h1_e1hTOF_master, eT1);
    fill1d(h1_e2hTOF_master, eT2);
    fill1d(h1_e3hTOF_master, eT3);
    fill1d(h1_e4hTOF_master, eT4);
    fill2d(h2_e1h2hPEPECO_master, eT1, eT2);
    fill2d(h2_e2h3hPEPECO_master, eT2, eT3);
    fill2d(h2_e3h4hPEPECO_master, eT3, eT4);
  }

  // ElecFish
  fill2d(h2_e1hXFish, eT1, eX1);
  fill2d(h2_e1hYFish, eT1, eY1);
  fill2d(h2_e1hRFish, eT1, eR1);
  fill2d(h2_e2hXFish, eT2, eX2);
  fill2d(h2_e2hYFish, eT2, eY2);
  fill2d(h2_e2hRFish, eT2, eR2);
  fill2d(h2_e3hXFish, eT3, eX3);
  fill2d(h2_e3hYFish, eT3, eY3);
  fill2d(h2_e3hRFish, eT3, eR3);
  fill2d(h2_e4hXFish, eT4, eX4);
  fill2d(h2_e4hYFish, eT4, eY4);
  fill2d(h2_e4hRFish, eT4, eR4);
  if (master) {
    fill2d(h2_e1hXFish_master, eT1, eX1);
    fill2d(h2_e1hYFish_master, eT1, eY1);
    fill2d(h2_e1hRFish_master, eT1, eR1);
    fill2d(h2_e2hXFish_master, eT2, eX2);
    fill2d(h2_e2hYFish_master, eT2, eY2);
    fill2d(h2_e2hRFish_master, eT2, eR2);
    fill2d(h2_e3hXFish_master, eT3, eX3);
    fill2d(h2_e3hYFish_master, eT3, eY3);
    fill2d(h2_e3hRFish_master, eT3, eR3);
    fill2d(h2_e4hXFish_master, eT4, eX4);
    fill2d(h2_e4hYFish_master, eT4, eY4);
    fill2d(h2_e4hRFish_master, eT4, eR4);
  }

  // ElecMomentum
  fill1d(h1_e1hPX, ePX1);
  fill1d(h1_e1hPY, ePY1);
  fill1d(h1_e1hPZ, ePZ1);
  fill1d(h1_e1hP, eP1);
  fill1d(h1_e2hPX, ePX2);
  fill1d(h1_e2hPY, ePY2);
  fill1d(h1_e2hPZ, ePZ2);
  fill1d(h1_e2hP, eP2);
  fill1d(h1_e3hPX, ePX3);
  fill1d(h1_e3hPY, ePY3);
  fill1d(h1_e3hPZ, ePZ3);
  fill1d(h1_e3hP, eP3);
  fill1d(h1_e4hPX, ePX4);
  fill1d(h1_e4hPY, ePY4);
  fill1d(h1_e4hPZ, ePZ4);
  fill1d(h1_e4hP, eP4);
  fill2d(h2_e1hPDirDistXY, ePDirXY1, ePXY1);
  fill2d(h2_e1hPDirDistYZ, ePDirYZ1, ePYZ1);
  fill2d(h2_e1hPDirDistZX, ePDirZX1, ePZX1);
  fill2d(h2_e1hPDirDist, ePDirXY1, eCosPDirZ1);
  fill2d(h2_e2hPDirDistXY, ePDirXY2, ePXY2);
  fill2d(h2_e2hPDirDistYZ, ePDirYZ2, ePYZ2);
  fill2d(h2_e2hPDirDistZX, ePDirZX2, ePZX2);
  fill2d(h2_e2hPDirDist, ePDirXY2, eCosPDirZ2);
  fill2d(h2_e3hPDirDistXY, ePDirXY3, ePXY3);
  fill2d(h2_e3hPDirDistYZ, ePDirYZ3, ePYZ3);
  fill2d(h2_e3hPDirDistZX, ePDirZX3, ePZX3);
  fill2d(h2_e3hPDirDist, ePDirXY3, eCosPDirZ3);
  fill2d(h2_e4hPDirDistXY, ePDirXY4, ePXY4);
  fill2d(h2_e4hPDirDistYZ, ePDirYZ4, ePYZ4);
  fill2d(h2_e4hPDirDistZX, ePDirZX4, ePZX4);
  fill2d(h2_e4hPDirDist, ePDirXY4, eCosPDirZ4);
  if (master) {
    fill1d(h1_e1hPX_master, ePX1);
    fill1d(h1_e1hPY_master, ePY1);
    fill1d(h1_e1hPZ_master, ePZ1);
    fill1d(h1_e1hP_master, eP1);
    fill1d(h1_e2hPX_master, ePX2);
    fill1d(h1_e2hPY_master, ePY2);
    fill1d(h1_e2hPZ_master, ePZ2);
    fill1d(h1_e2hP_master, eP2);
    fill1d(h1_e3hPX_master, ePX3);
    fill1d(h1_e3hPY_master, ePY3);
    fill1d(h1_e3hPZ_master, ePZ3);
    fill1d(h1_e3hP_master, eP3);
    fill1d(h1_e4hPX_master, ePX4);
    fill1d(h1_e4hPY_master, ePY4);
    fill1d(h1_e4hPZ_master, ePZ4);
    fill1d(h1_e4hP_master, eP4);
    fill2d(h2_e1hPDirDistXY_master, ePDirXY1, ePXY1);
    fill2d(h2_e1hPDirDistYZ_master, ePDirYZ1, ePYZ1);
    fill2d(h2_e1hPDirDistZX_master, ePDirZX1, ePZX1);
    fill2d(h2_e1hPDirDist_master, ePDirXY1, eCosPDirZ1);
    fill2d(h2_e2hPDirDistXY_master, ePDirXY2, ePXY2);
    fill2d(h2_e2hPDirDistYZ_master, ePDirYZ2, ePYZ2);
    fill2d(h2_e2hPDirDistZX_master, ePDirZX2, ePZX2);
    fill2d(h2_e2hPDirDist_master, ePDirXY2, eCosPDirZ2);
    fill2d(h2_e3hPDirDistXY_master, ePDirXY3, ePXY3);
    fill2d(h2_e3hPDirDistYZ_master, ePDirYZ3, ePYZ3);
    fill2d(h2_e3hPDirDistZX_master, ePDirZX3, ePZX3);
    fill2d(h2_e3hPDirDist_master, ePDirXY3, eCosPDirZ3);
    fill2d(h2_e4hPDirDistXY_master, ePDirXY4, ePXY4);
    fill2d(h2_e4hPDirDistYZ_master, ePDirYZ4, ePYZ4);
    fill2d(h2_e4hPDirDistZX_master, ePDirZX4, ePZX4);
    fill2d(h2_e4hPDirDist_master, ePDirXY4, eCosPDirZ4);
  }

  // ElecEnergy
  fill1d(h1_e1hE, eE1);
  fill1d(h1_e2hE, eE2);
  fill1d(h1_e3hE, eE3);
  fill1d(h1_e4hE, eE4);
  fill1d(h1_eTotalE, eETotal);
  if (master) {
    fill1d(h1_e1hE_master, eE1);
    fill1d(h1_e2hE_master, eE2);
    fill1d(h1_e3hE_master, eE3);
    fill1d(h1_e4hE_master, eE4);
    fill1d(h1_eTotalE_master, eETotal);
  }

  // IonElecCorr
  fill2d(h2_iKER_e1hE, iETotal, eE1);
  if (master) {
    fill2d(h2_iKER_e1hE_master, iETotal, eE1);
  }

  // Others
  if ((!iDead2) && (!iDead3)) {
    fill1d(h1_i1hTOF_i2h3hNotDead, iT1);
  }
  if (iMaster1) {
    fill2d(h2_i2h3hPIPICO_i1hMaster, iT2, iT3);
  }
  if (master) {
    fill2d(h2_e1hE_iTotalTOF_master, eE1, iTTotal);
  }
}