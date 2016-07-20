#include "SortRun.h"

bool Analysis::SortRun::isFileExist(const char *fileName) {
    std::ifstream file(fileName);
    return file.good();
}

Analysis::SortRun::~SortRun() {
    delete[] pIonDataSet;
    delete[] pElecDataSet;
    numberOfIons = 0;
    numberOfElectrons = 0;

    printf("writing root tree... ");
    if (pRootTree) pRootTree->Write();
    printf("ok\n");

    printf("writing root file... ");
    if (pRootFile) pRootFile->Write();
    printf("ok\n");

    printf("Closing root file... ");
    if (pRootFile) pRootFile->Close();
    printf("ok\n");

    if (pRootTree) delete pRootTree;
    if (pRootFile) delete pRootFile;

    // id = nullptr;
    LMFFilename = "";
    rootFilename = "";
    ionSorterFilename = "";
    elecSorterFilename = "";
    ionCalibTableFilename = "";
    elecCalibTableFilename = "";
}

Analysis::SortRun::SortRun(const std::string configFilename) {
    // Setup json file reader
    Analysis::JSONReader configReader(configFilename);

    // Change the working directory
    chdir(configReader.getStringAt("working_directory").c_str());
    LMFFilename = configReader.getStringAt("LMF_file");
    ionSorterFilename = configReader.getStringAt("ion_sorter");
    elecSorterFilename = configReader.getStringAt("electron_sorter");
    ionCalibTableFilename = configReader.getStringAt("ion_calibration_table");
    elecCalibTableFilename = configReader.getStringAt("electron_calibration_table");

    // Create id
    pRootTree = new TTree("resortedData", "Resorted Data");
    for (int i = 0; i < 10000; i++) {
        sprintf(id, "%04d", i);
        rootFilename = "resortLess";
        rootFilename += id;
        rootFilename += ".root";
        if (isFileExist(rootFilename.c_str())) {
            continue;
        }
        break;
    }
    pRootFile = new TFile(rootFilename.c_str(), "new");

    // Setup ions and electrons
    numberOfIons = 0;
    numberOfElectrons = 0;
}

void Analysis::SortRun::processEvent(const sort_class &ionSort, const sort_class &elecSort) {
    for (int i = 0; i < numberOfIons; i++) {
        pIonDataSet[i].x = ionSort.output_hit_array[i]->x;
        pIonDataSet[i].y = ionSort.output_hit_array[i]->y;
        pIonDataSet[i].t = ionSort.output_hit_array[i]->time;
        pIonDataSet[i].flag = ionSort.output_hit_array[i]->method;
    }
    for (int i = 0; i < numberOfElectrons; i++) {
        pElecDataSet[i].x = elecSort.output_hit_array[i]->x;
        pElecDataSet[i].y = elecSort.output_hit_array[i]->y;
        pElecDataSet[i].t = elecSort.output_hit_array[i]->time;
        pElecDataSet[i].flag = elecSort.output_hit_array[i]->method;
    }
    pRootTree->Fill();
}

