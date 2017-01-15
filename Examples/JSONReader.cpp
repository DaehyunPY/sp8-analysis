#include "../Core/JSONReader.h"

int main() {
  const char *defaultJSON = "{\n"
      "    \"hello\": \"world\",\n"
      "    \"b\": true,\n"
      "    \"n\": null,\n"
      "    \"i\": 123,\n"
      "    \"pi\": 3.1416,\n"
      "    \"inf\": Inf,\n"
      "    \"nan\": NaN,\n"
      "    \"a\": [1, 2, 3, 4],\n"
      "    \"o\": {\"i\": 456},\n"
      "}";
  const char *userJSON = "{\n"
      "    \"hello\": \"it's me!\",\n"
      "    \"b\": false,\n"
      "}";
  Analysis::JSONReader reader;
  reader.appendDoc(Analysis::JSONReader::fromStr, defaultJSON);
  reader.appendDoc(Analysis::JSONReader::fromStr, userJSON);
  std::cout << *reader.getOpt<const char*>("hello") << std::endl;
  std::cout << (*reader.getOpt<bool>("b")? "true":"false") << std::endl;
  std::cout << *reader.getOpt<int>("i") << std::endl;
  std::cout << *reader.getOpt<double>("pi") << std::endl;
  std::cout << *reader.getOpt<double>("inf") << std::endl;
  std::cout << *reader.getOpt<double>("nan") << std::endl;
  std::cout << *reader.getOpt<int>("a.1") << std::endl;
  std::cout << *reader.getOpt<int>("o.i") << std::endl;
  return 0;
}