#include "../Core/JSONReader.h"

int main() {
  const char *defaultJSON = "{\n"
          "    \"hello\": \"world\",\n"
          "    \"bool\": false,\n"
          "    \"i\": 123,\n"
          "    \"pi\": 3.1416,\n"
          "    \"inf\": Inf,\n"
          "    \"nan\": NaN,\n"
          "    \"arr\": [1, 2, 3, 4],\n"
          "    \"obj\": {\"i\": 456, \"j\": 789}\n"
          "}";
  const char *userJSON = "{\n"
          "    \"hello\": \"it's me!\",\n"
          "    \"bool\": true,\n"
          "    \"1st\": \"key can include num\"\n"
          "}";

  Analysis::JSONReader reader;
  reader.appendDoc(Analysis::JSONReader::fromStr, userJSON);
  reader.appendDoc(Analysis::JSONReader::fromStr, defaultJSON);

    std::cout << "hello: " << reader.get<const char *>("hello") << std::endl;
    std::cout << "overwritten value: " << (reader.get<bool>("bool") ? "true" : "false") << std::endl;
    std::cout << "int: " << reader.get<int>("i") << std::endl;
    std::cout << "pi: " << reader.get<double>("pi") << std::endl;
    std::cout << "double Inf: " << reader.get<double>("inf") << std::endl;
    std::cout << "double NaN: " << reader.get<double>("nan") << std::endl;
    std::cout << "1st elem of arr: " << reader.get<int>("arr.0") << std::endl;
    std::cout << "arr size: " << reader.getArrSize("arr") << std::endl;
    std::cout << "read arr: " << std::endl;
    auto a = reader.getArr<int>("arr");
    for (auto e: a) std::cout << "  " << e << std::endl;
    std::cout << "use dot btw objs: " << reader.get<int>("obj.i") << std::endl;
    std::cout << "read map: " << std::endl;
    auto m = reader.getMap<int>("obj");
    for (auto &e: m) std::cout << "  " << e.first << ": " << e.second << std::endl;
    std::cout << "1st ch of: " << reader.get<const char *>("1st") << std::endl;
  return 0;
}