void Analysis::SortRun::branchRootTree(const int ionNum, const int elecNum) {
    std::string str;

    // Ion setup
    str = "Ion";
    numberOfIons = ionNum;
    pIonDataSet = new DataSet[numberOfIons];
    for (int i = 0; i < numberOfIons; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        pRootTree->Branch((str + "X" + ch).c_str(), &pIonDataSet[i].x, (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &pIonDataSet[i].y, (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &pIonDataSet[i].t, (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &pIonDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
    }

    // Electron setup
    str = "Elec";
    numberOfElectrons = elecNum;
    pElecDataSet = new DataSet[numberOfIons];
    for (int i = 0; i < numberOfElectrons; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        pRootTree->Branch((str + "X" + ch).c_str(), &pElecDataSet[i].x, (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &pElecDataSet[i].y, (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &pElecDataSet[i].t, (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &pElecDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
    }
}

char *Analysis::SortRun::getIonSorterFilename() const {
    return (char *) ionSorterFilename.c_str();
}

char *Analysis::SortRun::getElecSorterFilename() const {
    return (char *) elecSorterFilename.c_str();
}

char *Analysis::SortRun::getIonCalibTableFilename() const {
    return (char *) ionCalibTableFilename.c_str();
}

char *Analysis::SortRun::getElecCalibTableFilename() const {
    return (char *) elecCalibTableFilename.c_str();
}

char *Analysis::SortRun::getLMFFilename() const {
    return (char *) LMFFilename.c_str();
}

TFile *Analysis::SortRun::getRootFile() {
    return pRootFile;
}

TFile *Analysis::SortRun::RecreateRootFile(char *name, char *comment) {
    TFile *m_RootFile = new TFile(name, "RECREATE", comment);
    return m_RootFile;
}

TCanvas *Analysis::SortRun::newCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy) {
    TCanvas *canvaspointer;
    canvaspointer = new TCanvas(name, titel, xposition, yposition, pixelsx, pixelsy);
    return canvaspointer;
}

void Analysis::SortRun::SetGreyScale(int number_of_contours) {
    //remove every color above 228 because otherwise this function cannot be called very often//
    TColor *color;
    Int_t highestIndex = 228;

    TIter next(gROOT->GetListOfColors());
    TSeqCollection *coltab = gROOT->GetListOfColors();
    while ((color = (TColor *) next())) {
        if (color->GetNumber() > highestIndex) {
            coltab->Remove(color);
            delete color;
        }
    }

    //grayscale
    UInt_t Number = 2;
    Double_t Red[2] = {0.85, 0.15};
    Double_t Green[2] = {0.85, 0.15};
    Double_t Blue[2] = {0.85, 0.15};
    Double_t Stops[2] = {0.0, 1.0};
    TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, number_of_contours);
    gStyle->SetNumberContours(number_of_contours);
}

void Analysis::SortRun::color(int color_palette_id, int number_of_contours) {
    //remove every color above 228 because otherwise this function cannot be called very often//
    TColor *color;
    Int_t highestIndex = 228;

    TIter next(gROOT->GetListOfColors());
    TSeqCollection *coltab = gROOT->GetListOfColors();
    while ((color = (TColor *) next())) {
        if (color->GetNumber() > highestIndex) {
            coltab->Remove(color);
            delete color;
            color = 0;
        }
    }

    if (color_palette_id == 0) {
        //grayscale
        UInt_t Number = 2;
        Double_t Red[2] = {0.85, 0.15};
        Double_t Green[2] = {0.85, 0.15};
        Double_t Blue[2] = {0.85, 0.15};
        Double_t Stops[2] = {0.0, 1.0};
        TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, number_of_contours);
        gStyle->SetNumberContours(number_of_contours);
    }

    if (color_palette_id == 1) {
        //standart cobold palette
        UInt_t Number = 7;
        Double_t Red[7] = {0.00, 0.00, 0.00, 0.00, 0.65, 0.99, 0.99};
        Double_t Green[7] = {0.00, 0.00, 0.75, 0.99, 0.00, 0.00, 0.99};
        Double_t Blue[7] = {0.33, 0.99, 0.75, 0.00, 0.65, 0.00, 0.00};
        Double_t Stops[7] = {0.00, 0.10, 0.30, 0.45, 0.70, 0.80, 1.00};
        TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, number_of_contours);
        gStyle->SetNumberContours(number_of_contours);
    }

    if (color_palette_id == 2) {
        //tills palette
        UInt_t Number = 4;
        Double_t Red[4] = {0.50, 0.00, 0.99, 0.99};
        Double_t Green[4] = {0.99, 0.00, 0.00, 0.99};
        Double_t Blue[4] = {0.99, 0.99, 0.00, 0.00};
        Double_t Stops[4] = {0.00, 0.30, 0.70, 1.00};
        TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, number_of_contours);
        gStyle->SetNumberContours(number_of_contours);

    }

    if (color_palette_id == 3) {
        //macieks palette
        UInt_t Number = 6;
        Double_t Red[6] = {0.55, 0.00, 0.00, 0.00, 0.99, 0.99};
        Double_t Green[6] = {0.00, 0.00, 0.99, 0.99, 0.99, 0.00};
        Double_t Blue[6] = {0.99, 0.99, 0.99, 0.00, 0.00, 0.00};
        Double_t Stops[6] = {0.00, 0.15, 0.35, 0.57, 0.77, 1.00};
        TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, number_of_contours);
        gStyle->SetNumberContours(number_of_contours);
    }
}

TF1 *Analysis::SortRun::newTF1(char *a, char *b, double aa, double bb) {
    return new TF1(a, b, aa, bb);
}

TH1D *Analysis::SortRun::newTH1D(int number, char *comment, int bins, double xmin, double xmax) {
    char name[200];
    TH1D *hist;
    sprintf(name, "%i", number);
    hist = new TH1D(name, comment, bins, xmin, xmax);
    return hist;
}

TH1D *Analysis::SortRun::newTH1D(char *name, char *comment, int bins, double xmin, double xmax) {
    TH1D *hist;
    hist = new TH1D(name, comment, bins, xmin, xmax);
    return hist;
}

TH2D *Analysis::SortRun::newTH2D(int number, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
    char name[200];
    TH2D *hist;
    sprintf(name, "%i", number);
    hist = new TH2D(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
    hist->SetOption(option);
    return hist;
}

TH2D *Analysis::SortRun::newTH2D(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
    TH2D *hist;
    hist = new TH2D(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
    hist->SetOption(option);
    return hist;
}

TH2F *Analysis::SortRun::newTH2F(int number, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
    char name[200];
    TH2F *hist;
    sprintf(name, "%i", number);
    hist = new TH2F(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
    hist->SetOption(option);
    return hist;
}

TH2F *Analysis::SortRun::newTH2F(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
    TH2F *hist;
    hist = new TH2F(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
    hist->SetOption(option);
    return hist;
}

TH2I *Analysis::SortRun::newTH2I(int number, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
    char name[200];
    TH2I *hist;
    sprintf(name, "%i", number);
    hist = new TH2I(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
    hist->SetOption(option);
    return hist;
}

TH2I *Analysis::SortRun::newTH2I(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
    TH2I *hist;
    hist = new TH2I(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
    hist->SetOption(option);
    return hist;
}

TH3D *Analysis::SortRun::newTH3D(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, int zbins, double zmin, double zmax, char *option) {
    TH3D *hist;
    hist = new TH3D(name, comment, xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    hist->SetOption(option);
    return hist;
}

TTree *Analysis::SortRun::newTTree(char *arg1, char *arg2) {
    TTree *t = new TTree(arg1, arg2);
    return t;
}
