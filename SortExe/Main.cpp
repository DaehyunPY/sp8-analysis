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

sort_class *ion_sorter;
TCanvas *ion_canvas;
double ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w;
double ion_w_offset, ion_pos_offset_x, ion_pos_offset_y;
int ion_command;

sort_class *elec_sorter;
TCanvas *elec_canvas;
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
    int i = -1;
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
        if (real_numbers_found) break;
    }

    return;
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

bool read_config_file(char *name, sort_class *&sorter, int &command, double &offset_sum_u, double &offset_sum_v,
                      double &offset_sum_w, double &w_offset, double &pos_offset_x, double &pos_offset_y) {
    // read the config file:
    printf("opening %s... ", name);
    FILE *parameterfile_handle = fopen(name, "rt");

    if (!parameterfile_handle) {
        printf("file %s was not found.%c\n", name, 7);
        return false;
    }
    printf("ok\n");
    printf("reading %s... ", name);
    int int_dummy;
    command = read_int(parameterfile_handle);

    if (command == -1) {
        printf("ok\n");
        return false;
    }

    int_dummy = read_int(parameterfile_handle);
    sorter->use_HEX = int_dummy ? true : false;

    int_dummy = read_int(parameterfile_handle);
    sorter->common_start_mode = (!int_dummy) ? true : false;

    sorter->Cu1 = read_int(parameterfile_handle) - 1;
    sorter->Cu2 = read_int(parameterfile_handle) - 1;
    sorter->Cv1 = read_int(parameterfile_handle) - 1;
    sorter->Cv2 = read_int(parameterfile_handle) - 1;
    sorter->Cw1 = read_int(parameterfile_handle) - 1;
    sorter->Cw2 = read_int(parameterfile_handle) - 1;

    sorter->Cmcp = read_int(parameterfile_handle) - 1;
    sorter->use_MCP = (sorter->Cmcp > -1) ? true : false;

    offset_sum_u = read_double(parameterfile_handle);
    offset_sum_v = read_double(parameterfile_handle);
    offset_sum_w = read_double(parameterfile_handle);

    pos_offset_x = read_double(parameterfile_handle);
    pos_offset_y = read_double(parameterfile_handle);

    sorter->uncorrected_time_sum_half_width_u = read_double(parameterfile_handle);
    sorter->uncorrected_time_sum_half_width_v = read_double(parameterfile_handle);
    sorter->uncorrected_time_sum_half_width_w = read_double(parameterfile_handle);

    sorter->fu = 0.5 * read_double(parameterfile_handle);
    sorter->fv = 0.5 * read_double(parameterfile_handle);
    sorter->fw = 0.5 * read_double(parameterfile_handle);
    w_offset = read_double(parameterfile_handle);
    sorter->runtime_u = read_double(parameterfile_handle);
    sorter->runtime_v = read_double(parameterfile_handle);
    if (!sorter->runtime_v) sorter->runtime_v = sorter->runtime_u;
    sorter->runtime_w = read_double(parameterfile_handle);
    if (!sorter->runtime_w) sorter->runtime_w = sorter->runtime_u;

    sorter->MCP_radius = read_double(parameterfile_handle);

    sorter->dead_time_anode = read_double(parameterfile_handle);
    sorter->dead_time_mcp = read_double(parameterfile_handle);

    int_dummy = read_int(parameterfile_handle);
    sorter->use_sum_correction = (int_dummy != 0) ? true : false;
    int_dummy = read_int(parameterfile_handle);
    sorter->use_pos_correction = (int_dummy != 0) ? true : false;

    __int32 check_dummy = read_int(parameterfile_handle);
    if (check_dummy != 88888) {
        printf("File %s was not correctly read.\n", name);
        fclose(parameterfile_handle);
        if (sorter) {
            delete sorter;
            sorter = 0;
        }
        return false;
    }

    fclose(parameterfile_handle);
    // end of reading the config file:
    printf("ok\n");

    return true;
}

bool read_calibration_tables(char *filename, sort_class *sorter) {
    if (!filename) return false;
    if (!sorter) return false;

    FILE *infile_handle = fopen(filename, "rt");
    if (!infile_handle) return false;
    int points = 0;

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

    if (infile_handle) {
        fclose(infile_handle);
        infile_handle = 0;
    }
    return true;
}

bool create_calibration_tables(char *filename, sort_class *sorter) {
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
    fo = 0;
    return true;
}

void clean_up1() {
    while (my_kbhit()); // empty keyboard buffer

    printf("deleting the ion sorter... ");
    if (ion_sorter) {
        delete ion_sorter;
        ion_sorter = 0;
    }
    printf("ok \n");

    printf("deleting the elec sorter... ");
    if (elec_sorter) {
        delete elec_sorter;
        elec_sorter = 0;
    }
    printf("ok \n");

    printf("deleting the LMF reader instance... ");
    if (LMF) {
        delete LMF;
        LMF = 0;
    }
    printf("ok \n");

    if (ion_canvas) {
        for (int i = 1; i <= 9; i++) {
            ion_canvas->cd(i);
            gPad->Modified(true);
            gPad->Update();
        }
    }

    if (elec_canvas) {
        for (int i = 1; i <= 9; i++) {
            elec_canvas->cd(i);
            gPad->Modified(true);
            gPad->Update();
        }
    }
}

