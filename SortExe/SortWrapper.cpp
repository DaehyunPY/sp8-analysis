//
// Created by daehyun on 1/17/17.
//

#include "SortWrapper.h"
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
Analysis::SortWrapper::~SortWrapper() {
  pLMFSource = nullptr;
  if (pSorter != nullptr) {
    delete pSorter;
    pSorter = nullptr;
  }
  if (pChT0 != nullptr) {
    delete pChT0;
    pChT0 = nullptr;
  }
}
bool Analysis::SortWrapper::readConfig(const Analysis::JSONReader &reader, const std::string prefix) {
  std::cout << "Initialize sorter from config file... " << std::endl;
  if (!reader.hasMember(prefix)) throw std::invalid_argument("The member does not exist!");

  cmd = (SortCmd) reader.get<int>(prefix + ".cmd");
  if (cmd == -1) {
    std::cout << "ok" << std::endl;
    return false;
  }
  calibTabFilename = reader.get<const char *>(prefix + ".calibration_table");

  std::map<std::string, int> chMap;
  chMap = reader.getMap<int>(prefix + ".channel_map");
  pSorter->use_HEX = reader.getBoolAt(prefix + ".hexanode_used");
  pSorter->common_start_mode = reader.getBoolAt(prefix + ".common_start_mode");
  pSorter->Cu1 = chMap["u1"] - 1;
  pSorter->Cu2 = chMap["u2"] - 1;
  pSorter->Cv1 = chMap["v1"] - 1;
  pSorter->Cv2 = chMap["v2"] - 1;
  pSorter->Cw1 = chMap["w1"] - 1;
  pSorter->Cw2 = chMap["w2"] - 1;
  pSorter->Cmcp = chMap["MCP"] - 1;
  pSorter->use_MCP = (pSorter->Cmcp) > -1;
  if (chMap["t0"] != 0) pChT0 = new auto(chMap["t0"] -1);

  factors = reader.getMap<double>(prefix + ".factors");
  pSorter->uncorrected_time_sum_half_width_u = factors["halfwidth_u"];
  pSorter->uncorrected_time_sum_half_width_v = factors["halfwidth_v"];
  pSorter->uncorrected_time_sum_half_width_w = factors["halfwidth_w"];
  for (auto key: {"runtime_u", "runtime_v", "runtime_w"}) {
    auto found = factors.find(key);
    if (found == factors.end()) factors[key] = factors["runtime"];
  }
  pSorter->runtime_u = factors["runtime_u"];
  pSorter->runtime_v = factors["runtime_v"];
  pSorter->runtime_w = factors["runtime_w"];
  pSorter->fu = 0.5 * factors["fu"];
  pSorter->fv = 0.5 * factors["fv"];
  pSorter->fw = 0.5 * factors["fw"];

  pSorter->MCP_radius = reader.getDoubleAt(prefix + ".MCP_radius");
  pSorter->dead_time_anode = reader.getDoubleAt(prefix + ".anode_deadtime");
  pSorter->dead_time_mcp = reader.getDoubleAt(prefix + ".MCP_deadtime");
  pSorter->use_sum_correction = reader.getBoolAt(prefix + ".correct_timesum");
  pSorter->use_pos_correction = reader.getBoolAt(prefix + ".correct_position");
  return true;
}
bool Analysis::SortWrapper::init() {
    if (pSorter == nullptr) return true;
    std::cout << "init sorter... ";
    pSorter->TDC_resolution_ns = pLMFSource->TDCRes;
    pSorter->tdc_array_row_length = NUM_IONS;
    pSorter->count = (int *) pLMFSource->count;
    pSorter->tdc_pointer = &pLMFSource->TDCns[0][0];
    if (cmd >= kCalib) {
      pSorter->create_scalefactors_calibrator(
          true, // BuildStack
          pSorter->runtime_u, pSorter->runtime_v, pSorter->runtime_w, // runtime u, v, w
          0.78, // runtime inner fraction
          pSorter->fu, pSorter->fv, pSorter->fw // fu, fv, fw
      );
    }
    int error_code = pSorter->init_after_setting_parameters();
    if (error_code) {
      std::cout << "ion sorter could not be initialized" << std::endl;
      char error_text[512];
      pSorter->get_error_text(error_code, 512, error_text);
      printf("Error %i: %s\n", error_code, error_text);
      return false;
    }
    std::cout << "ok" << std::endl;
    return true;
}
Analysis::SortWrapper::SortWrapper(Analysis::LMFWrapper *p) {
  if (p==nullptr) throw std::invalid_argument("The LMFWrapper is invalid!");
  pLMFSource = p;
  pSorter = new sort_class();
  cmd = kNoDetector;
  pChT0 = nullptr;
  t0 = 0;
  factors.clear();
  calibTabFilename = "";
  numHits = 0;
}
bool Analysis::SortWrapper::isNull() const {
  return pSorter == nullptr;
}
bool Analysis::SortWrapper::convertTDC() {
  if (pSorter == nullptr) return true;
  const double &offset_u = factors["offset_u"];
  const double &offset_v = factors["offset_v"];
  const double &offset_w = factors["offset_w"];
  const double &fw_offset = factors["fw_offset"];
  const double &offset_x = factors["offset_x"];
  const double &offset_y = factors["offset_y"];
  const double &Res = pLMFSource->TDCRes;
  const auto &count = pLMFSource->count;
  const auto &TDC = pLMFSource->TDC;
  auto &tdc_ns = pLMFSource->TDCns;
  if (pSorter->Cmcp > -1) {
    for (unsigned int i = 0; i < count[pSorter->Cmcp]; ++i)
      tdc_ns[pSorter->Cmcp][i] = double(TDC[pSorter->Cmcp][i]) * Res;
  }
  for (unsigned int i = 0; i < count[pSorter->Cu1]; ++i)
    tdc_ns[pSorter->Cu1][i] = double(TDC[pSorter->Cu1][i]) * Res;
  for (unsigned int i = 0; i < count[pSorter->Cu2]; ++i)
    tdc_ns[pSorter->Cu2][i] = double(TDC[pSorter->Cu2][i]) * Res;
  for (unsigned int i = 0; i < count[pSorter->Cv1]; ++i)
    tdc_ns[pSorter->Cv1][i] = double(TDC[pSorter->Cv1][i]) * Res;
  for (unsigned int i = 0; i < count[pSorter->Cv2]; ++i)
    tdc_ns[pSorter->Cv2][i] = double(TDC[pSorter->Cv2][i]) * Res;
  if (pSorter->use_HEX) {
    for (unsigned int i = 0; i < count[pSorter->Cw1]; ++i)
      tdc_ns[pSorter->Cw1][i] = double(TDC[pSorter->Cw1][i]) * Res;
    for (unsigned int i = 0; i < count[pSorter->Cw2]; ++i)
      tdc_ns[pSorter->Cw2][i] = double(TDC[pSorter->Cw2][i]) * Res;
  }
  if (pChT0 != nullptr) t0 = double(TDC[*pChT0][0]) * Res;
  if (pSorter->use_HEX) {
    // shift the time sums to zero:
    pSorter->shift_sums(+1, offset_u, offset_v, offset_w);
    // shift layer w so that the middle lines of all layers intersect in one point:
    pSorter->shift_layer_w(+1, fw_offset);
  } else {
    // shift the time sums to zero:
    pSorter->shift_sums(+1, offset_u, offset_v);
  }
  // shift all signals from the anode so that the center of the detector is at x=y=0:
  pSorter->shift_position_origin(+1, offset_x, offset_y);
  // for calibration of fv, fw, w_offset and correction tables
  pSorter->feed_calibration_data(true, fw_offset);
  return true;
}
bool Analysis::SortWrapper::isFull() const {
  if (cmd >= kCalib && pSorter->scalefactors_calibrator != nullptr)
    if (pSorter->scalefactors_calibrator->map_is_full_enough())
      return true;
  return false;
}
bool Analysis::SortWrapper::readCalibTab() {
  if (pSorter == nullptr) return true;
  if (pSorter->use_sum_correction || pSorter->use_pos_correction) {
    std::cout << "Reading calibration table: " << calibTabFilename << "... ";
    const bool b = read_calibration_tables(calibTabFilename.c_str(), pSorter);
    std::cout << "ok" << std::endl;
    return b;
  }
  return true;
}
bool Analysis::SortWrapper::genClibTab() const {
    if (pSorter == nullptr) return true;
    if (cmd == kGenCalibTab) {
      std::cout << "Generating calibration table: " << calibTabFilename << "... ";
      const bool result = create_calibration_tables(calibTabFilename.c_str(), pSorter);
      std::cout << "ok" << std::endl;
      return result;
    }
    return false;
}
bool Analysis::SortWrapper::calibFactors() const {
    if (pSorter == nullptr) return true;
    if (cmd == kCalib) {
      std::cout << "Calibrating... ";
      const bool b = pSorter->do_calibration();
      std::cout << "ok" << std::endl;
      if (pSorter->scalefactors_calibrator) {
        printf("Good scalefactors are:\nf_U = %lg\nf_V = %lg\nf_W = %lg\nOffset on layer W = %lg\n",
               2. * pSorter->fu,
               2. * pSorter->scalefactors_calibrator->best_fv,
               2. * pSorter->scalefactors_calibrator->best_fw,
               pSorter->scalefactors_calibrator->best_w_offset);
      }
      return b;
    }
    return false;
}
Analysis::SortWrapper::SortCmd Analysis::SortWrapper::getCmd() const {
  return cmd;
}
std::shared_ptr<double> Analysis::SortWrapper::getMCP() const {
  if (pSorter==nullptr) return nullptr;
  if (!pSorter->use_MCP) return std::make_shared<double>(0);
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (count[pSorter->Cmcp] > 0) return std::make_shared<double>(TDCns[pSorter->Cmcp][0]);
  else return nullptr;
}
std::shared_ptr<double> Analysis::SortWrapper::getXDev() const {
  if (pSorter==nullptr) return nullptr;
  if (!pSorter->use_HEX) return nullptr;
  return std::make_shared<double>(pSorter->scalefactors_calibrator->binx - pSorter->scalefactors_calibrator->detector_map_size / 2.0);
}
std::shared_ptr<double> Analysis::SortWrapper::getYDev() const {
  if (pSorter==nullptr) return nullptr;
  if (!pSorter->use_HEX) return nullptr;
  return std::make_shared<double>(pSorter->scalefactors_calibrator->biny - pSorter->scalefactors_calibrator->detector_map_size / 2.0);
}
std::shared_ptr<double> Analysis::SortWrapper::getWeightDev() const {
  if (pSorter==nullptr) return nullptr;
  if (!pSorter->use_HEX) return nullptr;
  return std::make_shared<double>(pSorter->scalefactors_calibrator->detector_map_devi_fill);
}
std::shared_ptr<double> Analysis::SortWrapper::getXRaw() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0)) return nullptr;
  if (!(count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0)) return nullptr;
  double u_raw = pSorter->fu * (TDCns[pSorter->Cu1][0] - TDCns[pSorter->Cu2][0]);
  return std::make_shared<double>(u_raw);
}
std::shared_ptr<double> Analysis::SortWrapper::getYRaw() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0)) return nullptr;
  if (!(count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0)) return nullptr;
  double u_raw = pSorter->fu * (TDCns[pSorter->Cu1][0] - TDCns[pSorter->Cu2][0]);
  double v_raw = pSorter->fv * (TDCns[pSorter->Cv1][0] - TDCns[pSorter->Cv2][0]);
  double y_raw = (u_raw - 2. * v_raw) / std::sqrt(3.0);
  return std::make_shared<double>(y_raw);
}
std::shared_ptr<double> Analysis::SortWrapper::getUTimesum() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0)) return nullptr;
  const auto mcp = getMCP();
  if (mcp == nullptr) return nullptr;
  return std::make_shared<double>(TDCns[pSorter->Cu1][0] + TDCns[pSorter->Cu2][0] - 2 * *mcp);
}
std::shared_ptr<double> Analysis::SortWrapper::getUTimediff() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cu1] > 0 && count[pSorter->Cu2] > 0)) return nullptr;
  return std::make_shared<double>(TDCns[pSorter->Cu1][0] - TDCns[pSorter->Cu2][0]);
}
std::shared_ptr<double> Analysis::SortWrapper::getVTimesum() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0)) return nullptr;
  const auto mcp = getMCP();
  if (mcp == nullptr) return nullptr;
  return std::make_shared<double>(TDCns[pSorter->Cv1][0] + TDCns[pSorter->Cv2][0] - 2 * *mcp);
}
std::shared_ptr<double> Analysis::SortWrapper::getVTimediff() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cv1] > 0 && count[pSorter->Cv2] > 0)) return nullptr;
  return std::make_shared<double>(TDCns[pSorter->Cv1][0] - TDCns[pSorter->Cv2][0]);
}
std::shared_ptr<double> Analysis::SortWrapper::getWTimesum() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cw1] > 0 && count[pSorter->Cw2] > 0)) return nullptr;
  const auto mcp = getMCP();
  if (mcp == nullptr) return nullptr;
  return std::make_shared<double>(TDCns[pSorter->Cw1][0] + TDCns[pSorter->Cw2][0] - 2 * *mcp);
}
std::shared_ptr<double> Analysis::SortWrapper::getWTimediff() const {
  const auto &count = pLMFSource->count;
  const auto &TDCns = pLMFSource->TDCns;
  if (!(count[pSorter->Cw1] > 0 && count[pSorter->Cw2] > 0)) return nullptr;
  return std::make_shared<double>(TDCns[pSorter->Cw1][0] - TDCns[pSorter->Cw2][0]);
}
bool Analysis::SortWrapper::sort() {
  if (pSorter == nullptr) return true;
  if (cmd == kSort) { // sort and write new file
    // sort/reconstruct the detector signals and apply the sum- and NL-correction.
    const int n = pSorter->sort();
    numHits = n;
    return true;
    // "number_of_ions" is the number of reconstructed number of particles
  } else {
    const int n = pSorter->run_without_sorting();
    numHits = n;
    return true;
  }
}
int Analysis::SortWrapper::getNumHits() const {
  return numHits;
}
hit_class **Analysis::SortWrapper::getOutputArr() const {
  return pSorter->output_hit_array;
}
std::shared_ptr<double> Analysis::SortWrapper::getNthX(const int i) const {
  if (i<0) return nullptr;
  if (i>=numHits) return nullptr;
  return std::make_shared<double>(pSorter->output_hit_array[i]->x);
}
std::shared_ptr<double> Analysis::SortWrapper::getNthY(const int i) const {
  if (i<0) return nullptr;
  if (i>=numHits) return nullptr;
  return std::make_shared<double>(pSorter->output_hit_array[i]->y);
}
std::shared_ptr<double> Analysis::SortWrapper::getNthT(const int i) const {
  if (i<0) return nullptr;
  if (i>=numHits) return nullptr;
  if (pChT0 == nullptr) {
    return std::make_shared<double>(pSorter->output_hit_array[i]->time);
  } else {
    return std::make_shared<double>(pSorter->output_hit_array[i]->time -t0);
  }
}
std::shared_ptr<int> Analysis::SortWrapper::getNthMethod(const int i) const {
  if (i<0) return nullptr;
  if (i>=numHits) return nullptr;
  return std::make_shared<int>(pSorter->output_hit_array[i]->method);
}
bool Analysis::LMFWrapper::readConfig(const Analysis::JSONReader &reader) {
    auto pStr = reader.getOpt<const char *>("LMF_files");
    if (pStr) {
      filenames.push_back(std::string(*pStr));
      return true;
    } else {
      auto pArr = reader.getOptArr<const char *>("LMF_files");
      if (!pArr) return false;
      for (auto str: *pArr) filenames.push_back(std::string(str));
      return true;
    }
}
bool Analysis::LMFWrapper::readFile(const int i) {
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
bool Analysis::LMFWrapper::readNextEvent() {
  memset(count, 0, pLMF->number_of_channels * sizeof(int));
  if (!pLMF->ReadNextEvent()) return false;
  pLMF->GetNumberOfHitsArray(count);
  pLMF->GetTDCDataArray((int *) TDC);
  timestamp = pLMF->GetDoubleTimeStamp(); // absolute timestamp in seconds
  return true;
}
void Analysis::LMFWrapper::cleanup() {
  if (pLMF) {
    delete pLMF;
    pLMF = nullptr;
  }
}
