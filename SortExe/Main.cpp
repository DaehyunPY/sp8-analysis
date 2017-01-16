#include <cstdlib>
#include <cmath>
#include <termios.h>
#include <TSystem.h>
#include <TApplication.h>
#include "resort64c.h"
#include "LMF_IO.h"
#include "SortRun.h"

#define NUM_IONS 200
#define NUM_CHANNELS 80

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

void readline_from_config_file(FILE *ffile, char *text, __int32 max_len) {
  int i;
  text[0] = 0;

  while (true) {
    i = -1;
    char c = 1;
    bool real_numbers_found = false;
    bool start_of_line = true;

    while (true) {
      fread(&c, 1, 1, ffile);
      if (c == 13) continue;
      if (start_of_line) {
        if (c == ' ' || c == 9 || c == 10) continue;
        start_of_line = false;
      }
      if (c == '/') { // if there is a comment then read until end of the line
        while (c != 10) {
          fread(&c, 1, 1, ffile);
          if (c == 13) continue;
        }
        if (real_numbers_found) break;
        start_of_line = true;
        continue;
      }
      real_numbers_found = true;
      if (c == ' ' || c == 9) break;
      if (c == 10) break;
      if (c == ',') break;
      ++i;
      if (i < max_len - 1) {
        text[i] = c;
        text[i + 1] = 0;
      }
    }
    if (real_numbers_found) {
      break;
    }
  }
}

int read_int(FILE *ffile) {
  char a[1024];
  readline_from_config_file(ffile, a, 1024);
  return std::atoi(a);
}

double read_double(FILE *ffile) {
  char a[1024];
  readline_from_config_file(ffile, a, 1024);
  return double(std::atof(a));
}

bool read_calibration_tables(const char *filename, sort_class *sorter) {
  if (!filename) return false;
  if (!sorter) return false;

  FILE *infile_handle = fopen(filename, "rt");
  if (!infile_handle) return false;
  int points;

  points = read_int(infile_handle);
  for (int j = 0; j < points; ++j) {
    double x = read_double(infile_handle);
    double y = read_double(infile_handle);
    if (sorter->use_sum_correction) sorter->signal_corrector->sum_corrector_U->set_point(x, y);
  }
  points = read_int(infile_handle);
  for (int j = 0; j < points; ++j) {
    double x = read_double(infile_handle);
    double y = read_double(infile_handle);
    if (sorter->use_sum_correction) sorter->signal_corrector->sum_corrector_V->set_point(x, y);
  }
  if (sorter->use_HEX) {
    points = read_int(infile_handle);
    for (int j = 0; j < points; ++j) {
      double x = read_double(infile_handle);
      double y = read_double(infile_handle);
      if (sorter->use_sum_correction) sorter->signal_corrector->sum_corrector_W->set_point(x, y);
    }
  }

  points = read_int(infile_handle);
  for (int j = 0; j < points; ++j) {
    double x = read_double(infile_handle);
    double y = read_double(infile_handle);
    if (sorter->use_pos_correction) sorter->signal_corrector->pos_corrector_U->set_point(x, y);
  }
  points = read_int(infile_handle);
  for (int j = 0; j < points; ++j) {
    double x = read_double(infile_handle);
    double y = read_double(infile_handle);
    if (sorter->use_pos_correction) sorter->signal_corrector->pos_corrector_V->set_point(x, y);
  }
  if (sorter->use_HEX) {
    points = read_int(infile_handle);
    for (int j = 0; j < points; ++j) {
      double x = read_double(infile_handle);
      double y = read_double(infile_handle);
      if (sorter->use_pos_correction) sorter->signal_corrector->pos_corrector_W->set_point(x, y);
    }
  }

  if (infile_handle) fclose(infile_handle);
  return true;
}

bool create_calibration_tables(const char *filename, sort_class *sorter) {
  if (!sorter) return false;
  if (!filename) return false;
  FILE *fo = fopen(filename, "wt");
  sorter->do_calibration();
  int number_of_columns = sorter->sum_walk_calibrator->sumu_profile->number_of_columns;
  fprintf(fo, "\n\n%i  	// number of sum calibration points for layer U\n", number_of_columns);
  for (int binx = 0; binx < number_of_columns; ++binx) {
    double x, y;
    sorter->sum_walk_calibrator->get_correction_point(x, y, binx, 0); // 0 = layer u
    fprintf(fo, "%lg  %lg\n", x, y);
  }
  number_of_columns = sorter->sum_walk_calibrator->sumv_profile->number_of_columns;
  fprintf(fo, "\n\n%i  	// number of sum calibration points for layer V\n", number_of_columns);
  for (int binx = 0; binx < number_of_columns; ++binx) {
    double x, y;
    sorter->sum_walk_calibrator->get_correction_point(x, y, binx, 1); // 1 = layer v
    fprintf(fo, "%lg  %lg\n", x, y);
  }
  number_of_columns = sorter->sum_walk_calibrator->sumw_profile->number_of_columns;
  fprintf(fo, "\n\n%i  	// number of sum calibration points for layer W (only needed for HEX-detectors)\n",
          number_of_columns);
  if (sorter->use_HEX) {
    for (int binx = 0; binx < number_of_columns; ++binx) {
      double x, y;
      sorter->sum_walk_calibrator->get_correction_point(x, y, binx, 2); // 2 = layer w
      fprintf(fo, "%lg  %lg\n", x, y);
    }
  }

  number_of_columns = sorter->pos_walk_calibrator->number_of_columns;
  fprintf(fo, "\n\n%i  	// number of pos-calibration points for layer U\n", number_of_columns);
  for (int binx = 0; binx < number_of_columns; ++binx) {
    double x, y;
    sorter->pos_walk_calibrator->get_correction_point(x, y, binx, 0); // 0 = layer u
    fprintf(fo, "%lg  %lg\n", x, y);
  }
  number_of_columns = sorter->pos_walk_calibrator->number_of_columns;
  fprintf(fo, "\n\n%i  	// number of pos-calibration points for layer V\n", number_of_columns);
  for (int binx = 0; binx < number_of_columns; ++binx) {
    double x, y;
    sorter->pos_walk_calibrator->get_correction_point(x, y, binx, 1); // 1 = layer v
    fprintf(fo, "%lg  %lg\n", x, y);
  }
  number_of_columns = sorter->pos_walk_calibrator->number_of_columns;
  fprintf(fo, "\n\n%i  	// number of pos-calibration points for layer W (only needed for HEX-detectors)\n",
          number_of_columns);
  if (sorter->use_HEX) {
    for (int binx = 0; binx < number_of_columns; ++binx) {
      double x, y;
      sorter->pos_walk_calibrator->get_correction_point(x, y, binx, 2); // 2 = layer w
      fprintf(fo, "%lg  %lg\n", x, y);
    }
  }
  fclose(fo);
  return true;
}

