//
// Created by Daehyun You on 11/29/15.
//
// #define ANALYSIS_DEBUG_BUILD

#include <iostream>
#include <thread>
#include "AnalysisRun.h"

void showProgressBar(const float prog = 0) {
  const int width = 50; // characters
  std::cout << "[";
  int pos = (int) (width * prog);
  for (int i = 0; i < width; ++i) {
    if (i < pos) {
      std::cout << "=";
    } else if (i == pos) {
      std::cout << ">";
    } else {
      std::cout << " ";
    }
  }
  std::cout << "] " << (int) (100 * prog) << "%\r";
  std::cout.flush();
}

enum StatusInfo {
  keepRunning,
  quitProgramSafely,
  done
};

void inputManager(StatusInfo &info) {
  std::string input;
  if (info != keepRunning) { return; }
  while (std::cin) {
    std::getline(std::cin, input);
    if (input.empty()) {
      continue;
    }
    std::cout << "typed: " << input << std::endl;
    if (input.compare("quit") == 0 && info == keepRunning) {
      info = quitProgramSafely;
    }
  }
  std::cout << "Our work here is done." << std::endl;
}

int main(int argc, char *argv[]) {
  // Inform status
  if (argc < 2) {
    printf("syntax: AnalysisExe filename\n");
    printf("Please provide a filename.\n");
    return 0;
  }
  if (argc > 2) {
    printf("syntax: AnalysisExe filename\n");
    printf("too many arguments\n");
    return 0;
  }
  std::cout << "arg 0: `" << argv[0] << "' is running now. " << std::endl;
  std::cout << "arg 1: `" << argv[1] << "' is going to be read for config file. " << std::endl;
  srand((unsigned int) time(nullptr));

  // Setup to run
  Analysis::AnalysisRun *pRun = nullptr;

  // Setup json file reader
  Analysis::JSONReader *pReader = nullptr;
  std::cout << "opening the config file... ";
  pReader = new Analysis::JSONReader();
  pReader->appendDoc(Analysis::JSONReader::fromFile, argv[1]);
  std::cout << "okay" << std::endl;

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

  // divid output files
  pRun = new Analysis::AnalysisRun(*pReader);
  const auto totalEntries = pRun->getEntries();
  std::cout << "         total entries: " << totalEntries << std::endl;
  const auto limitEnt = pReader->getIntAt("setup_output.limitation_of_entries");
  std::cout << " limitation of entries: " << limitEnt << std::endl;
  const auto remainder = (int) (totalEntries % limitEnt);
  auto numFiles = (int) (totalEntries / limitEnt);
  if (remainder != 0) numFiles += 1;
  if (pReader->getBoolAt("setup_output.finish_after_filing_single_file")) numFiles = 1;
  std::cout << "number of output files: " << numFiles << std::endl;

  // Make input thread
  std::cout << "make a thread to read keyboard hit... ";
  StatusInfo statusInfo = keepRunning;
  std::thread threadForInput(inputManager, std::ref(statusInfo));
  std::cout << "okay" << std::endl;
  std::cout << "To quit this program safely, input 'quit'. " << std::endl;

  // Run processes
  int currentPercentage = -1;
  for (int k = 0; k < numFiles; k++) {
    if (statusInfo == quitProgramSafely) break;
    if (k != 0) pRun = new Analysis::AnalysisRun(*pReader);

    for (int j = 0; j < limitEnt; j++) {
      const long i = k * ((long) limitEnt) + j;
      if (statusInfo == quitProgramSafely) break;
      if (i >= totalEntries) break;
      if (currentPercentage / 100.0 < i / (double) totalEntries) {
        currentPercentage++;
        showProgressBar((const float) (currentPercentage / 100.0));
      }
      pRun->processEvent(i);
    }
    delete pRun;
    pRun = nullptr;
  }
  if (pRun) delete pRun;
  delete pReader;

  // Finish the program
  std::cout << "closing the program... ";
  statusInfo = done;
  threadForInput.detach();
  threadForInput.~thread();
  std::cout << "okay" << std::endl;
  return 0;
}