void clean_up2() {
    if (ion_canvas) {
        printf("closing ion canvas\n");
        ion_canvas->Close();
        delete ion_canvas;
        ion_canvas = 0;
    }
    if (elec_canvas) {
        printf("closing electron canvas\n");
        elec_canvas->Close();
        delete elec_canvas;
        elec_canvas = 0;
    }
}

int main(int argc, char *argv[]) {
    // Inform status
    printf("syntax: SortExe filename\n");
    printf("        This file will be sorted and\n");
    printf("        a new file will be written.\n\n");
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 0;
    }
    if (argc > 2) {
        printf("too many arguments\n");
        return 0;
    }
    std::cout << "The exe file which place at " << argv[0] << ", is running now. " << std::endl;
    std::cout << "The configure file which place at " << argv[1] << ", is going to be read. " << std::endl;
    std::cout << "To quit this program safely, input 'quit'. " << std::endl;

    // start the Root-Environment
    char *root_argv[3];
    char argv2[50], argv3[50];
    int root_argc = 2;
    sprintf(argv2, "-l");
    root_argv[0] = argv[0];
    root_argv[1] = argv2;
    root_argv[2] = argv3;
    TApplication theRootApp("theRootApp", &root_argc, root_argv);

    // Setup Run
    Analysis::SortRun *pRun;
    pRun = new Analysis::SortRun(argv[1]);
    bool fill_histograms = true;
