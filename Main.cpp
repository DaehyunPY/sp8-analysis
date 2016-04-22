//
// Created by Daehyun You on 11/29/15.
//
#include <iostream>
#include "Core/JSONReader.h"

int main(int argc, char * argv[]) {
  std::cout << "Hello world!" << std::endl;
  std::cout << argv[1] << std::endl;
  Analysis::JSONReader reader(argv[1]);
  std::cout << (reader.getFlag().hasNoParseError() ? "true" : "false") << std::endl;
  return 0;
}
