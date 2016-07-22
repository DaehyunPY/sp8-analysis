#ifdef WIN32
#pragma warning(disable : 4005)
#pragma warning(disable : 4996)
#endif

#ifdef WIN64
#pragma warning(disable : 4005)
#pragma warning(disable : 4996)
#endif

#ifndef LINUX
#ifndef WIN32
#ifndef WIN64
#define LINUX
#endif
#endif
#endif

#include <TSystem.h>
#include <TApplication.h>

#include "../resort/resort64c.h"
#include "LMF_IO.h"
#include "SortRun.h"

#define NUM_IONS 200
#define NUM_CHANNELS 80

sort_class *ionSorter;
std::string ionCalibTabFilename;
double ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w;
double ion_w_offset, ion_pos_offset_x, ion_pos_offset_y;
int ion_command;

sort_class *elecSorter;
std::string elecClibTabFilename;
double elec_offset_sum_u, elec_offset_sum_v, elec_offset_sum_w;
double elec_w_offset;
double elec_pos_offset_x, elec_pos_offset_y;
int elec_command;

LMF_IO *LMF;

#ifndef LINUX
#include "conio.h"
__int32 my_kbhit() {
    if (!_kbhit()) return 0;
    __int32 c = _getch();
    while (_kbhit()) c = _getch(); // empty keyboard buffer
    return c;
}
#else

#include <termios.h>
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
#endif

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
  return atoi(a);
}

double read_double(FILE *ffile) {
  char a[1024];
  readline_from_config_file(ffile, a, 1024);
  return double(atof(a));
}