//    TFile *rootfile = rt->RecreateRootFile("output.root", "");
    TFile *rootfile = pRun->getRootFile();

    ion_canvas = 0;
    elec_canvas = 0;

    TH1D *Hion_sum_u = 0;
    TH1D *Hion_sum_v = 0;
    TH1D *Hion_sum_w = 0;
    TH1D *Hion_u = 0;
    TH1D *Hion_v = 0;
    TH1D *Hion_w = 0;
    TH2D *Hion_xy_raw = 0;
    TH2D *Hion_xy = 0;
    TH2D *Hion_xy_dev = 0;

    TH1D *Helec_sum_u = 0;
    TH1D *Helec_sum_v = 0;
    TH1D *Helec_sum_w = 0;
    TH1D *Helec_u = 0;
    TH1D *Helec_v = 0;
    TH1D *Helec_w = 0;
    TH2D *Helec_xy_raw = 0;
    TH2D *Helec_xy = 0;
    TH2D *Helec_xy_dev = 0;

    LMF = new LMF_IO(NUM_CHANNELS, NUM_IONS);

    double tdcresolution = 0.025; // 25ps tdc bin size
    int TDC[NUM_CHANNELS][NUM_IONS];
    double tdc_ns[NUM_CHANNELS][NUM_IONS];
    unsigned int count[NUM_CHANNELS];
    ion_command = elec_command = -1;

    // The "command"-value is set in the first line of "elec_sorter.txt" and "ion_sorter.txt"
    // 0 = only convert to new file format
    // 1 = sort and write new file
    // 2 = calibrate fv, fw, w_offset
    // 3 = create calibration table files


    // create the sorter:
    printf("creating the ion sorter... ");
    ion_sorter = new sort_class();
    printf("ok\n");

    printf("creating the electron sorter... ");
    elec_sorter = new sort_class();
    printf("ok\n");


    if (!read_config_file(pRun->getIonSorterFilename(), ion_sorter, ion_command, ion_offset_sum_u, ion_offset_sum_v,
                          ion_offset_sum_w, ion_w_offset, ion_pos_offset_x, ion_pos_offset_y)) {
        delete ion_sorter;
        ion_sorter = 0;
    }
    if (ion_sorter) {
        if (ion_sorter->use_sum_correction || ion_sorter->use_pos_correction) {
            read_calibration_tables(pRun->getIonCalibTableFilename(), ion_sorter);
        }
    }

    if (!read_config_file(pRun->getElecSorterFilename(), elec_sorter, elec_command, elec_offset_sum_u,
                          elec_offset_sum_v,
                          elec_offset_sum_w, elec_w_offset, elec_pos_offset_x, elec_pos_offset_y)) {
        delete elec_sorter;
        elec_sorter = 0;
    }
    if (elec_sorter) {
        if (elec_sorter->use_sum_correction || elec_sorter->use_pos_correction) {
            read_calibration_tables(pRun->getElecCalibTableFilename(), elec_sorter);
        }
    }

    if (ion_command > 1 && elec_command > 1) {
        printf("Error: Do not calibrate 2 detectors simultaneously.\n");
        return 0;
    }

    if (ion_command == -1 && elec_command == -1) {
        printf("no config file was read. Nothing to do.\n");
        return 0;
    }


    // initialization of the sorters:
    if (ion_sorter) {
        printf("init ion sorter... ");
        ion_sorter->TDC_resolution_ns = tdcresolution;
        ion_sorter->tdc_array_row_length = NUM_IONS;
        ion_sorter->count = (int *) count;
        ion_sorter->tdc_pointer = &tdc_ns[0][0];
        if (ion_command >= 2) {
            ion_sorter->create_scalefactors_calibrator(true, ion_sorter->runtime_u, ion_sorter->runtime_v,
                                                       ion_sorter->runtime_w, 0.78, ion_sorter->fu, ion_sorter->fv,
                                                       ion_sorter->fw);
        }
        int error_code = ion_sorter->init_after_setting_parameters();
        if (error_code) {
            printf("ion sorter could not be initialized\n");
            char error_text[512];
            ion_sorter->get_error_text(error_code, 512, error_text);
            printf("Error %i: %s\n", error_code, error_text);
            return 0;
        }
        printf("ok\n");
    }

    if (elec_sorter) {
        printf("init electron sorter... ");
        elec_sorter->TDC_resolution_ns = tdcresolution;
        elec_sorter->tdc_array_row_length = NUM_IONS;
        elec_sorter->count = (int *) count;
        elec_sorter->tdc_pointer = &tdc_ns[0][0];
        if (elec_command >= 2) {
            elec_sorter->create_scalefactors_calibrator(true, elec_sorter->runtime_u, elec_sorter->runtime_v,
                                                        elec_sorter->runtime_w, 0.78, elec_sorter->fu, elec_sorter->fv,
                                                        elec_sorter->fw);
        }
        int error_code = elec_sorter->init_after_setting_parameters();
        if (error_code) {
            printf("electron sorter could not be initialized\n");
            char error_text[512];
            elec_sorter->get_error_text(error_code, 512, error_text);
            printf("Error %i: %s\n", error_code, error_text);
            return 0;
        }
        printf("ok\n");
    }

    if (ion_sorter && fill_histograms) {
        ion_canvas = pRun->newCanvas("ion_canvas", "ion_canvas", 10, 10, 910, 910);
        ion_canvas->Divide(3, 3);
        ion_canvas->cd(1);
        Hion_sum_u = pRun->newTH1D("ion_sum_u", "", 500 * 10, -250, 250.);
        Hion_sum_u->Draw();
        ion_canvas->cd(2);
        Hion_sum_v = pRun->newTH1D("ion_sum_v", "", 500 * 10, -250, 250.);
        Hion_sum_v->Draw();
        ion_canvas->cd(3);
        Hion_sum_w = pRun->newTH1D("ion_sum_w", "", 500 * 10, -250, 250.);
        Hion_sum_w->Draw();
        ion_canvas->cd(4);
        Hion_u = pRun->newTH1D("ion_u", "", 500 * 2, -250, 250.);
        Hion_u->Draw();
        ion_canvas->cd(5);
        Hion_v = pRun->newTH1D("ion_v", "", 500 * 2, -250, 250.);
        Hion_v->Draw();
        ion_canvas->cd(6);
        Hion_w = pRun->newTH1D("ion_w", "", 500 * 2, -250, 250.);
        Hion_w->Draw();
        ion_canvas->cd(7);
        Hion_xy_raw = pRun->newTH2D("ion_xy_raw", "", 120 * 4, -60, 60., 120 * 4, -60, 60., "COLZ");
        Hion_xy_raw->Draw();
        ion_canvas->cd(8);
        Hion_xy = pRun->newTH2D("ion_xy", "", 120 * 4, -60, 60., 120 * 4, -60, 60., "COLZ");
        Hion_xy->Draw();
        int s = 100;
        ion_canvas->cd(9);
        Hion_xy_dev = pRun->newTH2D("ion_xy_dev", "", s * 2, -double(s), double(s), 2 * s, -double(s), double(s),
                                    "COLZ");
        Hion_xy_dev->Draw();
    }

    if (elec_sorter && fill_histograms) {
        elec_canvas = pRun->newCanvas("elec_canvas", "elec_canvas", 10, 10, 910, 910);
        elec_canvas->Divide(3, 3);
        elec_canvas->cd(1);
        Helec_sum_u = pRun->newTH1D("elec_sum_u", "", 500 * 10, -250, 250.);
        Helec_sum_u->Draw();
        elec_canvas->cd(2);
        Helec_sum_v = pRun->newTH1D("elec_sum_v", "", 500 * 10, -250, 250.);
        Helec_sum_v->Draw();
        elec_canvas->cd(3);
        Helec_sum_w = pRun->newTH1D("elec_sum_w", "", 500 * 10, -250, 250.);
        Helec_sum_w->Draw();
        elec_canvas->cd(4);
        Helec_u = pRun->newTH1D("elec_u", "", 500 * 2, -250, 250.);
        Helec_u->Draw();
        elec_canvas->cd(5);
        Helec_v = pRun->newTH1D("elec_v", "", 500 * 2, -250, 250.);
        Helec_v->Draw();
        elec_canvas->cd(6);
        Helec_w = pRun->newTH1D("elec_w", "", 500 * 2, -250, 250.);
        Helec_w->Draw();
        elec_canvas->cd(7);
        Helec_xy_raw = pRun->newTH2D("elec_xy_raw", "", 120 * 4, -60, 60., 120 * 4, -60, 60., "COLZ");
        Helec_xy_raw->Draw();
        elec_canvas->cd(8);
        Helec_xy = pRun->newTH2D("elec_xy", "", 120 * 4, -60, 60., 120 * 4, -60, 60., "COLZ");
        Helec_xy->Draw();
        int s = 100;
        elec_canvas->cd(9);
        Helec_xy_dev = pRun->newTH2D("elec_xy_dev", "", s * 2, -double(s), double(s), 2 * s, -double(s), double(s),
                                     "COLZ");
        Helec_xy_dev->Draw();
    }

    gSystem->ProcessEvents(); // allow the system to show the histograms

    // open input and output data files:
    char LMF_InputFilename[500];
    char OutputFilename[500];


    // open the data input file:
    sprintf(LMF_InputFilename, "%s", pRun->getLMFFilename());

    // Check LMF file
    if (!LMF->OpenInputLMF(LMF_InputFilename)) {
        printf("could not open input file.\n");
        printf("terminating root app.\n");
        clean_up1();
        if (pRun) delete pRun;
		clean_up2();
        theRootApp.Terminate();
        return false;
    }

    printf("data file is open for reading\n");

    FILE *outfile = 0;
    sprintf(OutputFilename, "%s.sorted", LMF_InputFilename);
    if (ion_command == 0 || ion_command == 1 || elec_command == 0 || elec_command == 1) {
        outfile = fopen(OutputFilename, "wb");
        if (!outfile) {
            printf("could not open output file.\n");
            outfile = 0;
            return false;
        }
        if (ferror(outfile)) {
            printf("could not open output file.\n");
            outfile = 0;
            return false;
        }
    }

    if (outfile) printf("new data file is open for writing\n");

    // now start to read the input file:
    LMF->number_of_channels = LMF->number_of_channels;
    if (outfile) fwrite(&LMF->number_of_channels, sizeof(unsigned int), 1, outfile);

    while (my_kbhit()); // empty keyboard buffer


    // Branch root file
    if (ion_sorter && elec_sorter) {
        int number_of_ions = 0;
        int number_of_electrons = 0;
        if (ion_command == 1) {  // sort and write new file
            // sort/reconstruct the detector signals and apply the sum- and NL-correction.
            number_of_ions = ion_sorter->sort();
            // "number_of_ions" is the number of reconstructed number of particles
        } else {
            number_of_ions = ion_sorter->run_without_sorting();
        }
        if (elec_command == 1) {  // sort and write new file
            // sort/reconstruct the detector signals and apply the sum- and NL-correction.
            number_of_electrons = elec_sorter->sort();
            // "number_of_electrons" is the number of reconstructed number of particles
        } else {
            number_of_electrons = elec_sorter->run_without_sorting();
        }
        pRun->branchRootTree(number_of_ions, number_of_electrons);
    }

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
                if (ion_canvas) {
                    for (__int32 i = 0; i <= 9; i++) {
                        //ion_canvas->cd(i)->Modified(true);
                        ion_canvas->cd(i)->Update();
                    }
                }
                if (elec_canvas) {
                    for (__int32 i = 0; i <= 9; i++) {
                        //elec_canvas->cd(i)->Modified(true);
                        elec_canvas->cd(i)->Update();
                    }
                }
            }
        }

        // read one new event data block from the file:
        memset(count, 0, LMF->number_of_channels * sizeof(int));
        if (!LMF->ReadNextEvent()) break;

        LMF->GetNumberOfHitsArray(count);
        double timestamp_s = LMF->GetDoubleTimeStamp(); // absolute timestamp in seconds
        LMF->GetTDCDataArray((int *) TDC);

        // convert the raw TDC data to nanoseconds:
        if (ion_sorter) {
            if (ion_sorter->Cmcp > -1) {
                for (unsigned int i = 0; i < count[ion_sorter->Cmcp]; ++i)
                    tdc_ns[ion_sorter->Cmcp][i] = double(TDC[ion_sorter->Cmcp][i]) * tdcresolution;
            }
            for (unsigned int i = 0; i < count[ion_sorter->Cu1]; ++i)
                tdc_ns[ion_sorter->Cu1][i] = double(TDC[ion_sorter->Cu1][i]) * tdcresolution;
            for (unsigned int i = 0; i < count[ion_sorter->Cu2]; ++i)
                tdc_ns[ion_sorter->Cu2][i] = double(TDC[ion_sorter->Cu2][i]) * tdcresolution;
            for (unsigned int i = 0; i < count[ion_sorter->Cv1]; ++i)
                tdc_ns[ion_sorter->Cv1][i] = double(TDC[ion_sorter->Cv1][i]) * tdcresolution;
            for (unsigned int i = 0; i < count[ion_sorter->Cv2]; ++i)
                tdc_ns[ion_sorter->Cv2][i] = double(TDC[ion_sorter->Cv2][i]) * tdcresolution;
            if (ion_sorter->use_HEX) {
                for (unsigned int i = 0; i < count[ion_sorter->Cw1]; ++i)
                    tdc_ns[ion_sorter->Cw1][i] = double(TDC[ion_sorter->Cw1][i]) * tdcresolution;
                for (unsigned int i = 0; i < count[ion_sorter->Cw2]; ++i)
                    tdc_ns[ion_sorter->Cw2][i] = double(TDC[ion_sorter->Cw2][i]) * tdcresolution;
            }
            if (ion_sorter->use_HEX) {
                // shift the time sums to zero:
                ion_sorter->shift_sums(+1, ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w);
                // shift layer w so that the middle lines of all layers intersect in one point:
                ion_sorter->shift_layer_w(+1, ion_w_offset);
            } else {
                // shift the time sums to zero:
                ion_sorter->shift_sums(+1, ion_offset_sum_u, ion_offset_sum_v);
            }
            // shift all signals from the anode so that the center of the detector is at x=y=0:
            ion_sorter->shift_position_origin(+1, ion_pos_offset_x, ion_pos_offset_y);

            ion_sorter->feed_calibration_data(true,
                                              ion_w_offset); // for calibration of fv, fw, w_offset and correction tables
            if (ion_sorter->scalefactors_calibrator && ion_command >= 2) {
                if (ion_sorter->scalefactors_calibrator->map_is_full_enough()) break;
            }
            if (ion_sorter->use_HEX && fill_histograms)
                Hion_xy_dev->Fill(ion_sorter->scalefactors_calibrator->binx -
                                  ion_sorter->scalefactors_calibrator->detector_map_size / 2,
                                  ion_sorter->scalefactors_calibrator->biny -
                                  ion_sorter->scalefactors_calibrator->detector_map_size / 2,
                                  ion_sorter->scalefactors_calibrator->detector_map_devi_fill);

            if (count[ion_sorter->Cu1] > 0 && count[ion_sorter->Cu2] > 0) {
                if (count[ion_sorter->Cv1] > 0 && count[ion_sorter->Cv2] > 0) {
                    double u = ion_sorter->fu * (tdc_ns[ion_sorter->Cu1][0] - tdc_ns[ion_sorter->Cu2][0]);
                    double v = ion_sorter->fv * (tdc_ns[ion_sorter->Cv1][0] - tdc_ns[ion_sorter->Cv2][0]);
                    double y = (u - 2. * v) * 0.577350269; // 0.557 = 1/sqrt(3)
                    if (fill_histograms) Hion_xy_raw->Fill(u, y);
                }
            }

            if (count[ion_sorter->Cu1] > 0 && count[ion_sorter->Cu2] > 0) {
                if (fill_histograms) Hion_u->Fill(tdc_ns[ion_sorter->Cu1][0] - tdc_ns[ion_sorter->Cu2][0]);
                double mcp = 0.;
                if (ion_sorter->use_MCP) {
                    if (count[ion_sorter->Cmcp] > 0) mcp = tdc_ns[ion_sorter->Cmcp][0]; else mcp = -1.e100;
                }
                if (fill_histograms)
                    Hion_sum_u->Fill(tdc_ns[ion_sorter->Cu1][0] + tdc_ns[ion_sorter->Cu2][0] - 2 * mcp);
            }
            if (count[ion_sorter->Cv1] > 0 && count[ion_sorter->Cv2] > 0) {
                if (fill_histograms) Hion_v->Fill(tdc_ns[ion_sorter->Cv1][0] - tdc_ns[ion_sorter->Cv2][0]);
                double mcp = 0.;
                if (ion_sorter->use_MCP) {
                    if (count[ion_sorter->Cmcp] > 0) mcp = tdc_ns[ion_sorter->Cmcp][0]; else mcp = -1.e100;
                }
                if (fill_histograms)
                    Hion_sum_v->Fill(tdc_ns[ion_sorter->Cv1][0] + tdc_ns[ion_sorter->Cv2][0] - 2 * mcp);
            }
            if (ion_sorter->use_HEX) {
                if (count[ion_sorter->Cw1] > 0 && count[ion_sorter->Cw2] > 0) {
                    if (fill_histograms) Hion_w->Fill(tdc_ns[ion_sorter->Cw1][0] - tdc_ns[ion_sorter->Cw2][0]);
                    double mcp = 0.;
                    if (ion_sorter->use_MCP) {
                        if (count[ion_sorter->Cmcp] > 0) mcp = tdc_ns[ion_sorter->Cmcp][0]; else mcp = -1.e100;
                    }
                    if (fill_histograms)
                        Hion_sum_w->Fill(tdc_ns[ion_sorter->Cw1][0] + tdc_ns[ion_sorter->Cw2][0] - 2 * mcp);
                }
            }
        }

        if (elec_sorter) {
            if (elec_sorter->Cmcp > -1) {
                for (unsigned int i = 0; i < count[elec_sorter->Cmcp]; ++i)
                    tdc_ns[elec_sorter->Cmcp][i] = double(TDC[elec_sorter->Cmcp][i]) * tdcresolution;
            }
            for (unsigned int i = 0; i < count[elec_sorter->Cu1]; ++i)
                tdc_ns[elec_sorter->Cu1][i] = double(TDC[elec_sorter->Cu1][i]) * tdcresolution;
            for (unsigned int i = 0; i < count[elec_sorter->Cu2]; ++i)
                tdc_ns[elec_sorter->Cu2][i] = double(TDC[elec_sorter->Cu2][i]) * tdcresolution;
            for (unsigned int i = 0; i < count[elec_sorter->Cv1]; ++i)
                tdc_ns[elec_sorter->Cv1][i] = double(TDC[elec_sorter->Cv1][i]) * tdcresolution;
            for (unsigned int i = 0; i < count[elec_sorter->Cv2]; ++i)
                tdc_ns[elec_sorter->Cv2][i] = double(TDC[elec_sorter->Cv2][i]) * tdcresolution;
            if (elec_sorter->use_HEX) {
                for (unsigned int i = 0; i < count[elec_sorter->Cw1]; ++i)
                    tdc_ns[elec_sorter->Cw1][i] = double(TDC[elec_sorter->Cw1][i]) * tdcresolution;
                for (unsigned int i = 0; i < count[elec_sorter->Cw2]; ++i)
                    tdc_ns[elec_sorter->Cw2][i] = double(TDC[elec_sorter->Cw2][i]) * tdcresolution;
            }
            if (elec_sorter->use_HEX) {
                // shift the time sums to zero:
                elec_sorter->shift_sums(+1, elec_offset_sum_u, elec_offset_sum_v, elec_offset_sum_w);
                // shift layer w so that the middle lines of all layers intersect in one point:
                elec_sorter->shift_layer_w(+1, elec_w_offset);
            } else {
                // shift the time sums to zero:
                elec_sorter->shift_sums(+1, elec_offset_sum_u, elec_offset_sum_v);
            }
            // shift all signals from the anode so that the center of the detector is at x=y=0:
            elec_sorter->shift_position_origin(+1, elec_pos_offset_x, elec_pos_offset_y);

            elec_sorter->feed_calibration_data(true,
                                               elec_w_offset); // for calibration of fv, fw, w_offset and correction tables
            if (elec_sorter->scalefactors_calibrator && elec_command >= 2) {
                if (elec_sorter->scalefactors_calibrator->map_is_full_enough()) break;
            }
            if (elec_sorter->use_HEX && fill_histograms)
                Helec_xy_dev->Fill(elec_sorter->scalefactors_calibrator->binx -
                                   elec_sorter->scalefactors_calibrator->detector_map_size / 2,
                                   elec_sorter->scalefactors_calibrator->biny -
                                   elec_sorter->scalefactors_calibrator->detector_map_size / 2,
                                   elec_sorter->scalefactors_calibrator->detector_map_devi_fill);

            if (count[elec_sorter->Cu1] > 0 && count[elec_sorter->Cu2] > 0) {
                if (count[elec_sorter->Cv1] > 0 && count[elec_sorter->Cv2] > 0) {
                    double u = elec_sorter->fu * (tdc_ns[elec_sorter->Cu1][0] - tdc_ns[elec_sorter->Cu2][0]);
                    double v = elec_sorter->fv * (tdc_ns[elec_sorter->Cv1][0] - tdc_ns[elec_sorter->Cv2][0]);
                    double y = (u - 2. * v) * 0.577350269; // 0.557 = 1/sqrt(3)
                    if (fill_histograms) Helec_xy_raw->Fill(u, y);
                }
            }

            if (count[elec_sorter->Cu1] > 0 && count[elec_sorter->Cu2] > 0) {
                if (fill_histograms) Helec_u->Fill(tdc_ns[elec_sorter->Cu1][0] - tdc_ns[elec_sorter->Cu2][0]);
                double mcp = 0.;
                if (elec_sorter->use_MCP) {
                    if (count[elec_sorter->Cmcp] > 0) mcp = tdc_ns[elec_sorter->Cmcp][0]; else mcp = -1.e100;
                }
                if (fill_histograms)
                    Helec_sum_u->Fill(tdc_ns[elec_sorter->Cu1][0] + tdc_ns[elec_sorter->Cu2][0] - 2 * mcp);
            }
            if (count[elec_sorter->Cv1] > 0 && count[elec_sorter->Cv2] > 0) {
                if (fill_histograms) Helec_v->Fill(tdc_ns[elec_sorter->Cv1][0] - tdc_ns[elec_sorter->Cv2][0]);
                double mcp = 0.;
                if (elec_sorter->use_MCP) {
                    if (count[elec_sorter->Cmcp] > 0) mcp = tdc_ns[elec_sorter->Cmcp][0]; else mcp = -1.e100;
                }
                if (fill_histograms)
                    Helec_sum_v->Fill(tdc_ns[elec_sorter->Cv1][0] + tdc_ns[elec_sorter->Cv2][0] - 2 * mcp);
            }
            if (elec_sorter->use_HEX) {
                if (count[elec_sorter->Cw1] > 0 && count[elec_sorter->Cw2] > 0) {
                    if (fill_histograms) Helec_w->Fill(tdc_ns[elec_sorter->Cw1][0] - tdc_ns[elec_sorter->Cw2][0]);
                    double mcp = 0.;
                    if (elec_sorter->use_MCP) {
                        if (count[elec_sorter->Cmcp] > 0) mcp = tdc_ns[elec_sorter->Cmcp][0]; else mcp = -1.e100;
                    }
                    if (fill_histograms)
                        Helec_sum_w->Fill(tdc_ns[elec_sorter->Cw1][0] + tdc_ns[elec_sorter->Cw2][0] - 2 * mcp);
                }
            }
        }


        if (ion_sorter) {
            int number_of_ions = 0;
            if (ion_command == 1) {  // sort and write new file
                // sort/reconstruct the detector signals and apply the sum- and NL-correction.
                number_of_ions = ion_sorter->sort();
                // "number_of_ions" is the number of reconstructed number of particles
            } else {
                number_of_ions = ion_sorter->run_without_sorting();
            }
            for (int i = 0; i < number_of_ions; i++) {
                if (fill_histograms)
                    Hion_xy->Fill(ion_sorter->output_hit_array[i]->x, ion_sorter->output_hit_array[i]->y);
            }
        }

        if (elec_sorter) {
            int number_of_electrons = 0;
            if (elec_command == 1) {  // sort and write new file
                // sort/reconstruct the detector signals and apply the sum- and NL-correction.
                number_of_electrons = elec_sorter->sort();
                // "number_of_electrons" is the number of reconstructed number of particles
            } else {
                number_of_electrons = elec_sorter->run_without_sorting();
            }
            for (int i = 0; i < number_of_electrons; i++) {
                if (fill_histograms)
                    Helec_xy->Fill(elec_sorter->output_hit_array[i]->x, elec_sorter->output_hit_array[i]->y);
            }
        }

        if (ion_sorter && elec_sorter) {
            pRun->processEvent(*ion_sorter, *elec_sorter);
        }

        if (outfile) { // write to output file:
            if (ion_sorter) {
                // the following steps are necessary to make the new output look as the old one
                // (in respect to time offsets)

                // shift the detector signals back (note the -1 instead of the +1)
                if (ion_sorter->use_HEX)
                    ion_sorter->shift_sums(-1, ion_offset_sum_u, ion_offset_sum_v, ion_offset_sum_w);
                if (!ion_sorter->use_HEX) ion_sorter->shift_sums(-1, ion_offset_sum_u, ion_offset_sum_v);
                ion_sorter->shift_layer_w(-1, ion_w_offset);
                ion_sorter->shift_position_origin(-1, ion_pos_offset_x, ion_pos_offset_y);

                // convert the times from nanoseconds back to raw channels:
                if (ion_sorter->Cmcp > -1) {
                    for (unsigned int i = 0; i < count[ion_sorter->Cmcp]; ++i)
                        TDC[ion_sorter->Cmcp][i] = int(tdc_ns[ion_sorter->Cmcp][i] / tdcresolution);
                }
                for (unsigned int i = 0; i < count[ion_sorter->Cu1]; ++i)
                    TDC[ion_sorter->Cu1][i] = int(tdc_ns[ion_sorter->Cu1][i] / tdcresolution);
                for (unsigned int i = 0; i < count[ion_sorter->Cu2]; ++i)
                    TDC[ion_sorter->Cu2][i] = int(tdc_ns[ion_sorter->Cu2][i] / tdcresolution);
                for (unsigned int i = 0; i < count[ion_sorter->Cv1]; ++i)
                    TDC[ion_sorter->Cv1][i] = int(tdc_ns[ion_sorter->Cv1][i] / tdcresolution);
                for (unsigned int i = 0; i < count[ion_sorter->Cv2]; ++i)
                    TDC[ion_sorter->Cv2][i] = int(tdc_ns[ion_sorter->Cv2][i] / tdcresolution);
                if (ion_sorter->use_HEX) {
                    for (unsigned int i = 0; i < count[ion_sorter->Cw1]; ++i)
                        TDC[ion_sorter->Cw1][i] = int(tdc_ns[ion_sorter->Cw1][i] / tdcresolution);
                    for (unsigned int i = 0; i < count[ion_sorter->Cw2]; ++i)
                        TDC[ion_sorter->Cw2][i] = int(tdc_ns[ion_sorter->Cw2][i] / tdcresolution);
                }
            }

            if (elec_sorter) {
                // shift the detector signals back (note the -1 instead of the +1)
                if (elec_sorter->use_HEX)
                    elec_sorter->shift_sums(-1, elec_offset_sum_u, elec_offset_sum_v, elec_offset_sum_w);
                if (!elec_sorter->use_HEX) elec_sorter->shift_sums(-1, elec_offset_sum_u, elec_offset_sum_v);
                elec_sorter->shift_layer_w(-1, elec_w_offset);
                elec_sorter->shift_position_origin(-1, elec_pos_offset_x, elec_pos_offset_y);

                // convert the times from nanoseconds back to raw channels:
                if (elec_sorter->Cmcp > -1) {
                    for (unsigned int i = 0; i < count[elec_sorter->Cmcp]; ++i)
                        TDC[elec_sorter->Cmcp][i] = int(tdc_ns[elec_sorter->Cmcp][i] / tdcresolution);
                }
                for (unsigned int i = 0; i < count[elec_sorter->Cu1]; ++i)
                    TDC[elec_sorter->Cu1][i] = int(tdc_ns[elec_sorter->Cu1][i] / tdcresolution);
                for (unsigned int i = 0; i < count[elec_sorter->Cu2]; ++i)
                    TDC[elec_sorter->Cu2][i] = int(tdc_ns[elec_sorter->Cu2][i] / tdcresolution);
                for (unsigned int i = 0; i < count[elec_sorter->Cv1]; ++i)
                    TDC[elec_sorter->Cv1][i] = int(tdc_ns[elec_sorter->Cv1][i] / tdcresolution);
                for (unsigned int i = 0; i < count[elec_sorter->Cv2]; ++i)
                    TDC[elec_sorter->Cv2][i] = int(tdc_ns[elec_sorter->Cv2][i] / tdcresolution);
                if (elec_sorter->use_HEX) {
                    for (unsigned int i = 0; i < count[elec_sorter->Cw1]; ++i)
                        TDC[elec_sorter->Cw1][i] = int(tdc_ns[elec_sorter->Cw1][i] / tdcresolution);
                    for (unsigned int i = 0; i < count[elec_sorter->Cw2]; ++i)
                        TDC[elec_sorter->Cw2][i] = int(tdc_ns[elec_sorter->Cw2][i] / tdcresolution);
                }
            }

            for (unsigned int i = 0; i < LMF->number_of_channels; i++) {
                fwrite(&count[i], sizeof(int), 1, outfile);
                for (unsigned int j = 0; j < count[i]; j++) fwrite(&TDC[i][j], sizeof(int), 1, outfile);
            }
        }

    } // end of the big while loop

    printf("ok\n");

    printf("closing output file... ");
    // close the input and output file:
    if (outfile) {
        fclose(outfile);
        outfile = 0;
    }
    printf("ok\n");

    if (ion_command == 2) {
        printf("calibrating ion detector... ");
        ion_sorter->do_calibration();
        printf("ok\n");
        if (ion_sorter->scalefactors_calibrator) {
            printf("ion: Good scalefactors are:\nf_U = %lg\nf_V = %lg\nf_W = %lg\nOffset on layer W = %lg\n",
                   2. * ion_sorter->fu, 2. * ion_sorter->scalefactors_calibrator->best_fv,
                   2. * ion_sorter->scalefactors_calibrator->best_fw,
                   ion_sorter->scalefactors_calibrator->best_w_offset);
        }
    }

    if (elec_command == 2) {
        printf("calibrating elec detector... ");
        elec_sorter->do_calibration();
        printf("ok\n");
        if (elec_sorter->scalefactors_calibrator) {
            printf("elec: Good scalefactors are:\nf_U = %lg\nf_V = %lg\nf_W = %lg\nOffset on layer W = %lg\n",
                   2. * elec_sorter->fu, 2. * elec_sorter->scalefactors_calibrator->best_fv,
                   2. * elec_sorter->scalefactors_calibrator->best_fw,
                   elec_sorter->scalefactors_calibrator->best_w_offset);
        }
    }


    if (ion_command == 3) {   // generate and print correction tables for sum- and position-correction
        printf("ion: creating calibration tables...\n");
        create_calibration_tables(pRun->getIonCalibTableFilename(), ion_sorter);
        printf("\nfinished creating calibration tables\n");
    }

    if (elec_command == 3) {   // generate and print correction tables for sum- and position-correction
        printf("elec: creating calibration tables...\n");
        create_calibration_tables(pRun->getElecCalibTableFilename(), elec_sorter);
        printf("\nfinished creating calibration tables\n");
    }


    if (ion_canvas) {
        for (__int32 i = 0; i <= 9; i++) {
            ion_canvas->cd(i)->Modified(true);
            ion_canvas->cd(i)->Update();
        }
    }

    if (elec_canvas) {
        for (__int32 i = 0; i <= 9; i++) {
            elec_canvas->cd(i)->Modified(true);
            elec_canvas->cd(i)->Update();
        }
    }


    printf("hit any key to exit\n");
    while (true) {
        gSystem->Sleep(5);
        gSystem->ProcessEvents();
        if (my_kbhit()) break;
    }

    // Finish the program
    printf("terminating the root app.\n");
    clean_up1();
    if (pRun) delete pRun;
    clean_up2();
    theRootApp.Terminate();
    std::cout << "The program is done. " << std::endl;
    return 0;
}