namespace Analysis {
struct LMFWrapper {
  LMF_IO *pLMF;
  std::vector<std::string> filenames;
  const double TDCRes = 0.025; // 25ps tdc bin size
  int TDC[NUM_CHANNELS][NUM_IONS];
  double tdc_ns[NUM_CHANNELS][NUM_IONS];
  unsigned int count[NUM_CHANNELS];

  bool readConfig(const JSONReader &reader) {
    auto pStr = reader.getOpt<const char *>("LMF_files");
    if (pStr != nullptr) {
      filenames.push_back(std::string(*pStr));
      return true;
    } else {
      auto pArr = reader.getOptArr<const char *>("LMF_files");
      if (pArr == nullptr) return false;
      for (auto str: *pArr) filenames.push_back(std::string(str));
      return true;
    }
  }
  bool readFile(const int i) {
    pLMF = new LMF_IO(NUM_CHANNELS, NUM_IONS);
    bool b;
    b = pLMF->OpenInputLMF(filenames[i]);
    if (b) {
      std::cout << "A LMF file " << filenames[i] << " is open for reading!" << std::endl;
      return true;
    } else {
      std::cout << "Could not open LMF file: " << filenames[i] << std::endl;
      return false;
    }
  }
};

enum SortCmd {
  // -1 = detector does not exist
  //  0 = only convert to new file format
  //  1 = sort and write new file
  //  2 = calibrate fv, fw, w_offset
  //  3 = create calibration table files
      noDetector = -1,
  onlyConvert = 0,
  sort = 1,
  calib = 2,
  genCalibTab = 3
};

struct SortWrapper {
  sort_class *sorter = new sort_class();
  SortCmd cmd = noDetector;
  std::string calibTabFilename;
  std::map<std::string, int> chMap;
  std::map<std::string, double> factors;

