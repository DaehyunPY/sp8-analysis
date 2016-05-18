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
    fillNatureHists();
  }

  // Fill hists for resort flags
}
//void Analysis::Run::fillIonBasicData() {
//  const double &i1HitX = pIons->getRealOrDummyObject(0).getLocationX(*pUnit);
//  const double &i1HitY = pIons->getRealOrDummyObject(0).getLocationY(*pUnit);
//  const double &i2HitX = pIons->getRealOrDummyObject(1).getLocationX(*pUnit);
//  const double &i2HitY = pIons->getRealOrDummyObject(1).getLocationY(*pUnit);
//  const double &i3HitX = pIons->getRealOrDummyObject(2).getLocationX(*pUnit);
//  const double &i3HitY = pIons->getRealOrDummyObject(2).getLocationY(*pUnit);
//  const double &i4HitX = pIons->getRealOrDummyObject(3).getLocationX(*pUnit);
//  const double &i4HitY = pIons->getRealOrDummyObject(3).getLocationY(*pUnit);
//  const double &iCOMX = pIons->getLocationX(*pUnit);
//  const double &iCOMY = pIons->getLocationY(*pUnit);
//  const double &i1HitTOF = pIons->getRealOrDummyObject(0).getTOF(*pUnit);
//  const double &i2HitTOF = pIons->getRealOrDummyObject(1).getTOF(*pUnit);
//  const double &i3HitTOF = pIons->getRealOrDummyObject(2).getTOF(*pUnit);
//  const double &i4HitTOF = pIons->getRealOrDummyObject(3).getTOF(*pUnit);
//  const double &i1HitPlus2HitTOF = pIons->getSumOfTOF(*pUnit, 0, 1);
//  const double &i2HitPlus3HitTOF = pIons->getSumOfTOF(*pUnit, 1, 2);
//  root2DHistogramOf1stHitIonLocationX_LocationY.Fill(i1HitX, i1HitY);
//  root2DHistogramOf2ndHitIonLocationX_LocationY.Fill(i2HitX, i2HitY);
//  root2DHistogramOf3rdHitIonLocationX_LocationY.Fill(i3HitX, i3HitY);
//  root2DHistogramOf4thHitIonLocationX_LocationY.Fill(i4HitX, i4HitY);
//  root2DHistogramOfIonsCOMLocationX_LocationY.Fill(iCOMX, iCOMY);
//  root1DHistogramOf1stHitIonTOF.Fill(i1HitTOF);
//  root1DHistogramOf2ndHitIonTOF.Fill(i2HitTOF);
//  root1DHistogramOf3rdHitIonTOF.Fill(i3HitTOF);
//  root1DHistogramOf4thHitIonTOF.Fill(i4HitTOF);
//  root2DHistogramOf1stHitIonTOF_2ndHitIonTOF.Fill(i1HitTOF, i2HitTOF);
//  root2DHistogramOf2ndHitIonTOF_3rdHitIonTOF.Fill(i2HitTOF, i3HitTOF);
//  if (pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::WithinMasterRegion)) {
//    root2DHistogramOf2ndHitIonTOF_3rdHitIonTOF_under1stHitIonMaster.Fill(
//        i2HitTOF,
//        i3HitTOF);
//  }
//  root2DHistogramOf3rdHitIonTOF_4thHitIonTOF.Fill(i3HitTOF, i4HitTOF);
//  root2DHistogramOf1stHitIonTOFPlus2ndHitIonTOF_3rdHitIonTOF.Fill(
//      i1HitPlus2HitTOF,
//      i3HitTOF);
//  root2DHistogramOf2ndHitIonTOFPlus3rdHitIonTOF_4thHitIonTOF.Fill(
//      i2HitPlus3HitTOF,
//      i4HitTOF);
//}
//void Analysis::Run::fillIonMomentumData() {
//  const double &i1HitPx = pIons->getRealOrDummyObject(0).getMomentumX(*pUnit);
//  const double &i1HitPy = pIons->getRealOrDummyObject(0).getMomentumY(*pUnit);
//  const double &i1HitPz = pIons->getRealOrDummyObject(0).getMomentumZ(*pUnit);
//  const double &i1HitE = pIons->getRealOrDummyObject(0).getEnergy(*pUnit);
//  const double &i2HitPx = pIons->getRealOrDummyObject(1).getMomentumX(*pUnit);
//  const double &i2HitPy = pIons->getRealOrDummyObject(1).getMomentumY(*pUnit);
//  const double &i2HitPz = pIons->getRealOrDummyObject(1).getMomentumZ(*pUnit);
//  const double &i2HitE = pIons->getRealOrDummyObject(1).getEnergy(*pUnit);
//  const double &i3HitPx = pIons->getRealOrDummyObject(2).getMomentumX(*pUnit);
//  const double &i3HitPy = pIons->getRealOrDummyObject(2).getMomentumY(*pUnit);
//  const double &i3HitPz = pIons->getRealOrDummyObject(2).getMomentumZ(*pUnit);
//  const double &i3HitE = pIons->getRealOrDummyObject(2).getEnergy(*pUnit);
//  const double &i4HitPx = pIons->getRealOrDummyObject(3).getMomentumX(*pUnit);
//  const double &i4HitPy = pIons->getRealOrDummyObject(3).getMomentumY(*pUnit);
//  const double &i4HitPz = pIons->getRealOrDummyObject(3).getMomentumZ(*pUnit);
//  const double &i4HitE = pIons->getRealOrDummyObject(3).getEnergy(*pUnit);
//  const double &iTotalE = pIons->getEnergy(*pUnit);
//  root2DHistogramOf1stHitIonMomentumXY.Fill(i1HitPx, i1HitPy);
//  root2DHistogramOf2ndHitIonMomentumXY.Fill(i2HitPx, i2HitPy);
//  root2DHistogramOf3rdHitIonMomentumXY.Fill(i3HitPx, i3HitPy);
//  root2DHistogramOf4thHitIonMomentumXY.Fill(i4HitPx, i4HitPy);
//  root1DHistogramOf1stHitIonMomentumZ.Fill(i1HitPz);
//  root1DHistogramOf2ndHitIonMomentumZ.Fill(i2HitPz);
//  root1DHistogramOf3rdHitIonMomentumZ.Fill(i3HitPz);
//  root1DHistogramOf4thHitIonMomentumZ.Fill(i4HitPz);
//  root1DHistogramOf1stHitIonEnergy.Fill(i1HitE);
//  root1DHistogramOf2ndHitIonEnergy.Fill(i2HitE);
//  root1DHistogramOf3rdHitIonEnergy.Fill(i3HitE);
//  root1DHistogramOf4thHitIonEnergy.Fill(i4HitE);
//  root1DHistogramOfIonsTotalEnergy.Fill(iTotalE);
//}
//void Analysis::Run::fillElectronBasicData() {
//  const double
//      &e1HitX = pElectrons->getRealOrDummyObject(0).getLocationX(*pUnit);
//  const double
//      &e1HitY = pElectrons->getRealOrDummyObject(0).getLocationY(*pUnit);
//  const double
//      &e2HitX = pElectrons->getRealOrDummyObject(1).getLocationX(*pUnit);
//  const double
//      &e2HitY = pElectrons->getRealOrDummyObject(1).getLocationY(*pUnit);
//  const double
//      &e3HitX = pElectrons->getRealOrDummyObject(2).getLocationX(*pUnit);
//  const double
//      &e3HitY = pElectrons->getRealOrDummyObject(2).getLocationY(*pUnit);
//  const double
//      &e4HitX = pElectrons->getRealOrDummyObject(3).getLocationX(*pUnit);
//  const double
//      &e4HitY = pElectrons->getRealOrDummyObject(3).getLocationY(*pUnit);
//  const double &eCOMX = pElectrons->getLocationX(*pUnit);
//  const double &eCOMY = pElectrons->getLocationY(*pUnit);
//  const double &eCOMThetaXY = pElectrons->getLocationalDirectionXY(*pUnit);
//  const double &eCOMR = pElectrons->getLocation(*pUnit);
//  const double &e1HitTOF = pElectrons->getRealOrDummyObject(0).getTOF(*pUnit);
//  const double &e2HitTOF = pElectrons->getRealOrDummyObject(1).getTOF(*pUnit);
//  const double &e3HitTOF = pElectrons->getRealOrDummyObject(2).getTOF(*pUnit);
//  const double &e4HitTOF = pElectrons->getRealOrDummyObject(3).getTOF(*pUnit);
//  root2DHistogramOf1stHitElectronLocationX_LocationY.Fill(e1HitX, e1HitY);
//  root2DHistogramOf2ndHitElectronLocationX_LocationY.Fill(e2HitX, e2HitY);
//  root2DHistogramOf3rdHitElectronLocationX_LocationY.Fill(e3HitX, e3HitY);
//  root2DHistogramOf4thHitElectronLocationX_LocationY.Fill(e4HitX, e4HitY);
//  root2DHistogramOfElectronsCOMLocationX_LocationY.Fill(eCOMX, eCOMY);
//  root2DHistogramOfElectronsCOMLocationalDirectionXY_Radius.Fill(eCOMThetaXY,
//                                                                 eCOMR);
//  root1DHistogramOf1stHitElectronTOF.Fill(e1HitTOF);
//  root1DHistogramOf2ndHitElectronTOF.Fill(e2HitTOF);
//  root1DHistogramOf3rdHitElectronTOF.Fill(e3HitTOF);
//  root1DHistogramOf4thHitElectronTOF.Fill(e4HitTOF);
//}
//void Analysis::Run::fillElectronMomentumData() {
//  const double
//      &e1HitPx = pElectrons->getRealOrDummyObject(0).getMomentumX(*pUnit);
//  const double
//      &e1HitPy = pElectrons->getRealOrDummyObject(0).getMomentumY(*pUnit);
//  const double
//      &e1HitPz = pElectrons->getRealOrDummyObject(0).getMomentumZ(*pUnit);
//  const double &e1HitE = pElectrons->getRealOrDummyObject(0).getEnergy(*pUnit);
//  const double
//      &e2HitPx = pElectrons->getRealOrDummyObject(1).getMomentumX(*pUnit);
//  const double
//      &e2HitPy = pElectrons->getRealOrDummyObject(1).getMomentumY(*pUnit);
//  const double
//      &e2HitPz = pElectrons->getRealOrDummyObject(1).getMomentumZ(*pUnit);
//  const double &e2HitE = pElectrons->getRealOrDummyObject(1).getEnergy(*pUnit);
//  const double
//      &e3HitPx = pElectrons->getRealOrDummyObject(2).getMomentumX(*pUnit);
//  const double
//      &e3HitPy = pElectrons->getRealOrDummyObject(2).getMomentumY(*pUnit);
//  const double
//      &e3HitPz = pElectrons->getRealOrDummyObject(2).getMomentumZ(*pUnit);
//  const double &e3HitE = pElectrons->getRealOrDummyObject(2).getEnergy(*pUnit);
//  const double
//      &e4HitPx = pElectrons->getRealOrDummyObject(3).getMomentumX(*pUnit);
//  const double
//      &e4HitPy = pElectrons->getRealOrDummyObject(3).getMomentumY(*pUnit);
//  const double
//      &e4HitPz = pElectrons->getRealOrDummyObject(3).getMomentumZ(*pUnit);
//  const double &e4HitE = pElectrons->getRealOrDummyObject(3).getEnergy(*pUnit);
//  const double &eTotalP = pElectrons->getMomentum(*pUnit);
//  const double &eTotalThetaXY = pElectrons->getMotionalDirectionXY(*pUnit);
//  const double &eTotalThetaZX = pElectrons->getMotionalDirectionZX(*pUnit);
//  const double &eTotalThetaZY = pElectrons->getMotionalDirectionZY(*pUnit);
//  const double &eTotalE = pElectrons->getEnergy(*pUnit);
//  root2DHistogramOf1stHitElectronMomentumXY.Fill(e1HitPx, e1HitPy);
//  root2DHistogramOf2ndHitElectronMomentumXY.Fill(e2HitPx, e2HitPy);
//  root2DHistogramOf3rdHitElectronMomentumXY.Fill(e3HitPx, e3HitPy);
//  root2DHistogramOf4thHitElectronMomentumXY.Fill(e4HitPx, e4HitPy);
//  root1DHistogramOf1stHitElectronMomentumZ.Fill(e1HitPz);
//  root1DHistogramOf2ndHitElectronMomentumZ.Fill(e2HitPz);
//  root1DHistogramOf3rdHitElectronMomentumZ.Fill(e3HitPz);
//  root1DHistogramOf4thHitElectronMomentumZ.Fill(e4HitPz);
//  root2DHistogramOfElectronsTotalMotionalDirectionXY_Momentum.Fill(eTotalThetaXY,
//                                                                   eTotalP);
//  root2DHistogramOfElectronsTotalMotionalDirectionZX_Momentum.Fill(eTotalThetaZX,
//                                                                   eTotalP);
//  root2DHistogramOfElectronsTotalMotionalDirectionZY_Momentum.Fill(eTotalThetaZY,
//                                                                   eTotalP);
//  root1DHistogramOf1stHitElectronEnergy.Fill(e1HitE);
//  root1DHistogramOf2ndHitElectronEnergy.Fill(e2HitE);
//  root1DHistogramOf3rdHitElectronEnergy.Fill(e3HitE);
//  root1DHistogramOf4thHitElectronEnergy.Fill(e4HitE);
//  root2DHistogramOf1stHitElectronEnergy_2ndHitElectronEnergy.Fill(e1HitE,
//                                                                  e2HitE);
//  root2DHistogramOf2ndHitElectronEnergy_3rdHitElectronEnergy.Fill(e2HitE,
//                                                                  e3HitE);
//  root2DHistogramOf3rdHitElectronEnergy_4thHitElectronEnergy.Fill(e3HitE,
//                                                                  e4HitE);
//  root1DHistogramOfElectronsTotalEnergy.Fill(eTotalE);
//}
//void Analysis::Run::fillIonAndElectronBasicData() {
//
//}
//void Analysis::Run::fillIonAndElectronMomentumData() {
//  const double &iSumOfTOF = pIons->getSumOfTOF(*pUnit);
//  const double &iTotalE = pIons->getEnergy(*pUnit);
//  const double &e1HitE = pElectrons->getRealOrDummyObject(0).getEnergy(*pUnit);
//  root2DHistogramOfSumOfIonTOF_1stHitElectronEnergy.Fill(iSumOfTOF, e1HitE);
//  root2DHistogramOfTotalEnergy_1stHitElectronEnergy.Fill(iTotalE, e1HitE);
//}
const long Analysis::Run::getEntries() const {
  return (long) pEventChain->GetEntries();
}
void Analysis::Run::createNatureHists() {
  pHist->create1d(histID_1stHitIonTOF_under2ndAnd3rdHitIonAreNotDead,
                  "h1_i1TOF_i2Andi3AreNotDead",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfNatureHists);
  pHist->create2d(histID_2ndAnd3rdHitIonTOF_under1stHitIonIsInMasterRegion,
                  "h2_i2TOF_i3TOF_i1IsInMasterRegion",
                  "1st Hit Ion TOF [ns]", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfNatureHists);
  pHist->create2d(histID_1stHitElecEAndSumOfIonTOFs_underMasterCondition,
                  "h2_e1E_iSumTOF_master",
                  "Energy [eV]", "Sum of TOFs [ns]",
                  H2_ELECTRON_ENERGY, H2_ION_SUMOFTOF(4),
                  dirNameOfNatureHists);
  pHist->create1d(histID_1stHitElecE_underMasterCondition,
                  "h1_e1E_master", "Energy [eV]",
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
  pHist->create2d(hist2ID_1stHitIonLocXY_notDead,
                  "h2_1stHitIonLocXY",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndHitIonLocXY_notDead,
                  "h2_2ndHitIonLocXY",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_3rdHitIonLocXY_notDead,
                  "h2_3rdHitIonLocXY",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_4thHitIonLocXY_notDead,
                  "h2_4thHitIonLocXY",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_COMOfIonsLocXY_notDead,
                  "h2_COMOfIonsLocXY",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_1stHitIonLocXY_master,
                  "h2_1stHitIonLocXY_masterCondit",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndHitIonLocXY_master,
                  "h2_2ndHitIonLocXY_masterCondit",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_3rdHitIonLocXY_master,
                  "h2_3rdHitIonLocXY_masterCondit",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_4thHitIonLocXY_master,
                  "h2_4thHitIonLocXY_masterCondit",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_COMOfIonsLocXY_master,
                  "h2_COMOfIonsLocXY_masterCondit",
                  "Location X [mm]", "Location Y [mm]",
                  H2_ION_LOCATION, H2_ION_LOCATION,
                  dirNameOfIonHist);
  // TOF
  pHist->create1d(hist1ID_1stHitIonTOF_notDead,
                  "h1_1stHitIonTOF",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_2ndHitIonTOF_notDead,
                  "h1_2ndHitIonTOF",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_3rdHitIonTOF_notDead,
                  "h1_3rdHitIonTOF",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_4thHitIonTOF_notDead,
                  "h1_4thHitIonTOF",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_1stHitIonTOF_master,
                  "h1_1stHitIonTOF_masterCondit",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_2ndHitIonTOF_master,
                  "h1_2ndHitIonTOF_masterCondit",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_3rdHitIonTOF_master,
                  "h1_3rdHitIonTOF_masterCondit",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  pHist->create1d(hist1ID_4thHitIonTOF_master,
                  "h1_4thHitIonTOF_masterCondit",
                  "TOF [ns]",
                  H1_ION_TOF,
                  dirNameOfIonHist);
  // PIPICO
  pHist->create2d(hist2ID_1stAnd2ndHitIonTOF,
                  "h2_1stAnd2ndHitIonTOF",
                  "1st Hit Ion TOF [ns] ", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndAnd3rdHitIonTOF,
                  "h2_2ndAnd3rdHitIonTOF",
                  "2nd Hit Ion TOF [ns] ", "3rd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_3rdAnd4thHitIonTOF,
                  "h2_3rdAnd4thHitIonTOF",
                  "3rd Hit Ion TOF [ns] ", "4th Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_1stAnd2ndHitIonTOF_masterCondit,
                  "h2_1stAnd2ndHitIonTOF_masterCondit",
                  "1st Hit Ion TOF [ns] ", "2nd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndAnd3rdHitIonTOF_masterCondit,
                  "h2_2ndAnd3rdHitIonTOF_masterCondit",
                  "2nd Hit Ion TOF [ns] ", "3rd Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_3rdAnd4thHitIonTOF_masterCondit,
                  "h2_3rdAnd4thHitIonTOF_masterCondit",
                  "3rd Hit Ion TOF [ns] ", "4th Hit Ion TOF [ns]",
                  H2_ION_TOF, H2_ION_TOF,
                  dirNameOfIonHist);
  // PIPIPICO
  pHist->create2d(hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF,
                  "h2_SumOf1stAnd2ndHitIonTOFAnd3rdHitIonTOF",
                  "Sum of 1st and 2nd hit ion TOFs [ns]",
                  "3rd hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2),
                  H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs,
                  "h2_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs",
                  "1st hit ion TOF [ns]",
                  "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  H2_ION_TOF,
                  H2_ION_SUMOFTOF(2),
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF,
                  "h2_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF",
                  "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2),
                  H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs,
                  "h2_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs",
                  "2nd hit ion TOF [ns]",
                  "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF,
                  H2_ION_SUMOFTOF(2),
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF_masterCondit,
                  "h2_SumOf1stAnd2ndHitIonTOFAnd3rdHitIonTOF_masterCondit",
                  "Sum of 1st and 2nd hit ion TOFs [ns]",
                  "3rd hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2),
                  H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs_masterCondit,
                  "h2_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs_masterCondit",
                  "1st hit ion TOF [ns]",
                  "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  H2_ION_TOF,
                  H2_ION_SUMOFTOF(2),
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF_masterCondit,
                  "h2_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF_masterCondit",
                  "Sum of 2nd and 3rd hit ion TOFs [ns]",
                  "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(2),
                  H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs_masterCondit,
                  "h2_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs_masterCondit",
                  "2nd hit ion TOF [ns]",
                  "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF,
                  H2_ION_SUMOFTOF(2),
                  dirNameOfIonHist);
  // PIPIPIPICO
  pHist->create2d(hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF,
                  "h2_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF",
                  "Sum of 1st, 2nd and 3rd hit ion TOFs [ns]",
                  "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(3),
                  H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF,
                  "h2_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF",
                  "Sum of 1st and 2nd hit ion TOFs [ns]",
                  "Sum of 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_SUMOFTOF(2),
                  H2_ION_SUMOFTOF(2),
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF,
                  "h2_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF",
                  "1st hit ion TOF [ns]",
                  "Sum of 2nd, 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF,
                  H2_ION_SUMOFTOF(3),
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF_masterCondit,
                  "h2_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF_masterCondit",
                  "Sum of 1st, 2nd and 3rd hit ion TOFs [ns]",
                  "4th hit ion TOF [ns]",
                  H2_ION_SUMOFTOF(3),
                  H2_ION_TOF,
                  dirNameOfIonHist);
  pHist->create2d(
      hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF_masterCondit,
      "h2_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF_masterCondit",
      "Sum of 1st and 2nd hit ion TOFs [ns]",
      "Sum of 3rd and 4th hit ion TOFs [ns]",
      H2_ION_SUMOFTOF(2),
      H2_ION_SUMOFTOF(2),
      dirNameOfIonHist);
  pHist->create2d(hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF_masterCondit,
                  "h2_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF_masterCondit",
                  "1st hit ion TOF [ns]",
                  "Sum of 2nd, 3rd and 4th hit ion TOFs [ns]",
                  H2_ION_TOF,
                  H2_ION_SUMOFTOF(3),
                  dirNameOfIonHist);
  //
  pHist->create3d(hist3ID_1stHitIonPxPyPz,
                  "h3_1stHistIonPxPyPz",
                  "Momentum X [au]", "Momentum Y [au]", "Momentum Z [au]",
                  H3_ION_MOMENTUM, H3_ION_MOMENTUM, H3_ION_MOMENTUM,
                  dirNameOfIonHist);
  pHist->create3d(hist3ID_1stHitIonPxPyPz_underMasterCondition,
                  "h3_i1Px_Py_Pz_master",
                  "Momentum X [au]", "Momentum Y [au]", "Momentum Z [au]",
                  H3_ION_MOMENTUM, H3_ION_MOMENTUM, H3_ION_MOMENTUM,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_1stHitIonPDirecXYAndCosPDirecZ,
                  "h2_1stHitIonPDirecXYAndCosPDirecZ",
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_2ndHitIonPDirecXYAndCosPDirecZ,
                  "h2_2ndHitIonPDirecXYAndCosPDirecZ",
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_3rdHitIonPDirecXYAndCosPDirecZ,
                  "h2_3rdHitIonPDirecXYAndCosPDirecZ",
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHist);
  pHist->create2d(hist2ID_4thHitIonPDirecXYAndCosPDirecZ,
                  "h2_4thHitIonPDirecXYAndCosPDirecZ",
                  "Motional Direction XY [degree]", "Motional Direction Z [1]",
                  H2_DEGREE, H2_SINCOS,
                  dirNameOfIonHist);
}
void Analysis::Run::fillIonHists() {
  const bool isIonMaster = pIons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool isElecMaster = pElectrons->areAllFlag(ObjectFlag::WithinMasterRegion);
  const bool isMaster = isIonMaster && isElecMaster;
  const double x1 = pIons->getRealOrDummyIon(0).getLocationX(*pUnit);
  const double y1 = pIons->getRealOrDummyIon(0).getLocationY(*pUnit);
  const double t1 = pIons->getRealOrDummyIon(0).getTOF(*pUnit);
  const double x2 = pIons->getRealOrDummyIon(1).getLocationX(*pUnit);
  const double y2 = pIons->getRealOrDummyIon(1).getLocationY(*pUnit);
  const double t2 = pIons->getRealOrDummyIon(1).getTOF(*pUnit);
  const double x3 = pIons->getRealOrDummyIon(2).getLocationX(*pUnit);
  const double y3 = pIons->getRealOrDummyIon(2).getLocationY(*pUnit);
  const double t3 = pIons->getRealOrDummyIon(2).getTOF(*pUnit);
  const double x4 = pIons->getRealOrDummyIon(3).getLocationX(*pUnit);
  const double y4 = pIons->getRealOrDummyIon(3).getLocationY(*pUnit);
  const double t4 = pIons->getRealOrDummyIon(3).getTOF(*pUnit);

  // Detector image and TOF
  if (!pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::Dead)) {
    pHist->fill2d(hist2ID_1stHitIonLocXY_notDead, x1, y1);
    pHist->fill1d(hist1ID_1stHitIonTOF_notDead, t1);
  }
  if (!pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::Dead)) {
    pHist->fill2d(hist2ID_2ndHitIonLocXY_notDead, x2, y2);
    pHist->fill1d(hist1ID_2ndHitIonTOF_notDead, t2);
  }
  if (!pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::Dead)) {
	  pHist->fill2d(hist2ID_3rdHitIonLocXY_notDead, x3, y3);
	  pHist->fill1d(hist1ID_3rdHitIonTOF_notDead, t3);
  }
  if (!pIons->getRealOrDummyIon(3).isFlag(ObjectFlag::Dead)) {
	  pHist->fill2d(hist2ID_4thHitIonLocXY_notDead, x4, y4);
	  pHist->fill1d(hist1ID_4thHitIonTOF_notDead, t4);
  }
  // PIPICO 
  if (!pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::Dead) && !pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::Dead)) {
	  pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF, t1, t2);
  }
  if (!pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::Dead) && !pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::Dead)) {
	  pHist->fill2d(hist2ID_2ndAnd3rdHitIonTOF, t2, t3);
  }
  if (!pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::Dead) && !pIons->getRealOrDummyIon(3).isFlag(ObjectFlag::Dead)) {
	  pHist->fill2d(hist2ID_3rdAnd4thHitIonTOF, t3, t4);
  }
  // Under master condition 
  if (isMaster) {
	// Detector image and TOF
    if (!pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::Dead)) {
      pHist->fill2d(hist2ID_1stHitIonLocXY_master, x1, y1);
      pHist->fill1d(hist1ID_1stHitIonTOF_master, t1);
    }
    if (!pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::Dead)) {
      pHist->fill2d(hist2ID_2ndHitIonLocXY_master, x2, y2);
      pHist->fill1d(hist1ID_2ndHitIonTOF_master, t2);
    }
	if (!pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::Dead)) {
		pHist->fill2d(hist2ID_3rdHitIonLocXY_master, x3, y3);
		pHist->fill1d(hist1ID_3rdHitIonTOF_master, t3);
	}
	if (!pIons->getRealOrDummyIon(3).isFlag(ObjectFlag::Dead)) {
		pHist->fill2d(hist2ID_4thHitIonLocXY_master, x4, y4);
		pHist->fill1d(hist1ID_4thHitIonTOF_master, t4);
	}
	// PIPICO 
	if (!pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::Dead) && !pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::Dead)) {
		pHist->fill2d(hist2ID_1stAnd2ndHitIonTOF_masterCondit, t1, t2);
	}
	if (!pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::Dead) && !pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::Dead)) {
		pHist->fill2d(hist2ID_2ndAnd3rdHitIonTOF_masterCondit, t2, t3);
	}
	if (!pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::Dead) && !pIons->getRealOrDummyIon(3).isFlag(ObjectFlag::Dead)) {
		pHist->fill2d(hist2ID_3rdAnd4thHitIonTOF_masterCondit, t3, t4);
	}
  }

  //
  if (pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::HavingProperPzData)) {
    pHist->fill3d(hist3ID_1stHitIonPxPyPz,
                  pIons->getIon(0).getMomentumX(*pUnit),
                  pIons->getIon(0).getMomentumY(*pUnit),
                  pIons->getIon(0).getMomentumZ(*pUnit));
    pHist->fill2d(hist2ID_1stHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(0).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(0).getMotionalDirectionZ()));
  }
  if (pIons->getRealOrDummyIon(1).isFlag(ObjectFlag::HavingProperPzData)) {
    pHist->fill2d(hist2ID_2ndHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(1).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(1).getMotionalDirectionZ()));
  }
  if (pIons->getRealOrDummyIon(2).isFlag(ObjectFlag::HavingProperPzData)) {
    pHist->fill2d(hist2ID_3rdHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(2).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(2).getMotionalDirectionZ()));
  }
  if (pIons->getRealOrDummyIon(3).isFlag(ObjectFlag::HavingProperPzData)) {
    pHist->fill2d(hist2ID_4thHitIonPDirecXYAndCosPDirecZ,
                  pIons->getIon(3).getMotionalDirectionXY(*pUnit),
                  cos(pIons->getIon(3).getMotionalDirectionZ()));
  }
  // Under master condition
  if (isMaster) {
    if (pIons->getRealOrDummyIon(0).isFlag(ObjectFlag::HavingProperPzData)) {
      pHist->fill3d(hist3ID_1stHitIonPxPyPz_underMasterCondition,
                    pIons->getIon(0).getMomentumX(*pUnit),
                    pIons->getIon(0).getMomentumY(*pUnit),
                    pIons->getIon(0).getMomentumZ(*pUnit));
    }
  }
}