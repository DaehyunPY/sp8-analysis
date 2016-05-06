//
// Created by Daehyun You on 11/29/15.
//
// #define ANALYSIS_DEBUG_BUILD

#include <iostream>
#include <thread>

#include "Run.h"

void showProgressBar(const float prog = 0) {
  const int width = 50; // characters
  std::cout << "[";
  int pos = (int) (width * prog);
  for (int i = 0; i < width; ++i) {
    if(i < pos) {
      std::cout << "=";
    } else if(i == pos) {
      std::cout << ">";
    } else {
      std::cout << " ";
    }
  }
  std::cout << "] " << (int) (100*prog) << "%\r";
  std::cout.flush();
}
enum StatusInfo {
  keepRunning,
  quitProgramSafely,
  done 
};
void inputManager(StatusInfo &info) {
  std::string input;
  if(info != keepRunning) { return; }
  while (std::cin) {
    std::cout << "Type something in:" << std::endl;
    std::getline(std::cin, input);
    if(input.empty()){
      continue;
    }
    std::cout << "You typed [" << input << "]" << std::endl;
    if(input=="q" && info == keepRunning) {
      info = quitProgramSafely;
    }
  }
  std::cout << "Our work here is done." << std::endl;
}
int main(int argc, char * argv[]) {
  // Inform
  std::cout << "The exe file which place at " << argv[0] << ", is running now. " << std::endl;
  std::cout << "The configure file which place at " << argv[1] << ", is going to be read. " << std::endl;
  std::cout << "To quit this program safely, input 'q'. " << std::endl;

  // Make input thread
  StatusInfo statusInfo = keepRunning;
  std::thread threadForInput(inputManager, std::ref(statusInfo));

  // Setup to run
  srand((unsigned int) time(nullptr));
  int currentPercentage = -1;
  Analysis::Run run(argv[1]);
  const long totalEntries = run.getEntries();

  // Run processes
  for (long i=0; i<totalEntries; i++) {
#ifdef ANALYSIS_DEBUG_BUILD
    if(i==100) {
      break;
    }
#endif
    // Show the process bar
    if(currentPercentage/100.0 < i/(double)totalEntries) {
      currentPercentage++;
      showProgressBar((const float) (currentPercentage/100.0));
    }
    // Check input
    if(statusInfo == quitProgramSafely) {
      break;
    }
    run.processEvent(i);
  }

  // Finish the program
  statusInfo = done;
  if(threadForInput.joinable()) {
    threadForInput.join();
  } else {
    threadForInput.~thread();
  }

  return 0;
}