  bool readConfig(const JSONReader &reader, const std::string prefix) {
    std::cout << "Initialize sorter from config file... " << std::endl;
    if (!reader.hasMember(prefix)) throw std::invalid_argument("The member does not exist!");

    cmd = (SortCmd) reader.get<int>(prefix + ".cmd");
    if (cmd == -1) {
      std::cout << "ok" << std::endl;
      return false;
    }
    calibTabFilename = reader.get<const char *>(prefix + ".calibration_table");

    chMap = reader.getMap<int>(prefix + ".channel_map");
    sorter->use_HEX = reader.getBoolAt(prefix + ".hexanode_used");
    sorter->common_start_mode = reader.getBoolAt(prefix + ".common_start_mode");
    sorter->Cu1 = chMap["u1"] - 1;
    sorter->Cu2 = chMap["u2"] - 1;
    sorter->Cv1 = chMap["v1"] - 1;
    sorter->Cv2 = chMap["v2"] - 1;
    sorter->Cw1 = chMap["w1"] - 1;
    sorter->Cw2 = chMap["w2"] - 1;
    sorter->Cmcp = chMap["MCP"] - 1;
    sorter->use_MCP = (sorter->Cmcp) > -1;

    factors = reader.getMap<double>(prefix + ".factors");
    sorter->uncorrected_time_sum_half_width_u = factors["halfwidth_u"];
    sorter->uncorrected_time_sum_half_width_v = factors["halfwidth_v"];
    sorter->uncorrected_time_sum_half_width_w = factors["halfwidth_w"];
    for (auto key: {"runtime_u", "runtime_v", "runtime_w"}) {
      auto found = factors.find(key);
      if (found == factors.end()) factors[key] = factors["runtime"];
    }
    sorter->runtime_u = factors["runtime_u"];
    sorter->runtime_v = factors["runtime_v"];
    sorter->runtime_w = factors["runtime_w"];
    sorter->fu = 0.5 * factors["fu"];
    sorter->fv = 0.5 * factors["fv"];
    sorter->fw = 0.5 * factors["fw"];

    sorter->MCP_radius = reader.getDoubleAt(prefix + ".MCP_radius");
    sorter->dead_time_anode = reader.getDoubleAt(prefix + ".anode_deadtime");
    sorter->dead_time_mcp = reader.getDoubleAt(prefix + ".MCP_deadtime");
    sorter->use_sum_correction = reader.getBoolAt(prefix + ".correct_timesum");
    sorter->use_pos_correction = reader.getBoolAt(prefix + ".correct_position");

    // end of reading the config file:
    std::cout << "ok" << std::endl;
    return true;
  }
  bool init(const double TDCRes, double *pTDCns, int *pCount) {
    if (sorter == nullptr) return true;
    std::cout << "init sorter... " << std::endl;
    sorter->TDC_resolution_ns = TDCRes;
    sorter->tdc_array_row_length = NUM_IONS;
    sorter->count = pCount;
    sorter->tdc_pointer = pTDCns;
    if (cmd >= calib) {
      sorter->create_scalefactors_calibrator(
          true, // BuildStack
          sorter->runtime_u, sorter->runtime_v, sorter->runtime_w, // runtime u, v, w
          0.78, // runtime inner fraction
          sorter->fu, sorter->fv, sorter->fw // fu, fv, fw
      );
    }
    int error_code = sorter->init_after_setting_parameters();
    if (error_code) {
      std::cout << "ion sorter could not be initialized" << std::endl;
      char error_text[512];
      sorter->get_error_text(error_code, 512, error_text);
      printf("Error %i: %s\n", error_code, error_text);
      return false;
    }
    std::cout << "ok" << std::endl;
    return true;
  }
  bool readCalibTab() {
    if (sorter == nullptr) return true;
    if (sorter->use_sum_correction || sorter->use_pos_correction) {
      std::cout << "Reading calibration table: " << calibTabFilename << "... ";
      const bool result = read_calibration_tables(calibTabFilename.c_str(), sorter);
      std::cout << "ok" << std::endl;
      return result;
    }
    return false;
  }
  bool genClibTab() {
    if (sorter == nullptr) return true;
    if (cmd == genCalibTab) {
      std::cout << "Generating calibration table: " << calibTabFilename << "... ";
      const bool result = create_calibration_tables(calibTabFilename.c_str(), sorter);
      std::cout << "ok" << std::endl;
      return result;
    }
    return false;
  }
  bool calibFactors() {
    if (sorter == nullptr) return true;
    if (cmd == calib) {
      std::cout << "Calibrating... ";
      const bool result = sorter->do_calibration();
      std::cout << "ok" << std::endl;
      if (sorter->scalefactors_calibrator) {
        printf("ion: Good scalefactors are:\nf_U = %lg\nf_V = %lg\nf_W = %lg\nOffset on layer W = %lg\n",
               2. * sorter->fu,
               2. * sorter->scalefactors_calibrator->best_fv,
               2. * sorter->scalefactors_calibrator->best_fw,
               sorter->scalefactors_calibrator->best_w_offset);
      }
      return result;
    }
    return false;
  }
  bool cleanup() {
    if (sorter == nullptr) return true;
    delete sorter;
    sorter = nullptr;
    return true;
  }
};
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
  if (!pReader) {
    std::cout << "fail" << std::endl;
    return 0;
  }
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
  const auto chMap = pReader->getMap<int>("channel_map");
  const auto maxElecHits = pReader->get<int>("maxium_of_electron_hits");
  const auto maxIonHits = pReader->get<int>("maxium_of_ion_hits");
  const auto bunchCh = pReader->get<int>("bunch_marker_ch")-1;
  auto bunchMaskRm = Analysis::getBunchMaskRm(*pReader, "remove_bunch_region");

  // Setup LMF files
  Analysis::LMFWrapper aLMFWrapper;
  aLMFWrapper.readConfig(*pReader);

