//
// Created by Daehyun You on 11/29/15.
//
// #define ANALYSIS_DEBUG_BUILD

#include <iostream>
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

int main(int argc, char * argv[]) {

  // set seed for random
  srand((unsigned int) time(nullptr));

  std::cout << "Hello world!" << std::endl;
  std::cout << "The exe file which place at " << argv[0] << ", is running now. " << std::endl;
  std::cout << "The configure file which place at " << argv[1] << ", is going to be read. " << std::endl;

  // Setup reader & writer
  Analysis::Run run(argv[1]);
  int currentPercentage = -1;
  const int totalEntries = (int) run.getEntries();
  for (int i=0; i<totalEntries; i++) {
#ifdef ANALYSIS_DEBUG_BUILD
    if(i==100) {
      break;
    }
#endif
    if(currentPercentage/100 < i/totalEntries) {
      currentPercentage++;
      showProgressBar(currentPercentage / 100);
    }
    run.processEvent((size_t) i);
    // TODO: Add function reading commands JOT
  }
  return 0;
}
