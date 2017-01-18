#include <TSystem.h>
#include <TApplication.h>
#include "SortWrapper.h"
#include "SortRun.h"

__int32 my_kbhit(void) {
  struct termios term, oterm;
  __int32 fd = 0;
  __int32 c = 0;
  tcgetattr(fd, &oterm);
  memcpy(&term, &oterm, sizeof(term));
  term.c_lflag = term.c_lflag & (!ICANON);
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = 1;
  tcsetattr(fd, TCSANOW, &term);
  c = getchar();
  tcsetattr(fd, TCSANOW, &oterm);
  if (c == -1) return 0;
  return c;
}

int main(int argc, char *argv[]) {
  // Inform status
  if (argc < 2) {
    printf("Please provide a filename.\n");
    printf("syntax: SortExe filename\n");
    printf("        This file will be sorted and\n");
    printf("        a new file will be written.\n\n");
    return 0;
  }
  if (argc > 2) {
    printf("Too many arguments\n");
    printf("syntax: SortExe filename\n");
    printf("        This file will be sorted and\n");
    printf("        a new file will be written.\n\n");
    return 0;
  }
  std::cout << "The exe file which place at " << argv[0] << ", is running now. " << std::endl;
  std::cout << "The configure file which place at " << argv[1] << ", is going to be read. " << std::endl;

  // start the Root-Environment
  std::cout << "Opening the ROOT App... ";
  char *root_argv[3];
  char argv2[50], argv3[50];
  int root_argc = 2;
  sprintf(argv2, "-l");
  root_argv[0] = argv[0];
  root_argv[1] = argv2;
  root_argv[2] = argv3;
  TApplication theRootApp("theRootApp", &root_argc, root_argv);
  std::cout << "ok" << std::endl;

  // Open the JSON reader
  std::cout << "Opening the config file... ";
  Analysis::JSONReader *pReader;
  pReader = new Analysis::JSONReader();
  pReader->appendDoc(Analysis::JSONReader::fromFile, argv[1]);
  std::cout << "ok" << std::endl;

  // Change the working directory
  std::string path;
  if (pReader->hasMember("working_directory")) {
    path = pReader->getStringAt("working_directory");
  } else {
    path = argv[1];
    path = path.substr(0, path.find_last_of("/\\"));
  }
  std::cout << "changing path to `" << path << "'... ";
  chdir(path.c_str());
  std::cout << "okay" << std::endl;
  Analysis::SortRun *pRun;

  // Read options
  const auto isDrawingCanvases = pReader->get<bool>("draw_canvases");
  const auto maxElecHits = pReader->get<int>("maxium_of_electron_hits");
  const auto maxIonHits = pReader->get<int>("maxium_of_ion_hits");
  const auto bunchCh = pReader->get<int>("bunch_marker_ch")-1;
  auto bunchMaskRm = Analysis::readBunchMaskRm(*pReader, "remove_bunch_region");

  // Setup LMF files
  Analysis::LMFWrapper aLMFWrapper;
  aLMFWrapper.readConfig(*pReader);

  // create the sorter
  Analysis::SortWrapper iSortWrapper(&aLMFWrapper), eSortWrapper(&aLMFWrapper);
  {
    bool b1, b2;
    b1 = iSortWrapper.readConfig(*pReader, "ion_sorter");
    b2 = eSortWrapper.readConfig(*pReader, "electron_sorter");
    if (!b1 || !b2) throw std::invalid_argument("Fail to read the config file!");
    iSortWrapper.readCalibTab();
    eSortWrapper.readCalibTab();
  }

  // Close the JSON reader
  std::cout << "Closing the config file... ";
  if (pReader) {
    delete pReader;
    pReader = nullptr;
  }
  std::cout << "ok" << std::endl;

  // Check cmd
  if (iSortWrapper.getCmd() > Analysis::SortWrapper::kSort
      && eSortWrapper.getCmd() > Analysis::SortWrapper::kSort) {
    printf("Error: Do not calibrate 2 detectors simultaneously.\n");
    return 0;
  }
  if (iSortWrapper.getCmd() == Analysis::SortWrapper::kNoDetector
      && eSortWrapper.getCmd() == Analysis::SortWrapper::kNoDetector) {
    printf("no config file was read. Nothing to do.\n");
    return 0;
  }

  { // initialization of the sorters
    bool result;
    result = iSortWrapper.init();
    if (!result) throw std::invalid_argument("Fail to init the ion sorter!");
    result = eSortWrapper.init();
    if (!result) throw std::invalid_argument("Fail to init the electron sorter!");
  }

  bool theLoopIsOn = true;
  const int numLMF = (const int) aLMFWrapper.filenames.size();
  for (int iLMF=0; iLMF < numLMF; iLMF++) {
    // Check keyboard hit
    if (!theLoopIsOn) break;

    { // Read a LMF file
      bool result;
      result = aLMFWrapper.readFile(iLMF);
      if (!result) break;
    }

    // Setup Run
    pRun = new Analysis::SortRun("ResortLess", maxIonHits, maxElecHits);
    std::cout << "A root file is open for output." << std::endl;
    if (isDrawingCanvases) {
      if (!iSortWrapper.isNull()) pRun->createC1();
      if (!eSortWrapper.isNull()) pRun->createC2();
    }
    gSystem->ProcessEvents(); // allow the system to show the histograms

    // Start reading event data from input file:
    // ("event" is all the data that was recorded after a trigger signal)
    printf("reading event data... ");
    while (true) {
      {
        auto &pLMF = aLMFWrapper.pLMF;
        if (pLMF->GetEventNumber() % 20000 == 1) {
          if (my_kbhit()) {
            std::cout << "The keyboard is hit. Closing the program." << std::endl;
            theLoopIsOn = false;
            break;
          }
          gSystem->ProcessEvents(); // allow the system to show the histograms
          printf("\rreading event data... %2i %c  ",
                 __int32(100 * pLMF->GetEventNumber() / pLMF->uint64_Numberofevents),
                 37);
          if (pLMF->GetEventNumber() % 60000 == 1) {
            pRun->updateC1();
            pRun->updateC2();
          }
        }
      }

      { // read one new event data block from the file:
        const bool b = aLMFWrapper.readNextEvent();
        if (!b) {
          std::cout << "Done with reading one LMF file." << std::endl;
          break;
        }
      }

      // convert the raw TDC data to nanoseconds
      iSortWrapper.convertTDC();
      eSortWrapper.convertTDC();

      // fill
      { // TDC ns
        auto &tdc_ns = aLMFWrapper.TDCns;
        const int idxhist = Analysis::SortRun::h1_TDC01;
        const int numhist = 16;
        for (int i=0; i<numhist; i++) {
          pRun->fill1d(idxhist+i, tdc_ns[i][0]);
        }
      }
      if (!iSortWrapper.isNull()) { // ion
        const auto &wrapper = iSortWrapper;
        const auto x_dev = wrapper.getXDev();
        const auto y_dev = wrapper.getYDev();
        const auto weight_dev = wrapper.getWeightDev();
        const auto x_raw = wrapper.getXRaw();
        const auto y_raw = wrapper.getYRaw();
        const auto u_timesum = wrapper.getUTimesum();
        const auto u_timediff = wrapper.getUTimediff();
        const auto v_timesum = wrapper.getVTimesum();
        const auto v_timediff = wrapper.getVTimediff();
        const auto w_timesum = wrapper.getWTimesum();
        const auto w_timediff = wrapper.getWTimediff();

        pRun->fill2d(Analysis::SortRun::h2_ionXYDev, x_dev, y_dev, *weight_dev);
        pRun->fill2d(Analysis::SortRun::h2_ionXYRaw, x_raw, y_raw);
        pRun->fill1d(Analysis::SortRun::h1_ionTimesumU, u_timesum);
        pRun->fill1d(Analysis::SortRun::h1_ionTimediffU, u_timediff);
        pRun->fill2d(Analysis::SortRun::h2_ionTimesumDiffU, u_timediff, u_timesum);
        pRun->fill1d(Analysis::SortRun::h1_ionTimesumV, v_timesum);
        pRun->fill1d(Analysis::SortRun::h1_ionTimediffV, v_timediff);
        pRun->fill2d(Analysis::SortRun::h2_ionTimesumDiffV, v_timediff, v_timesum);
        pRun->fill1d(Analysis::SortRun::h1_ionTimesumW, w_timesum);
        pRun->fill1d(Analysis::SortRun::h1_ionTimediffW, w_timediff);
        pRun->fill2d(Analysis::SortRun::h2_ionTimesumDiffW, w_timediff, w_timesum);
      }
      if (!eSortWrapper.isNull()) { // electron
        const auto &wrapper = eSortWrapper;
        const auto x_dev = wrapper.getXDev();
        const auto y_dev = wrapper.getYDev();
        const auto weight_dev = wrapper.getWeightDev();
        const auto x_raw = wrapper.getXRaw();
        const auto y_raw = wrapper.getYRaw();
        const auto u_timesum = wrapper.getUTimesum();
        const auto u_timediff = wrapper.getUTimediff();
        const auto v_timesum = wrapper.getVTimesum();
        const auto v_timediff = wrapper.getVTimediff();
        const auto w_timesum = wrapper.getWTimesum();
        const auto w_timediff = wrapper.getWTimediff();

        pRun->fill2d(Analysis::SortRun::h2_elecXYDev, x_dev, y_dev, *weight_dev);
        pRun->fill2d(Analysis::SortRun::h2_elecXYRaw, x_raw, y_raw);
        pRun->fill1d(Analysis::SortRun::h1_elecTimesumU, u_timesum);
        pRun->fill1d(Analysis::SortRun::h1_elecTimediffU, u_timediff);
        pRun->fill2d(Analysis::SortRun::h2_elecTimesumDiffU, u_timediff, u_timesum);
        pRun->fill1d(Analysis::SortRun::h1_elecTimesumV, v_timesum);
        pRun->fill1d(Analysis::SortRun::h1_elecTimediffV, v_timediff);
        pRun->fill2d(Analysis::SortRun::h2_elecTimesumDiffV, v_timediff, v_timesum);
        pRun->fill1d(Analysis::SortRun::h1_elecTimesumW, w_timesum);
        pRun->fill1d(Analysis::SortRun::h1_elecTimediffW, w_timediff);
        pRun->fill2d(Analysis::SortRun::h2_elecTimesumDiffW, w_timediff, w_timesum);
      }

      iSortWrapper.sort();
      eSortWrapper.sort();
      const int numHitIons = iSortWrapper.getNumHits();
      const int numHitElecs = eSortWrapper.getNumHits();
      for (int i=0; i<numHitIons; i++)
        pRun->fill2d(Analysis::SortRun::h2_ionXY,
                     iSortWrapper.getOutputArr()[i]->x,
                     iSortWrapper.getOutputArr()[i]->y);
      for (int i=0; i<numHitElecs; i++)
        pRun->fill2d(Analysis::SortRun::h2_elecXY,
                     eSortWrapper.getOutputArr()[i]->x,
                     eSortWrapper.getOutputArr()[i]->y);

      // get bunch marker
      const double *pBunchMarker = nullptr;
      if (!eSortWrapper.isNull()) {
        const auto mcp = eSortWrapper.getMCP();
        if (mcp != nullptr) {
          const auto &TDC = aLMFWrapper.TDC;
          const auto TDCRes = aLMFWrapper.TDCRes;
          pBunchMarker = new auto(*mcp - TDC[bunchCh][0] * TDCRes);
        }
        pRun->fill1d(Analysis::SortRun::h1_bunchMarker, pBunchMarker);
      }
      if (!bunchMaskRm.isIn(pBunchMarker)) {
        pRun->fill1d(Analysis::SortRun::h1_bunchMarkerAfterRm, pBunchMarker);
//        auto iSorter = iSortWrapper.pSorter;
//        auto eSorter = eSortWrapper.pSorter;
//        pRun->fill1d(Analysis::SortRun::h1_bunchMarker, eMarker);
//        if(number_of_ions >= 1) {
//          double &x = iSorter->output_hit_array[0]->x;
//          double &y = iSorter->output_hit_array[0]->y;
//          double &t = iSorter->output_hit_array[0]->time;
//          pRun->fill2d(Analysis::SortRun::h2_ion1hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_ion1hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion1hitXY, x, y);
//        }
//        if(number_of_ions >= 2) {
//          double &x = iSorter->output_hit_array[1]->x;
//          double &y = iSorter->output_hit_array[1]->y;
//          double &t = iSorter->output_hit_array[1]->time;
//          double &tt = iSorter->output_hit_array[0]->time;
//          pRun->fill2d(Analysis::SortRun::h2_ion2hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_ion2hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion2hitXY, x, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion1hit2hitPIPICO, tt, t);
//        }
//        if(number_of_ions >= 3) {
//          double &x = iSorter->output_hit_array[2]->x;
//          double &y = iSorter->output_hit_array[2]->y;
//          double &t = iSorter->output_hit_array[2]->time;
//          double &tt = iSorter->output_hit_array[1]->time;
//          pRun->fill2d(Analysis::SortRun::h2_ion3hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_ion3hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion3hitXY, x, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion2hit3hitPIPICO, tt, t);
//        }
//        if(number_of_ions >= 4) {
//          double &x = iSorter->output_hit_array[3]->x;
//          double &y = iSorter->output_hit_array[3]->y;
//          double &t = iSorter->output_hit_array[3]->time;
//          double &tt = iSorter->output_hit_array[2]->time;
//          pRun->fill2d(Analysis::SortRun::h2_ion4hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_ion4hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion4hitXY, x, y);
//          pRun->fill2d(Analysis::SortRun::h2_ion3hit4hitPIPICO, tt, t);
//        }
//        if(number_of_electrons >= 1) {
//          double &x = eSorter->output_hit_array[0]->x;
//          double &y = eSorter->output_hit_array[0]->y;
//          double &t = eSorter->output_hit_array[0]->time;
//          pRun->fill2d(Analysis::SortRun::h2_elec1hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_elec1hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec1hitXY, x, y);
//        }
//        if(number_of_electrons >= 2) {
//          double &x = eSorter->output_hit_array[1]->x;
//          double &y = eSorter->output_hit_array[1]->y;
//          double &t = eSorter->output_hit_array[1]->time;
//          double &tt = eSorter->output_hit_array[0]->time;
//          pRun->fill2d(Analysis::SortRun::h2_elec2hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_elec2hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec2hitXY, x, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec1hit2hitPEPECO, tt, t);
//        }
//        if(number_of_electrons >= 3) {
//          double &x = eSorter->output_hit_array[2]->x;
//          double &y = eSorter->output_hit_array[2]->y;
//          double &t = eSorter->output_hit_array[2]->time;
//          double &tt = eSorter->output_hit_array[1]->time;
//          pRun->fill2d(Analysis::SortRun::h2_elec3hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_elec3hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec3hitXY, x, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec2hit3hitPEPECO, tt, t);
//        }
//        if(number_of_electrons >= 4) {
//          double &x = eSorter->output_hit_array[3]->x;
//          double &y = eSorter->output_hit_array[3]->y;
//          double &t = eSorter->output_hit_array[3]->time;
//          double &tt = eSorter->output_hit_array[2]->time;
//          pRun->fill2d(Analysis::SortRun::h2_elec4hitXFish, t, x);
//          pRun->fill2d(Analysis::SortRun::h2_elec4hitYFish, t, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec4hitXY, x, y);
//          pRun->fill2d(Analysis::SortRun::h2_elec3hit4hitPEPECO, tt, t);
//        }
//        Analysis::SortRun::DataSet *pIons, *pElecs;
//        pIons = new Analysis::SortRun::DataSet[number_of_ions];
//        pElecs = new Analysis::SortRun::DataSet[number_of_electrons];
//        for (int i = 0; i < number_of_ions; i++) {
//          pIons[i].x = iSorter->output_hit_array[i]->x;
//          pIons[i].y = iSorter->output_hit_array[i]->y;
//          pIons[i].t = iSorter->output_hit_array[i]->time;
//          pIons[i].flag = iSorter->output_hit_array[i]->method;
//        }
//        for (int i = 0; i < number_of_electrons; i++) {
//          pElecs[i].x = eSorter->output_hit_array[i]->x;
//          pElecs[i].y = eSorter->output_hit_array[i]->y;
//          pElecs[i].t = eSorter->output_hit_array[i]->time;
//          pElecs[i].flag = eSorter->output_hit_array[i]->method;
//        }
//        pRun->fillTree(number_of_ions, pIons, number_of_electrons, pElecs);
//        if (pIons) {
//          delete[] pIons;
//          pIons = nullptr;
//        }
//        if (pElecs) {
//          delete[] pElecs;
//		  pElecs = nullptr;
//        }
      }

      { // check if it's full
        bool b1, b2;
        b1 = iSortWrapper.isFull();
        b2 = iSortWrapper.isFull();
        if (b1 || b2) {
          std::cout << "ionSorter: map is full enough" << std::endl;
          theLoopIsOn = false;
          break;
        }
      }
    } // end of the loop reading events
    printf("ok\n");

    // calib
    iSortWrapper.calibFactors();
    eSortWrapper.calibFactors();
    iSortWrapper.genClibTab();
    eSortWrapper.genClibTab();

    // cleanup
    if (pRun != nullptr) {
      pRun->updateC1(true);
      pRun->updateC2(true);
      delete pRun;
      pRun = nullptr;
    }
    aLMFWrapper.cleanup();
  } // end of the loop reading LMF files

  printf("hit any key to exit\n");
  while (true) {
    gSystem->Sleep(5);
    gSystem->ProcessEvents();
    if (my_kbhit()) {
      break;
    }
  }

  // Finish the program
  printf("terminating the root app.\n");
  theRootApp.Terminate();
  std::cout << "The program is done. " << std::endl;
  return 0;
}