  // create the sorter:
  Analysis::SortWrapper iSortWrapper, eSortWrapper;
  {
    bool b;
    b = iSortWrapper.readConfig(*pReader, "ion_sorter");
    if (!b) throw std::invalid_argument("Fail to read the ion config!");
    iSortWrapper.readCalibTab();
    b = eSortWrapper.readConfig(*pReader, "electron_sorter");
    if (!b) throw std::invalid_argument("Fail to read the electron config!");
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
  if (iSortWrapper.cmd > Analysis::sort && eSortWrapper.cmd > Analysis::sort) {
    printf("Error: Do not calibrate 2 detectors simultaneously.\n");
    return 0;
  }
  if ((int) iSortWrapper.cmd == Analysis::noDetector && (int) eSortWrapper.cmd == Analysis::noDetector) {
    printf("no config file was read. Nothing to do.\n");
    return 0;
  }

  { // initialization of the sorters
    bool result;
    result = iSortWrapper.init(aLMFWrapper.TDCRes, &aLMFWrapper.tdc_ns[0][0], (int *) aLMFWrapper.count);
    if (!result) throw std::invalid_argument("Fail to init the ion sorter!");
    result = eSortWrapper.init(aLMFWrapper.TDCRes, &aLMFWrapper.tdc_ns[0][0], (int *) aLMFWrapper.count);
    if (!result) throw std::invalid_argument("Fail to init the electron sorter!");
  }

  // Open LMF file
  bool theLoopIsOn = true;
  const unsigned long numLMF = aLMFWrapper.filenames.size();
  for (int iLMF; iLMF < numLMF; iLMF++) {
    // Check keyboard hit
    if (!theLoopIsOn) break;

    { // Read a LMF file
      bool result;
      result = aLMFWrapper.readFile(iLMF);
      if (!result) break;
    }
    auto &pLMF = aLMFWrapper.pLMF;
    const double TDCResolution = aLMFWrapper.TDCRes;
    auto &TDC = aLMFWrapper.TDC;
    auto &tdc_ns = aLMFWrapper.tdc_ns;
    auto &count = aLMFWrapper.count;


    // Setup Run
    pRun = new Analysis::SortRun("ResortLess", maxIonHits, maxElecHits);
    std::cout << "A root file is open for output." << std::endl;
    if (isDrawingCanvases) {
      if (iSortWrapper.sorter != nullptr) pRun->createC1();
      if (eSortWrapper.sorter != nullptr) pRun->createC2();
    }
    gSystem->ProcessEvents(); // allow the system to show the histograms

    // Start reading event data from input file:
    // ("event" is all the data that was recorded after a trigger signal)
    printf("reading event data... ");
    while (true) {
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

      // read one new event data block from the file:
      memset(count, 0, pLMF->number_of_channels * sizeof(int));
      if (!pLMF->ReadNextEvent()) {
        std::cout << "Done with reading one LMF file." << std::endl;
        break;
      }

      pLMF->GetNumberOfHitsArray(count);
      //    double timestamp_s = pLMF->GetDoubleTimeStamp(); // absolute timestamp in seconds
      pLMF->GetTDCDataArray((int *) TDC);





      // convert the raw TDC data to nanoseconds
      // ion
      if (iSortWrapper.sorter != nullptr) {
        auto &pSorter = iSortWrapper.sorter;
        const auto cmd = iSortWrapper.cmd;
        double &offset_u = iSortWrapper.factors["offset_u"],
            &offset_v = iSortWrapper.factors["offset_v"],
            &offset_w = iSortWrapper.factors["offset_w"],
            &fw_offset = iSortWrapper.factors["fw_offset"],
            &offset_x = iSortWrapper.factors["offset_x"],
            &offset_y = iSortWrapper.factors["offset_y"];
        if (pSorter->Cmcp > -1) {
          for (unsigned int i = 0; i < count[pSorter->Cmcp]; ++i)
            tdc_ns[pSorter->Cmcp][i] = double(TDC[pSorter->Cmcp][i]) * TDCResolution;
        }
        for (unsigned int i = 0; i < count[pSorter->Cu1]; ++i)
          tdc_ns[pSorter->Cu1][i] = double(TDC[pSorter->Cu1][i]) * TDCResolution;
        for (unsigned int i = 0; i < count[pSorter->Cu2]; ++i)
          tdc_ns[pSorter->Cu2][i] = double(TDC[pSorter->Cu2][i]) * TDCResolution;
        for (unsigned int i = 0; i < count[pSorter->Cv1]; ++i)
          tdc_ns[pSorter->Cv1][i] = double(TDC[pSorter->Cv1][i]) * TDCResolution;
        for (unsigned int i = 0; i < count[pSorter->Cv2]; ++i)
          tdc_ns[pSorter->Cv2][i] = double(TDC[pSorter->Cv2][i]) * TDCResolution;
        if (pSorter->use_HEX) {
          for (unsigned int i = 0; i < count[pSorter->Cw1]; ++i)
            tdc_ns[pSorter->Cw1][i] = double(TDC[pSorter->Cw1][i]) * TDCResolution;
          for (unsigned int i = 0; i < count[pSorter->Cw2]; ++i)
            tdc_ns[pSorter->Cw2][i] = double(TDC[pSorter->Cw2][i]) * TDCResolution;
        }
        if (pSorter->use_HEX) {
          // shift the time sums to zero:
          pSorter->shift_sums(+1, offset_u, offset_v, offset_w);
          // shift layer w so that the middle lines of all layers intersect in one point:
          pSorter->shift_layer_w(+1, fw_offset);
        }
        else {
          // shift the time sums to zero:
          pSorter->shift_sums(+1, offset_u, offset_v);
        }
        // shift all signals from the anode so that the center of the detector is at x=y=0:
        pSorter->shift_position_origin(+1, offset_x, offset_y);

        // for calibration of fv, fw, w_offset and correction tables
        pSorter->feed_calibration_data(true, fw_offset);

        if (pSorter->scalefactors_calibrator && cmd >= 2) {
          if (pSorter->scalefactors_calibrator->map_is_full_enough()) {
            std::cout << "ionSorter: map is full enough" << std::endl;
            theLoopIsOn = false;
            break;
          }
        }
        if (pSorter->use_HEX)
          pRun->fill2d(Analysis::SortRun::h2_ionXYDev,
                       pSorter->scalefactors_calibrator->binx
                           - pSorter->scalefactors_calibrator->detector_map_size / 2.0,
                       pSorter->scalefactors_calibrator->biny
                           - pSorter->scalefactors_calibrator->detector_map_size / 2.0,
                       pSorter->scalefactors_calibrator->detector_map_devi_fill);

        if (count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0) {
          if (count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0) {
            double u = pSorter->fu * (tdc_ns[pSorter->Cu1][0] - tdc_ns[pSorter->Cu2][0]);
            double v = pSorter->fv * (tdc_ns[pSorter->Cv1][0] - tdc_ns[pSorter->Cv2][0]);
            double y = (u - 2. * v) / std::sqrt(3.0);
            pRun->fill2d(Analysis::SortRun::h2_ionXYRaw, u, y);
          }
        }
        if (count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0) {
          pRun->fill1d(Analysis::SortRun::h1_ionU, tdc_ns[pSorter->Cu1][0] - tdc_ns[pSorter->Cu2][0]);
          double mcp = 0.;
          if (pSorter->use_MCP) {
            if (count[pSorter->Cmcp] > 0) mcp = tdc_ns[pSorter->Cmcp][0]; else mcp = -1.e100;
          }
          const double timesum = tdc_ns[pSorter->Cu1][0] + tdc_ns[pSorter->Cu2][0] - 2 * mcp;
          const double timediff = tdc_ns[pSorter->Cu1][0] - tdc_ns[pSorter->Cu2][0];
          pRun->fill1d(Analysis::SortRun::h1_ionTimesumU, timesum);
          pRun->fill2d(Analysis::SortRun::h2_ionTimesumDiffU, timediff, timesum);
        }
        if (count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0) {
          pRun->fill1d(Analysis::SortRun::h1_ionV, tdc_ns[pSorter->Cv1][0] - tdc_ns[pSorter->Cv2][0]);
          double mcp = 0.;
          if (pSorter->use_MCP) {
            if (count[pSorter->Cmcp] > 0) mcp = tdc_ns[pSorter->Cmcp][0]; else mcp = -1.e100;
          }
          const double timesum = tdc_ns[pSorter->Cv1][0] + tdc_ns[pSorter->Cv2][0] - 2 * mcp;
          const double timediff = tdc_ns[pSorter->Cv1][0] - tdc_ns[pSorter->Cv2][0];
          pRun->fill1d(Analysis::SortRun::h1_ionTimesumV, timesum);
          pRun->fill2d(Analysis::SortRun::h2_ionTimesumDiffV, timediff, timesum);
        }
        if (pSorter->use_HEX) {
          if (count[pSorter->Cw1] > 0 && count[pSorter->Cw2] > 0) {
            pRun->fill1d(Analysis::SortRun::h1_ionW, tdc_ns[pSorter->Cw1][0] - tdc_ns[pSorter->Cw2][0]);
            double mcp = 0.;
            if (pSorter->use_MCP) {
              if (count[pSorter->Cmcp] > 0) mcp = tdc_ns[pSorter->Cmcp][0]; else mcp = -1.e100;
            }
            const double timesum = tdc_ns[pSorter->Cw1][0] + tdc_ns[pSorter->Cw2][0] - 2 * mcp;
            const double timediff = tdc_ns[pSorter->Cw1][0] - tdc_ns[pSorter->Cw2][0];
            pRun->fill1d(Analysis::SortRun::h1_ionTimesumW, timesum);
            pRun->fill2d(Analysis::SortRun::h2_ionTimesumDiffW, timediff, timesum);
          }
        }
      }





      // electron
      if (eSortWrapper.sorter != nullptr) {
        auto &pSorter = eSortWrapper.sorter;
        const auto cmd = eSortWrapper.cmd;
        double &offset_u = eSortWrapper.factors["offset_u"],
            &offset_v = eSortWrapper.factors["offset_v"],
            &offset_w = eSortWrapper.factors["offset_w"],
            &fw_offset = eSortWrapper.factors["fw_offset"],
            &offset_x = eSortWrapper.factors["offset_x"],
            &offset_y = eSortWrapper.factors["offset_y"];
        if (pSorter->Cmcp > -1) {
          for (unsigned int i = 0; i < count[pSorter->Cmcp]; ++i)
            tdc_ns[pSorter->Cmcp][i] = double(TDC[pSorter->Cmcp][i]) * TDCResolution;
        }
        for (unsigned int i = 0; i < count[pSorter->Cu1]; ++i)
          tdc_ns[pSorter->Cu1][i] = double(TDC[pSorter->Cu1][i]) * TDCResolution;
        for (unsigned int i = 0; i < count[pSorter->Cu2]; ++i)
          tdc_ns[pSorter->Cu2][i] = double(TDC[pSorter->Cu2][i]) * TDCResolution;
        for (unsigned int i = 0; i < count[pSorter->Cv1]; ++i)
          tdc_ns[pSorter->Cv1][i] = double(TDC[pSorter->Cv1][i]) * TDCResolution;
        for (unsigned int i = 0; i < count[pSorter->Cv2]; ++i)
          tdc_ns[pSorter->Cv2][i] = double(TDC[pSorter->Cv2][i]) * TDCResolution;
        if (pSorter->use_HEX) {
          for (unsigned int i = 0; i < count[pSorter->Cw1]; ++i)
            tdc_ns[pSorter->Cw1][i] = double(TDC[pSorter->Cw1][i]) * TDCResolution;
          for (unsigned int i = 0; i < count[pSorter->Cw2]; ++i)
            tdc_ns[pSorter->Cw2][i] = double(TDC[pSorter->Cw2][i]) * TDCResolution;
        }
        if (pSorter->use_HEX) {
          // shift the time sums to zero:
          pSorter->shift_sums(+1, offset_u, offset_v, offset_w);
          // shift layer w so that the middle lines of all layers intersect in one point:
          pSorter->shift_layer_w(+1, fw_offset);
        }
        else {
          // shift the time sums to zero:
          pSorter->shift_sums(+1, offset_u, offset_v);
        }

        // shift all signals from the anode so that the center of the detector is at x=y=0:
        pSorter->shift_position_origin(+1, offset_x, offset_y);

        // for calibration of fv, fw, w_offset and correction tables
        pSorter->feed_calibration_data(true, fw_offset);

        if (pSorter->scalefactors_calibrator && cmd >= 2) {
          if (pSorter->scalefactors_calibrator->map_is_full_enough()) {
            std::cout << "elecSorter: map is full enough." << std::endl;
            theLoopIsOn = false;
            break;
          }
        }
        if (pSorter->use_HEX)
          pRun->fill2d(Analysis::SortRun::h2_elecXYDev,
                       pSorter->scalefactors_calibrator->binx
                           - pSorter->scalefactors_calibrator->detector_map_size / 2.0,
                       pSorter->scalefactors_calibrator->biny
                           - pSorter->scalefactors_calibrator->detector_map_size / 2.0,
                       pSorter->scalefactors_calibrator->detector_map_devi_fill);

        if (count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0) {
          if (count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0) {
            double u = pSorter->fu * (tdc_ns[pSorter->Cu1][0] - tdc_ns[pSorter->Cu2][0]);
            double v = pSorter->fv * (tdc_ns[pSorter->Cv1][0] - tdc_ns[pSorter->Cv2][0]);
            double y = (u - 2. * v) * 0.577350269; // 0.557 = 1/sqrt(3)
            pRun->fill2d(Analysis::SortRun::h2_elecXYRaw, u, y);
          }
        }
        if (count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0) {
          pRun->fill1d(Analysis::SortRun::h1_elecU, tdc_ns[pSorter->Cu1][0] - tdc_ns[pSorter->Cu2][0]);
          double mcp = 0.;
          if (pSorter->use_MCP) {
            if (count[pSorter->Cmcp] > 0) mcp = tdc_ns[pSorter->Cmcp][0]; else mcp = -1.e100;
          }
          const double timesum = tdc_ns[pSorter->Cu1][0] + tdc_ns[pSorter->Cu2][0] - 2 * mcp;
          const double timediff = tdc_ns[pSorter->Cu1][0] - tdc_ns[pSorter->Cu2][0];
          pRun->fill1d(Analysis::SortRun::h1_elecTimesumU, timesum);
          pRun->fill2d(Analysis::SortRun::h2_elecTimesumDiffU, timediff, timesum);
        }
        if (count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0) {
          pRun->fill1d(Analysis::SortRun::h1_elecV, tdc_ns[pSorter->Cv1][0] - tdc_ns[pSorter->Cv2][0]);
          double mcp = 0.;
          if (pSorter->use_MCP) {
            if (count[pSorter->Cmcp] > 0) mcp = tdc_ns[pSorter->Cmcp][0]; else mcp = -1.e100;
          }
          const double timesum = tdc_ns[pSorter->Cv1][0] + tdc_ns[pSorter->Cv2][0] - 2 * mcp;
          const double timediff = tdc_ns[pSorter->Cv1][0] - tdc_ns[pSorter->Cv2][0];
          pRun->fill1d(Analysis::SortRun::h1_elecTimesumV, timesum);
          pRun->fill2d(Analysis::SortRun::h2_elecTimesumDiffV, timediff, timesum);
        }
        if (pSorter->use_HEX) {
          if (count[pSorter->Cw1] > 0 && count[pSorter->Cw2] > 0) {
            pRun->fill1d(Analysis::SortRun::h1_elecW, tdc_ns[pSorter->Cw1][0] - tdc_ns[pSorter->Cw2][0]);
            double mcp = 0.;
            if (pSorter->use_MCP) {
              if (count[pSorter->Cmcp] > 0) mcp = tdc_ns[pSorter->Cmcp][0]; else mcp = -1.e100;
            }
            const double timesum = tdc_ns[pSorter->Cw1][0] + tdc_ns[pSorter->Cw2][0] - 2 * mcp;
            const double timediff = tdc_ns[pSorter->Cw1][0] - tdc_ns[pSorter->Cw2][0];
            pRun->fill1d(Analysis::SortRun::h1_elecTimesumW, timesum);
            pRun->fill2d(Analysis::SortRun::h2_elecTimesumDiffW, timediff, timesum);
          }
        }
      }

      // Fill the tree and hists
      int number_of_ions = 0;
      int number_of_electrons = 0;
      if (iSortWrapper.sorter != nullptr) {
        auto pSorter = iSortWrapper.sorter;
        auto cmd = iSortWrapper.cmd;
        if (cmd == Analysis::sort) {  // sort and write new file
          // sort/reconstruct the detector signals and apply the sum- and NL-correction.
          number_of_ions = pSorter->sort();
          // "number_of_ions" is the number of reconstructed number of particles
        }
        else {
          number_of_ions = pSorter->run_without_sorting();
        }
        for (int i = 0; i < number_of_ions; i++) {
          pRun->fill2d(Analysis::SortRun::h2_ionXY,
                       pSorter->output_hit_array[i]->x,
                       pSorter->output_hit_array[i]->y);
        }
      }
      if (eSortWrapper.sorter != nullptr) {
        auto pSorter = eSortWrapper.sorter;
        auto cmd = eSortWrapper.cmd;
        if (cmd == 1) {  // sort and write new file
          // sort/reconstruct the detector signals and apply the sum- and NL-correction.
          number_of_electrons = pSorter->sort();
          // "number_of_electrons" is the number of reconstructed number of particles
        }
        else {
          number_of_electrons = pSorter->run_without_sorting();
        }
        for (int i = 0; i < number_of_electrons; i++) {
          pRun->fill2d(Analysis::SortRun::h2_elecXY,
                       pSorter->output_hit_array[i]->x,
                       pSorter->output_hit_array[i]->y);
        }
      }
      double eMarker = 0;
      if (eSortWrapper.sorter != nullptr) {
        auto eSorter = eSortWrapper.sorter;
        double mcp = 0.;
        if (eSorter->use_MCP) {
          if (count[eSorter->Cmcp] > 0) mcp = tdc_ns[eSorter->Cmcp][0]; else mcp = -1.e100;
        }
        eMarker = mcp - TDC[bunchCh][0] * TDCResolution;
      }
      if (!(bunchMaskRm.isIn(eMarker))) {
        auto iSorter = iSortWrapper.sorter;
        auto eSorter = eSortWrapper.sorter;
        pRun->fill1d(Analysis::SortRun::h1_eMarker, eMarker);
        if(number_of_ions >= 1) {
          double &x = iSorter->output_hit_array[0]->x;
          double &y = iSorter->output_hit_array[0]->y;
          double &t = iSorter->output_hit_array[0]->time;
          pRun->fill2d(Analysis::SortRun::h2_ion1hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_ion1hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_ion1hitXY, x, y);
        }
        if(number_of_ions >= 2) {
          double &x = iSorter->output_hit_array[1]->x;
          double &y = iSorter->output_hit_array[1]->y;
          double &t = iSorter->output_hit_array[1]->time;
          double &tt = iSorter->output_hit_array[0]->time;
          pRun->fill2d(Analysis::SortRun::h2_ion2hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_ion2hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_ion2hitXY, x, y);
          pRun->fill2d(Analysis::SortRun::h2_ion1hit2hitPIPICO, tt, t);
        }
        if(number_of_ions >= 3) {
          double &x = iSorter->output_hit_array[2]->x;
          double &y = iSorter->output_hit_array[2]->y;
          double &t = iSorter->output_hit_array[2]->time;
          double &tt = iSorter->output_hit_array[1]->time;
          pRun->fill2d(Analysis::SortRun::h2_ion3hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_ion3hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_ion3hitXY, x, y);
          pRun->fill2d(Analysis::SortRun::h2_ion2hit3hitPIPICO, tt, t);
        }
        if(number_of_ions >= 4) {
          double &x = iSorter->output_hit_array[3]->x;
          double &y = iSorter->output_hit_array[3]->y;
          double &t = iSorter->output_hit_array[3]->time;
          double &tt = iSorter->output_hit_array[2]->time;
          pRun->fill2d(Analysis::SortRun::h2_ion4hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_ion4hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_ion4hitXY, x, y);
          pRun->fill2d(Analysis::SortRun::h2_ion3hit4hitPIPICO, tt, t);
        }
        if(number_of_electrons >= 1) {
          double &x = eSorter->output_hit_array[0]->x;
          double &y = eSorter->output_hit_array[0]->y;
          double &t = eSorter->output_hit_array[0]->time;
          pRun->fill2d(Analysis::SortRun::h2_elec1hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_elec1hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_elec1hitXY, x, y);
        }
        if(number_of_electrons >= 2) {
          double &x = eSorter->output_hit_array[1]->x;
          double &y = eSorter->output_hit_array[1]->y;
          double &t = eSorter->output_hit_array[1]->time;
          double &tt = eSorter->output_hit_array[0]->time;
          pRun->fill2d(Analysis::SortRun::h2_elec2hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_elec2hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_elec2hitXY, x, y);
          pRun->fill2d(Analysis::SortRun::h2_elec1hit2hitPEPECO, tt, t);
        }
        if(number_of_electrons >= 3) {
          double &x = eSorter->output_hit_array[2]->x;
          double &y = eSorter->output_hit_array[2]->y;
          double &t = eSorter->output_hit_array[2]->time;
          double &tt = eSorter->output_hit_array[1]->time;
          pRun->fill2d(Analysis::SortRun::h2_elec3hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_elec3hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_elec3hitXY, x, y);
          pRun->fill2d(Analysis::SortRun::h2_elec2hit3hitPEPECO, tt, t);
        }
        if(number_of_electrons >= 4) {
          double &x = eSorter->output_hit_array[3]->x;
          double &y = eSorter->output_hit_array[3]->y;
          double &t = eSorter->output_hit_array[3]->time;
          double &tt = eSorter->output_hit_array[2]->time;
          pRun->fill2d(Analysis::SortRun::h2_elec4hitXFish, t, x);
          pRun->fill2d(Analysis::SortRun::h2_elec4hitYFish, t, y);
          pRun->fill2d(Analysis::SortRun::h2_elec4hitXY, x, y);
          pRun->fill2d(Analysis::SortRun::h2_elec3hit4hitPEPECO, tt, t);
        }
        Analysis::SortRun::DataSet *pIons, *pElecs;
        pIons = new Analysis::SortRun::DataSet[number_of_ions];
        pElecs = new Analysis::SortRun::DataSet[number_of_electrons];
        for (int i = 0; i < number_of_ions; i++) {
          pIons[i].x = iSorter->output_hit_array[i]->x;
          pIons[i].y = iSorter->output_hit_array[i]->y;
          pIons[i].t = iSorter->output_hit_array[i]->time;
          pIons[i].flag = iSorter->output_hit_array[i]->method;
        }
        for (int i = 0; i < number_of_electrons; i++) {
          pElecs[i].x = eSorter->output_hit_array[i]->x;
          pElecs[i].y = eSorter->output_hit_array[i]->y;
          pElecs[i].t = eSorter->output_hit_array[i]->time;
          pElecs[i].flag = eSorter->output_hit_array[i]->method;
        }
        pRun->fillTree(number_of_ions, pIons, number_of_electrons, pElecs);
        if (pIons) {
          delete[] pIons;
          pIons = nullptr;
        }
        if (pElecs) {
          delete[] pElecs;
		  pElecs = nullptr;
        }
      }





      // Write to output file
      FILE *outfile = nullptr;
      if (outfile) {
        // ion
        if (iSortWrapper.sorter != nullptr) {
          auto &pSorter = iSortWrapper.sorter;
          const auto cmd = iSortWrapper.cmd;
          double &offset_u = iSortWrapper.factors["offset_u"],
              &offset_v = iSortWrapper.factors["offset_v"],
              &offset_w = iSortWrapper.factors["offset_w"],
              &fw_offset = iSortWrapper.factors["fw_offset"],
              &offset_x = iSortWrapper.factors["offset_x"],
              &offset_y = iSortWrapper.factors["offset_y"];
          // the following steps are necessary to make the new output look as the old one
          // (in respect to time offsets)

          // shift the detector signals back (note the -1 instead of the +1)
          if (pSorter->use_HEX)
            pSorter->shift_sums(-1, offset_u, offset_v, offset_w);
          if (!pSorter->use_HEX) pSorter->shift_sums(-1, offset_u, offset_v);
          pSorter->shift_layer_w(-1, fw_offset);
          pSorter->shift_position_origin(-1, offset_x, offset_y);

          // convert the times from nanoseconds back to raw channels:
          if (pSorter->Cmcp > -1) {
            for (unsigned int i = 0; i < count[pSorter->Cmcp]; ++i)
              TDC[pSorter->Cmcp][i] = int(tdc_ns[pSorter->Cmcp][i] / TDCResolution);
          }
          for (unsigned int i = 0; i < count[pSorter->Cu1]; ++i)
            TDC[pSorter->Cu1][i] = int(tdc_ns[pSorter->Cu1][i] / TDCResolution);
          for (unsigned int i = 0; i < count[pSorter->Cu2]; ++i)
            TDC[pSorter->Cu2][i] = int(tdc_ns[pSorter->Cu2][i] / TDCResolution);
          for (unsigned int i = 0; i < count[pSorter->Cv1]; ++i)
            TDC[pSorter->Cv1][i] = int(tdc_ns[pSorter->Cv1][i] / TDCResolution);
          for (unsigned int i = 0; i < count[pSorter->Cv2]; ++i)
            TDC[pSorter->Cv2][i] = int(tdc_ns[pSorter->Cv2][i] / TDCResolution);
          if (pSorter->use_HEX) {
            for (unsigned int i = 0; i < count[pSorter->Cw1]; ++i)
              TDC[pSorter->Cw1][i] = int(tdc_ns[pSorter->Cw1][i] / TDCResolution);
            for (unsigned int i = 0; i < count[pSorter->Cw2]; ++i)
              TDC[pSorter->Cw2][i] = int(tdc_ns[pSorter->Cw2][i] / TDCResolution);
          }
        }





        // electron
        if (eSortWrapper.sorter != nullptr) {
          auto &pSorter = eSortWrapper.sorter;
          const auto cmd = eSortWrapper.cmd;
          double &offset_u = eSortWrapper.factors["offset_u"],
              &offset_v = eSortWrapper.factors["offset_v"],
              &offset_w = eSortWrapper.factors["offset_w"],
              &fw_offset = eSortWrapper.factors["fw_offset"],
              &offset_x = eSortWrapper.factors["offset_x"],
              &offset_y = eSortWrapper.factors["offset_y"];
          // shift the detector signals back (note the -1 instead of the +1)
          if (pSorter->use_HEX)
            pSorter->shift_sums(-1, offset_u, offset_v, offset_w);
          if (!pSorter->use_HEX) pSorter->shift_sums(-1, offset_u, offset_v);
          pSorter->shift_layer_w(-1, fw_offset);
          pSorter->shift_position_origin(-1, offset_x, offset_y);

          // convert the times from nanoseconds back to raw channels:
          if (pSorter->Cmcp > -1) {
            for (unsigned int i = 0; i < count[pSorter->Cmcp]; ++i)
              TDC[pSorter->Cmcp][i] = int(tdc_ns[pSorter->Cmcp][i] / TDCResolution);
          }
          for (unsigned int i = 0; i < count[pSorter->Cu1]; ++i)
            TDC[pSorter->Cu1][i] = int(tdc_ns[pSorter->Cu1][i] / TDCResolution);
          for (unsigned int i = 0; i < count[pSorter->Cu2]; ++i)
            TDC[pSorter->Cu2][i] = int(tdc_ns[pSorter->Cu2][i] / TDCResolution);
          for (unsigned int i = 0; i < count[pSorter->Cv1]; ++i)
            TDC[pSorter->Cv1][i] = int(tdc_ns[pSorter->Cv1][i] / TDCResolution);
          for (unsigned int i = 0; i < count[pSorter->Cv2]; ++i)
            TDC[pSorter->Cv2][i] = int(tdc_ns[pSorter->Cv2][i] / TDCResolution);
          if (pSorter->use_HEX) {
            for (unsigned int i = 0; i < count[pSorter->Cw1]; ++i)
              TDC[pSorter->Cw1][i] = int(tdc_ns[pSorter->Cw1][i] / TDCResolution);
            for (unsigned int i = 0; i < count[pSorter->Cw2]; ++i)
              TDC[pSorter->Cw2][i] = int(tdc_ns[pSorter->Cw2][i] / TDCResolution);
          }
        }





        // output TDC data
        for (unsigned int i = 0; i < pLMF->number_of_channels; i++) {
          fwrite(&count[i], sizeof(int), 1, outfile);
          for (unsigned int j = 0; j < count[i]; j++) fwrite(&TDC[i][j], sizeof(int), 1, outfile);
        }
      }

    } // end of the big while loop
    printf("ok\n");

    // calib
    iSortWrapper.calibFactors();
    eSortWrapper.calibFactors();
    iSortWrapper.genClibTab();
    eSortWrapper.genClibTab();

    if (pRun != nullptr) {
      pRun->updateC1(true);
      pRun->updateC2(true);
      delete pRun;
      pRun = nullptr;
    }

    printf("deleting the LMF reader instance... ");
    if (pLMF) {
      delete pLMF;
      pLMF = nullptr;
    }
    printf("ok \n");
  }

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
  iSortWrapper.cleanup();
  eSortWrapper.cleanup();
  theRootApp.Terminate();
  std::cout << "The program is done. " << std::endl;
  return 0;
}
