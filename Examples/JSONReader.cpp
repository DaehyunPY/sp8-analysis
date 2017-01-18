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
  reader.appendDoc(Analysis::JSONReader::fromStr, userJSON);
  reader.appendDoc(Analysis::JSONReader::fromStr, defaultJSON);
  std::cout << reader.get<const char*>("hello") << std::endl;
  std::cout << (reader.get<bool>("b")? "true":"false") << std::endl;
  std::cout << reader.get<int>("i") << std::endl;
  std::cout << reader.get<double>("pi") << std::endl;
  std::cout << reader.get<double>("inf") << std::endl;
  std::cout << reader.get<double>("nan") << std::endl;
  std::cout << reader.get<int>("a.0") << std::endl;
  auto a = reader.getArr<int>("a");
  std::cout << a[0] << std::endl;
  std::cout << reader.getArrSize("a") << std::endl;
  std::cout << reader.get<int>("o.i") << std::endl;
  auto m = reader.getMap<int>("o");
  std::cout << m["i"] << std::endl;
  auto keys = reader.getMapKeys("o");
  std::cout << keys[0] << std::endl;
  return 0;
}