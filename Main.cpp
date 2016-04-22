//
// Created by Daehyun You on 11/29/15.
//
#include <iostream>
#include <TChain.h>
#include "Core/JSONReader.h"
#include "Run.h"

int main(int argc, char * argv[]) {

  // set seed for random
  srand((unsigned int) time(nullptr));

  std::cout << "Hello world!" << std::endl;
  std::cout << "The exe file which place at " << argv[0] << ", is running now. " << std::endl;
  std::cout << "The configure file which place at " << argv[1] << ", is going to be read. " << std::endl;

  // Setup reader & writer
  Analysis::Run run(argv[1]);
  return 0;
}