bool read_config_file(const Analysis::JSONReader &reader, const std::string prefix, sort_class *&sorter, int &command, double &offset_sum_u, double &offset_sum_v,
                      double &offset_sum_w, double &w_offset, double &pos_offset_x, double &pos_offset_y) {
  // read the config file:
	std::cout << "Reading sorter config from the config file... ";
	if (!reader.hasMember(prefix)) {
		std::cout << "Member " << prefix << " was not found." << std::endl;
		std::cout << "ok" << std::endl;
		return false;
	}
  command = reader.getIntAt(prefix + ".mode");
  if (command == -1) {
	std::cout << "ok" << std::endl;
    return false;
  }

  sorter->use_HEX = reader.getBoolAt(prefix + ".is_hexanode_used");
  sorter->common_start_mode = reader.getBoolAt(prefix + ".common_start_mode");
  sorter->Cu1 = reader.getIntAt(prefix + ".TDC_channel.u1") - 1;
  sorter->Cu2 = reader.getIntAt(prefix + ".TDC_channel.u2") - 1;
  sorter->Cv1 = reader.getIntAt(prefix + ".TDC_channel.v1") - 1;
  sorter->Cv2 = reader.getIntAt(prefix + ".TDC_channel.v2") - 1;
  sorter->Cw1 = reader.getIntAt(prefix + ".TDC_channel.w1") - 1;
  sorter->Cw2 = reader.getIntAt(prefix + ".TDC_channel.w2") - 1;
  sorter->Cmcp = reader.getIntAt(prefix + ".TDC_channel.MCP") - 1;
  sorter->use_MCP = (sorter->Cmcp > -1) ? true : false;

  offset_sum_u = reader.getDoubleAt(prefix + ".offset_setup.shift_of_timesum_u");
  offset_sum_v = reader.getDoubleAt(prefix + ".offset_setup.shift_of_timesum_v");
  offset_sum_w = reader.getDoubleAt(prefix + ".offset_setup.shift_of_timesum_w");
  pos_offset_x = reader.getDoubleAt(prefix + ".offset_setup.shift_of_position_x");
  pos_offset_y = reader.getDoubleAt(prefix + ".offset_setup.shift_of_position_y");
  sorter->uncorrected_time_sum_half_width_u = reader.getDoubleAt(prefix + ".offset_setup.halfwidth_of_timesum_u");
  sorter->uncorrected_time_sum_half_width_v = reader.getDoubleAt(prefix + ".offset_setup.halfwidth_of_timesum_v");
  sorter->uncorrected_time_sum_half_width_w = reader.getDoubleAt(prefix + ".offset_setup.halfwidth_of_timesum_w");

  sorter->fu = 0.5 * reader.getDoubleAt(prefix + ".scalefactors.u");
  sorter->fv = 0.5 * reader.getDoubleAt(prefix + ".scalefactors.v");
  sorter->fw = 0.5 * reader.getDoubleAt(prefix + ".scalefactors.w");
  w_offset = reader.getDoubleAt(prefix + ".scalefactors.offset_w");

  if(reader.hasMember(prefix + ".runtime.u")) {
	  sorter->runtime_u = reader.getDoubleAt(prefix + ".runtime.u");
	  sorter->runtime_v = reader.getDoubleAt(prefix + ".runtime.v");
	  sorter->runtime_w = reader.getDoubleAt(prefix + ".runtime.w");
  } else {
	  double tmp;
	  tmp = reader.getDoubleAt(prefix + ".runtime");
	  sorter->runtime_u = tmp;
	  sorter->runtime_v = tmp;
	  sorter->runtime_w = tmp;
  }

  sorter->MCP_radius = reader.getDoubleAt(prefix + ".radius_of_active_MCP_area");
  sorter->dead_time_anode = reader.getDoubleAt(prefix + ".deadtime_of_signals_from_the_anode");
  sorter->dead_time_mcp = reader.getDoubleAt(prefix + ".deadtime_of_signals_from_time_MCP");
  sorter->use_sum_correction = reader.getBoolAt(prefix + ".use_position_depended_correction_of_timesums");
  sorter->use_pos_correction = reader.getBoolAt(prefix + ".use_position_depended_NL_correction_of_position");

  // end of reading the config file:
  std::cout << "ok" << std::endl;
  return true;
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

void cleanUp() {
  while (my_kbhit()) {
  } // empty keyboard buffer
  printf("deleting the ion sorter... ");
  if (ionSorter) {
    delete ionSorter;
    ionSorter = 0;
  }
  printf("ok \n");
  printf("deleting the elec sorter... ");
  if (elecSorter) {
    delete elecSorter;
    elecSorter = 0;
  }
  printf("ok \n");
  printf("deleting the LMF reader instance... ");
  if (LMF) {
    delete LMF;
    LMF = 0;
  }
  printf("ok \n");
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
	pReader = new Analysis::JSONReader(argv[1]);
	if(!pReader) {
		std::cout << "fail" << std::endl;
		return 0;
	}
	std::cout << "ok" << std::endl;

	// Setup Run
	Analysis::SortRun *pRun;
	pRun = new Analysis::SortRun(*pReader);
	bool fill_histograms = true;

	LMF = new LMF_IO(NUM_CHANNELS, NUM_IONS);

	double tdcresolution = 0.025; // 25ps tdc bin size
	int TDC[NUM_CHANNELS][NUM_IONS];
	double tdc_ns[NUM_CHANNELS][NUM_IONS];
	unsigned int count[NUM_CHANNELS];
	ion_command = -1;
	elec_command = -1;
	// 0 = only convert to new file format
	// 1 = sort and write new file
	// 2 = calibrate fv, fw, w_offset
	// 3 = create calibration table files

	// create the sorter:
	printf("creating the ion sorter... ");
	ionSorter = new sort_class();
	printf("ok\n");

	printf("creating the electron sorter... ");
	elecSorter = new sort_class();
	printf("ok\n");

	{ // read sorter config
		bool readSuccessfully;
		readSuccessfully = read_config_file(*pReader, "ion_sorter", ionSorter,
			ion_command, ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w, ion_w_offset, ion_pos_offset_x, ion_pos_offset_y);
		if (!readSuccessfully) {
			delete ionSorter;
			ionSorter = nullptr;
		}
		ionCalibTabFilename = pReader->getStringAt("ion_calibration_table");
		if (ionSorter) {
			if (ionSorter->use_sum_correction || ionSorter->use_pos_correction)
				read_calibration_tables(ionCalibTabFilename.c_str(), ionSorter);
		}
		readSuccessfully = read_config_file(*pReader, "electron_sorter", elecSorter,
			elec_command, elec_offset_sum_u, elec_offset_sum_v, elec_offset_sum_w, elec_w_offset, elec_pos_offset_x, elec_pos_offset_y);
		if (!readSuccessfully) {
			delete elecSorter;
			elecSorter = 0;
		}
		elecClibTabFilename = pReader->getStringAt("electron_calibration_table");
		if (elecSorter) {
			if (elecSorter->use_sum_correction || elecSorter->use_pos_correction)
				read_calibration_tables(elecClibTabFilename.c_str(), elecSorter);
		}
	}

	// Close the JSON reader 
	std::cout << "Closing the config file... ";
	if (pReader) delete pReader;
	std::cout << "ok" << std::endl;

  if (ion_command > 1 && elec_command > 1) {
    printf("Error: Do not calibrate 2 detectors simultaneously.\n");
    return 0;
  }

  if (ion_command == -1 && elec_command == -1) {
    printf("no config file was read. Nothing to do.\n");
    return 0;
  }


  // initialization of the sorters:
  if (ionSorter) {
    printf("init ion sorter... ");
    ionSorter->TDC_resolution_ns = tdcresolution;
    ionSorter->tdc_array_row_length = NUM_IONS;
    ionSorter->count = (int *) count;
    ionSorter->tdc_pointer = &tdc_ns[0][0];
    if (ion_command >= 2) {
      ionSorter->create_scalefactors_calibrator(true, ionSorter->runtime_u, ionSorter->runtime_v,
                                                 ionSorter->runtime_w, 0.78, ionSorter->fu, ionSorter->fv,
                                                 ionSorter->fw);
    }
    int error_code = ionSorter->init_after_setting_parameters();
    if (error_code) {
      printf("ion sorter could not be initialized\n");
      char error_text[512];
      ionSorter->get_error_text(error_code, 512, error_text);
      printf("Error %i: %s\n", error_code, error_text);
      return 0;
    }
    printf("ok\n");
  }

  if (elecSorter) {
    printf("init electron sorter... ");
    elecSorter->TDC_resolution_ns = tdcresolution;
    elecSorter->tdc_array_row_length = NUM_IONS;
    elecSorter->count = (int *) count;
    elecSorter->tdc_pointer = &tdc_ns[0][0];
    if (elec_command >= 2) {
      elecSorter->create_scalefactors_calibrator(true, elecSorter->runtime_u, elecSorter->runtime_v,
                                                  elecSorter->runtime_w, 0.78, elecSorter->fu, elecSorter->fv,
                                                  elecSorter->fw);
    }
    int error_code = elecSorter->init_after_setting_parameters();
    if (error_code) {
      printf("electron sorter could not be initialized\n");
      char error_text[512];
      elecSorter->get_error_text(error_code, 512, error_text);
      printf("Error %i: %s\n", error_code, error_text);
      return 0;
    }
    printf("ok\n");
  }

  printf("creating ROOT canvases... ");
  if (ionSorter && fill_histograms) pRun->createC1();
  if (elecSorter && fill_histograms) pRun->createC2();
  printf("ok\n");

  gSystem->ProcessEvents(); // allow the system to show the histograms

  // open input and output data files:
  char LMF_InputFilename[500];
  sprintf(LMF_InputFilename, "%s", pRun->getLMFFilename());

  // Check LMF file
  if (!LMF->OpenInputLMF(LMF_InputFilename)) {
    printf("could not open input file.\n");
    printf("terminating root app.\n");
    cleanUp();
	if (pRun) delete pRun;
    theRootApp.Terminate();
    return false;
  }
  printf("data file is open for reading\n");
  // empty keyboard buffer
  while (my_kbhit()) {
  }

  // Branch root file
  pRun->branchRootTree(4, 4);

  // Start reading event data from input file:
  // ("event" is all the data that was recorded after a trigger signal)
  printf("reading event data... ");
  while (true) {
    if (LMF->GetEventNumber() % 20000 == 1) {
      if (my_kbhit()) {
        break;
      }
      gSystem->ProcessEvents(); // allow the system to show the histograms
      printf("\rreading event data... %2i %c  ",
             __int32(100 * LMF->GetEventNumber() / LMF->uint64_Numberofevents),
             37);
      if (LMF->GetEventNumber() % 60000 == 1) {
		  pRun->updateC1();
		  pRun->updateC2();
      }
    }

    // read one new event data block from the file:
    memset(count, 0, LMF->number_of_channels * sizeof(int));
    if (!LMF->ReadNextEvent()) break;

    LMF->GetNumberOfHitsArray(count);
//    double timestamp_s = LMF->GetDoubleTimeStamp(); // absolute timestamp in seconds
    LMF->GetTDCDataArray((int *) TDC);

    // convert the raw TDC data to nanoseconds:
    if (ionSorter) {
      if (ionSorter->Cmcp > -1) {
        for (unsigned int i = 0; i < count[ionSorter->Cmcp]; ++i)
          tdc_ns[ionSorter->Cmcp][i] = double(TDC[ionSorter->Cmcp][i]) * tdcresolution;
      }
      for (unsigned int i = 0; i < count[ionSorter->Cu1]; ++i)
        tdc_ns[ionSorter->Cu1][i] = double(TDC[ionSorter->Cu1][i]) * tdcresolution;
      for (unsigned int i = 0; i < count[ionSorter->Cu2]; ++i)
        tdc_ns[ionSorter->Cu2][i] = double(TDC[ionSorter->Cu2][i]) * tdcresolution;
      for (unsigned int i = 0; i < count[ionSorter->Cv1]; ++i)
        tdc_ns[ionSorter->Cv1][i] = double(TDC[ionSorter->Cv1][i]) * tdcresolution;
      for (unsigned int i = 0; i < count[ionSorter->Cv2]; ++i)
        tdc_ns[ionSorter->Cv2][i] = double(TDC[ionSorter->Cv2][i]) * tdcresolution;
      if (ionSorter->use_HEX) {
        for (unsigned int i = 0; i < count[ionSorter->Cw1]; ++i)
          tdc_ns[ionSorter->Cw1][i] = double(TDC[ionSorter->Cw1][i]) * tdcresolution;
        for (unsigned int i = 0; i < count[ionSorter->Cw2]; ++i)
          tdc_ns[ionSorter->Cw2][i] = double(TDC[ionSorter->Cw2][i]) * tdcresolution;
      }
      if (ionSorter->use_HEX) {
        // shift the time sums to zero:
        ionSorter->shift_sums(+1, ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w);
        // shift layer w so that the middle lines of all layers intersect in one point:
        ionSorter->shift_layer_w(+1, ion_w_offset);
      } else {
        // shift the time sums to zero:
        ionSorter->shift_sums(+1, ion_offset_sum_u, ion_offset_sum_v);
      }
      // shift all signals from the anode so that the center of the detector is at x=y=0:
      ionSorter->shift_position_origin(+1, ion_pos_offset_x, ion_pos_offset_y);

      // for calibration of fv, fw, w_offset and correction tables
      ionSorter->feed_calibration_data(true, ion_w_offset);

      if (ionSorter->scalefactors_calibrator && ion_command >= 2) {
        if (ionSorter->scalefactors_calibrator->map_is_full_enough()) break;
      }
      if (ionSorter->use_HEX && fill_histograms)
        pRun->fill2d(Analysis::SortRun::h2_ionXYDev, 
			ionSorter->scalefactors_calibrator->binx - ionSorter->scalefactors_calibrator->detector_map_size / 2.0,
			ionSorter->scalefactors_calibrator->biny - ionSorter->scalefactors_calibrator->detector_map_size / 2.0, 
			ionSorter->scalefactors_calibrator->detector_map_devi_fill);

      if (count[ionSorter->Cu1] > 0 && count[ionSorter->Cu2] > 0) {
        if (count[ionSorter->Cv1] > 0 && count[ionSorter->Cv2] > 0) {
          double u = ionSorter->fu * (tdc_ns[ionSorter->Cu1][0] - tdc_ns[ionSorter->Cu2][0]);
          double v = ionSorter->fv * (tdc_ns[ionSorter->Cv1][0] - tdc_ns[ionSorter->Cv2][0]);
          double y = (u - 2. * v) * 0.577350269; // 0.557 = 1/sqrt(3)
          if (fill_histograms) 
			  pRun->fill2d(Analysis::SortRun::h2_ionXYRaw, u, y);
        }
      }
      if (count[ionSorter->Cu1] > 0 && count[ionSorter->Cu2] > 0) {
        if (fill_histograms) 
			pRun->fill1d(Analysis::SortRun::h1_ionU, tdc_ns[ionSorter->Cu1][0] - tdc_ns[ionSorter->Cu2][0]);
        double mcp = 0.;
        if (ionSorter->use_MCP) {
          if (count[ionSorter->Cmcp] > 0) mcp = tdc_ns[ionSorter->Cmcp][0]; else mcp = -1.e100;
        }
        if (fill_histograms)
          pRun->fill1d(Analysis::SortRun::h1_ionTimesumU, tdc_ns[ionSorter->Cu1][0] + tdc_ns[ionSorter->Cu2][0] - 2 * mcp);
      }
      if (count[ionSorter->Cv1] > 0 && count[ionSorter->Cv2] > 0) {
        if (fill_histograms) 
			pRun->fill1d(Analysis::SortRun::h1_ionV, tdc_ns[ionSorter->Cv1][0] - tdc_ns[ionSorter->Cv2][0]);
        double mcp = 0.;
        if (ionSorter->use_MCP) {
          if (count[ionSorter->Cmcp] > 0) mcp = tdc_ns[ionSorter->Cmcp][0]; else mcp = -1.e100;
        }
        if (fill_histograms)
          pRun->fill1d(Analysis::SortRun::h1_ionTimesumV, tdc_ns[ionSorter->Cv1][0] + tdc_ns[ionSorter->Cv2][0] - 2 * mcp);
      }
      if (ionSorter->use_HEX) {
        if (count[ionSorter->Cw1] > 0 && count[ionSorter->Cw2] > 0) {
          if (fill_histograms) 
			  pRun->fill1d(Analysis::SortRun::h1_ionW, tdc_ns[ionSorter->Cw1][0] - tdc_ns[ionSorter->Cw2][0]);
          double mcp = 0.;
          if (ionSorter->use_MCP) {
            if (count[ionSorter->Cmcp] > 0) mcp = tdc_ns[ionSorter->Cmcp][0]; else mcp = -1.e100;
          }
          if (fill_histograms)
            pRun->fill1d(Analysis::SortRun::h1_ionTimesumW, tdc_ns[ionSorter->Cw1][0] + tdc_ns[ionSorter->Cw2][0] - 2 * mcp);
        }
      }
    }

    if (elecSorter) {
      if (elecSorter->Cmcp > -1) {
        for (unsigned int i = 0; i < count[elecSorter->Cmcp]; ++i)
          tdc_ns[elecSorter->Cmcp][i] = double(TDC[elecSorter->Cmcp][i]) * tdcresolution;
      }
      for (unsigned int i = 0; i < count[elecSorter->Cu1]; ++i)
        tdc_ns[elecSorter->Cu1][i] = double(TDC[elecSorter->Cu1][i]) * tdcresolution;
      for (unsigned int i = 0; i < count[elecSorter->Cu2]; ++i)
        tdc_ns[elecSorter->Cu2][i] = double(TDC[elecSorter->Cu2][i]) * tdcresolution;
      for (unsigned int i = 0; i < count[elecSorter->Cv1]; ++i)
        tdc_ns[elecSorter->Cv1][i] = double(TDC[elecSorter->Cv1][i]) * tdcresolution;
      for (unsigned int i = 0; i < count[elecSorter->Cv2]; ++i)
        tdc_ns[elecSorter->Cv2][i] = double(TDC[elecSorter->Cv2][i]) * tdcresolution;
      if (elecSorter->use_HEX) {
        for (unsigned int i = 0; i < count[elecSorter->Cw1]; ++i)
          tdc_ns[elecSorter->Cw1][i] = double(TDC[elecSorter->Cw1][i]) * tdcresolution;
        for (unsigned int i = 0; i < count[elecSorter->Cw2]; ++i)
          tdc_ns[elecSorter->Cw2][i] = double(TDC[elecSorter->Cw2][i]) * tdcresolution;
      }
      if (elecSorter->use_HEX) {
        // shift the time sums to zero:
        elecSorter->shift_sums(+1, elec_offset_sum_u, elec_offset_sum_v, elec_offset_sum_w);
        // shift layer w so that the middle lines of all layers intersect in one point:
        elecSorter->shift_layer_w(+1, elec_w_offset);
      } else {
        // shift the time sums to zero:
        elecSorter->shift_sums(+1, elec_offset_sum_u, elec_offset_sum_v);
      }

      // shift all signals from the anode so that the center of the detector is at x=y=0:
      elecSorter->shift_position_origin(+1, elec_pos_offset_x, elec_pos_offset_y);

      // for calibration of fv, fw, w_offset and correction tables
      elecSorter->feed_calibration_data(true, elec_w_offset);

      if (elecSorter->scalefactors_calibrator && elec_command >= 2) {
        if (elecSorter->scalefactors_calibrator->map_is_full_enough()) break;
      }
      if (elecSorter->use_HEX && fill_histograms)
		  pRun->fill2d(Analysis::SortRun::h2_elecXYDev, 
			  elecSorter->scalefactors_calibrator->binx - elecSorter->scalefactors_calibrator->detector_map_size / 2.0,
			  elecSorter->scalefactors_calibrator->biny - elecSorter->scalefactors_calibrator->detector_map_size / 2.0,
			  elecSorter->scalefactors_calibrator->detector_map_devi_fill);

      if (count[elecSorter->Cu1] > 0 && count[elecSorter->Cu2] > 0) {
        if (count[elecSorter->Cv1] > 0 && count[elecSorter->Cv2] > 0) {
          double u = elecSorter->fu * (tdc_ns[elecSorter->Cu1][0] - tdc_ns[elecSorter->Cu2][0]);
          double v = elecSorter->fv * (tdc_ns[elecSorter->Cv1][0] - tdc_ns[elecSorter->Cv2][0]);
          double y = (u - 2. * v) * 0.577350269; // 0.557 = 1/sqrt(3)
          if (fill_histograms) 
			  pRun->fill2d(Analysis::SortRun::h2_elecXYRaw, u, y);
        }
      }
      if (count[elecSorter->Cu1] > 0 && count[elecSorter->Cu2] > 0) {
        if (fill_histograms) 
			pRun->fill1d(Analysis::SortRun::h1_elecU, tdc_ns[elecSorter->Cu1][0] - tdc_ns[elecSorter->Cu2][0]);
        double mcp = 0.;
        if (elecSorter->use_MCP) {
          if (count[elecSorter->Cmcp] > 0) mcp = tdc_ns[elecSorter->Cmcp][0]; else mcp = -1.e100;
        }
        if (fill_histograms) 
			pRun->fill1d(Analysis::SortRun::h1_elecTimesumU, tdc_ns[elecSorter->Cu1][0] + tdc_ns[elecSorter->Cu2][0] - 2 * mcp);
      }
      if (count[elecSorter->Cv1] > 0 && count[elecSorter->Cv2] > 0) {
        if (fill_histograms) 
			pRun->fill1d(Analysis::SortRun::h1_elecV, tdc_ns[elecSorter->Cv1][0] - tdc_ns[elecSorter->Cv2][0]);
        double mcp = 0.;
        if (elecSorter->use_MCP) {
          if (count[elecSorter->Cmcp] > 0) mcp = tdc_ns[elecSorter->Cmcp][0]; else mcp = -1.e100;
        }
        if (fill_histograms) 
			pRun->fill1d(Analysis::SortRun::h1_elecTimesumV, tdc_ns[elecSorter->Cv1][0] + tdc_ns[elecSorter->Cv2][0] - 2 * mcp);
      }
      if (elecSorter->use_HEX) {
        if (count[elecSorter->Cw1] > 0 && count[elecSorter->Cw2] > 0) {
          if (fill_histograms) 
			  pRun->fill1d(Analysis::SortRun::h1_elecW, tdc_ns[elecSorter->Cw1][0] - tdc_ns[elecSorter->Cw2][0]);
          double mcp = 0.;
          if (elecSorter->use_MCP) {
            if (count[elecSorter->Cmcp] > 0) mcp = tdc_ns[elecSorter->Cmcp][0]; else mcp = -1.e100;
          }
          if (fill_histograms) 
			  pRun->fill1d(Analysis::SortRun::h1_elecTimesumW, tdc_ns[elecSorter->Cw1][0] + tdc_ns[elecSorter->Cw2][0] - 2 * mcp);
        }
      }
    }

    int number_of_ions = 0;
    if (ionSorter) {
      if (ion_command == 1) {  // sort and write new file
        // sort/reconstruct the detector signals and apply the sum- and NL-correction.
        number_of_ions = ionSorter->sort();
        // "number_of_ions" is the number of reconstructed number of particles
      } else {
        number_of_ions = ionSorter->run_without_sorting();
      }
      for (int i = 0; i < number_of_ions; i++) {
        if (fill_histograms)
          pRun->fill2d(Analysis::SortRun::h2_ionXY, ionSorter->output_hit_array[i]->x, ionSorter->output_hit_array[i]->y);
      }
    }
    int number_of_electrons = 0;
    if (elecSorter) {
      if (elec_command == 1) {  // sort and write new file
        // sort/reconstruct the detector signals and apply the sum- and NL-correction.
        number_of_electrons = elecSorter->sort();
        // "number_of_electrons" is the number of reconstructed number of particles
      } else {
        number_of_electrons = elecSorter->run_without_sorting();
      }
      for (int i = 0; i < number_of_electrons; i++) {
        if (fill_histograms)
          pRun->fill2d(Analysis::SortRun::h2_elecXY, elecSorter->output_hit_array[i]->x, elecSorter->output_hit_array[i]->y);
      }
    }
    const int eMarkerCh = 16;
    double eMarker = 0;
    if (elecSorter) {
      double mcp = 0.;
      if (elecSorter->use_MCP) {
        if (count[elecSorter->Cmcp] > 0) mcp = tdc_ns[elecSorter->Cmcp][0]; else mcp = -1.e100;
      }
      eMarker = mcp - TDC[eMarkerCh][0]*tdcresolution;
    }
    pRun->processEvent(number_of_ions, ionSorter, number_of_electrons, elecSorter, eMarker);

    // Write to output file
    FILE *outfile = nullptr;
    if (outfile) {
      if (ionSorter) {
        // the following steps are necessary to make the new output look as the old one
        // (in respect to time offsets)

        // shift the detector signals back (note the -1 instead of the +1)
        if (ionSorter->use_HEX)
          ionSorter->shift_sums(-1, ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w);
        if (!ionSorter->use_HEX) ionSorter->shift_sums(-1, ion_offset_sum_u, ion_offset_sum_v);
        ionSorter->shift_layer_w(-1, ion_w_offset);
        ionSorter->shift_position_origin(-1, ion_pos_offset_x, ion_pos_offset_y);

        // convert the times from nanoseconds back to raw channels:
        if (ionSorter->Cmcp > -1) {
          for (unsigned int i = 0; i < count[ionSorter->Cmcp]; ++i)
            TDC[ionSorter->Cmcp][i] = int(tdc_ns[ionSorter->Cmcp][i] / tdcresolution);
        }
        for (unsigned int i = 0; i < count[ionSorter->Cu1]; ++i)
          TDC[ionSorter->Cu1][i] = int(tdc_ns[ionSorter->Cu1][i] / tdcresolution);
        for (unsigned int i = 0; i < count[ionSorter->Cu2]; ++i)
          TDC[ionSorter->Cu2][i] = int(tdc_ns[ionSorter->Cu2][i] / tdcresolution);
        for (unsigned int i = 0; i < count[ionSorter->Cv1]; ++i)
          TDC[ionSorter->Cv1][i] = int(tdc_ns[ionSorter->Cv1][i] / tdcresolution);
        for (unsigned int i = 0; i < count[ionSorter->Cv2]; ++i)
          TDC[ionSorter->Cv2][i] = int(tdc_ns[ionSorter->Cv2][i] / tdcresolution);
        if (ionSorter->use_HEX) {
          for (unsigned int i = 0; i < count[ionSorter->Cw1]; ++i)
            TDC[ionSorter->Cw1][i] = int(tdc_ns[ionSorter->Cw1][i] / tdcresolution);
          for (unsigned int i = 0; i < count[ionSorter->Cw2]; ++i)
            TDC[ionSorter->Cw2][i] = int(tdc_ns[ionSorter->Cw2][i] / tdcresolution);
        }
      }

      if (elecSorter) {
        // shift the detector signals back (note the -1 instead of the +1)
        if (elecSorter->use_HEX)
          elecSorter->shift_sums(-1, elec_offset_sum_u, elec_offset_sum_v, elec_offset_sum_w);
        if (!elecSorter->use_HEX) elecSorter->shift_sums(-1, elec_offset_sum_u, elec_offset_sum_v);
        elecSorter->shift_layer_w(-1, elec_w_offset);
        elecSorter->shift_position_origin(-1, elec_pos_offset_x, elec_pos_offset_y);

        // convert the times from nanoseconds back to raw channels:
        if (elecSorter->Cmcp > -1) {
          for (unsigned int i = 0; i < count[elecSorter->Cmcp]; ++i)
            TDC[elecSorter->Cmcp][i] = int(tdc_ns[elecSorter->Cmcp][i] / tdcresolution);
        }
        for (unsigned int i = 0; i < count[elecSorter->Cu1]; ++i)
          TDC[elecSorter->Cu1][i] = int(tdc_ns[elecSorter->Cu1][i] / tdcresolution);
        for (unsigned int i = 0; i < count[elecSorter->Cu2]; ++i)
          TDC[elecSorter->Cu2][i] = int(tdc_ns[elecSorter->Cu2][i] / tdcresolution);
        for (unsigned int i = 0; i < count[elecSorter->Cv1]; ++i)
          TDC[elecSorter->Cv1][i] = int(tdc_ns[elecSorter->Cv1][i] / tdcresolution);
        for (unsigned int i = 0; i < count[elecSorter->Cv2]; ++i)
          TDC[elecSorter->Cv2][i] = int(tdc_ns[elecSorter->Cv2][i] / tdcresolution);
        if (elecSorter->use_HEX) {
          for (unsigned int i = 0; i < count[elecSorter->Cw1]; ++i)
            TDC[elecSorter->Cw1][i] = int(tdc_ns[elecSorter->Cw1][i] / tdcresolution);
          for (unsigned int i = 0; i < count[elecSorter->Cw2]; ++i)
            TDC[elecSorter->Cw2][i] = int(tdc_ns[elecSorter->Cw2][i] / tdcresolution);
        }
      }

      // output TDC data
      for (unsigned int i = 0; i < LMF->number_of_channels; i++) {
        fwrite(&count[i], sizeof(int), 1, outfile);
        for (unsigned int j = 0; j < count[i]; j++) fwrite(&TDC[i][j], sizeof(int), 1, outfile);
      }
    }

  } // end of the big while loop
  printf("ok\n");

  if (ion_command == 2) {
    printf("calibrating ion detector... ");
    ionSorter->do_calibration();
    printf("ok\n");
    if (ionSorter->scalefactors_calibrator) {
      printf("ion: Good scalefactors are:\nf_U = %lg\nf_V = %lg\nf_W = %lg\nOffset on layer W = %lg\n",
             2. * ionSorter->fu, 2. * ionSorter->scalefactors_calibrator->best_fv,
             2. * ionSorter->scalefactors_calibrator->best_fw,
             ionSorter->scalefactors_calibrator->best_w_offset);
    }
  }
  if (elec_command == 2) {
    printf("calibrating elec detector... ");
    elecSorter->do_calibration();
    printf("ok\n");
    if (elecSorter->scalefactors_calibrator) {
      printf("elec: Good scalefactors are:\nf_U = %lg\nf_V = %lg\nf_W = %lg\nOffset on layer W = %lg\n",
             2. * elecSorter->fu, 2. * elecSorter->scalefactors_calibrator->best_fv,
             2. * elecSorter->scalefactors_calibrator->best_fw,
             elecSorter->scalefactors_calibrator->best_w_offset);
    }
  }
  if (ion_command == 3) {   // generate and print correction tables for sum- and position-correction
    printf("ion: creating calibration tables...\n");
    create_calibration_tables(ionCalibTabFilename.c_str(), ionSorter);
    printf("\nfinished creating calibration tables\n");
  }
  if (elec_command == 3) {   // generate and print correction tables for sum- and position-correction
    printf("elec: creating calibration tables...\n");
    create_calibration_tables(elecClibTabFilename.c_str(), elecSorter);
    printf("\nfinished creating calibration tables\n");
  }

  // Update canvases
  {
	  bool tBool = true;
	  pRun->updateC1(&tBool);
	  pRun->updateC2(&tBool);
  }

  printf("hit any key to exit\n");
  while (true) {
    gSystem->Sleep(5);
    gSystem->ProcessEvents();
    if (my_kbhit()) break;
  }

  // Finish the program
  printf("terminating the root app.\n");
  cleanUp();
	if (pRun) delete pRun;
  theRootApp.Terminate();
  std::cout << "The program is done. " << std::endl;
  return 0;
}
