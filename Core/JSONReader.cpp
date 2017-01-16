#include "JSONReader.h"

const int Analysis::JSONReader::getIntAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = getOptValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsNumber());
	assert(pV->IsInt());
	return pV->GetInt();
}
const double Analysis::JSONReader::getDoubleAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = getOptValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsNumber());
	return pV->GetDouble();
}
const bool Analysis::JSONReader::getBoolAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = getOptValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsBool());
	return pV->GetBool();
}
const std::string Analysis::JSONReader::getStringAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = getOptValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsString());
	return pV->GetString();
}
const int Analysis::JSONReader::getListSizeAt(const std::string str) const {
	const rapidjson::Value *pV = getOptValue(str);
	if(pV->IsArray()) {
		return pV->Size();
	} else {
		return -1;
	}
}
bool Analysis::JSONReader::getBoolAtIfItIs(const std::string str, const bool def) const {
  if (hasMember(str)) return getBoolAt(str);
  else return def;
}
void Analysis::JSONReader::ReadFromStr(const std::string str) {
  auto *pNewDoc = new rapidjson::Document;
  pNewDoc->Parse<parseFlags>(str.c_str());
  if(pNewDoc->HasParseError()) throw std::invalid_argument("It has parse error!");
  pDocs.insert(pDocs.begin(), pNewDoc);
}
void Analysis::JSONReader::ReadFromFile(const std::string filename) {
  if(vvv) std::cout << "Reading JSON file: " << filename << std::endl;
  std::fstream fileStream;
  fileStream.open(filename, std::fstream::in);
  if(!fileStream.is_open()) throw std::invalid_argument("The JSON file does not exist!");
  std::stringstream stringStream;
  stringStream << fileStream.rdbuf();
  ReadFromStr(stringStream.str());
  fileStream.close();
}
bool Analysis::JSONReader::hasMember(std::string str) const {
  auto pV = getOptValue(str);
  return pV != nullptr;
}
const rapidjson::Value *Analysis::JSONReader::_getOptValue(const std::string str1,
                                                           const rapidjson::Value *v,
                                                           const std::string str0) const {
  if (str1=="") return v;
  std::size_t found;
  std::string tmp0, tmp1;
  found = str1.find(".");
  if (found == std::string::npos) {
    tmp0 = str1;
    tmp1 = "";
  } else {
    tmp0 = str1.substr(0, found);
    tmp1 = str1.substr(found + 1);
  }
  if (isdigit(tmp0[0])) {
    if (!(v->IsArray())) {
      if(vvv) std::cout << "Member " << str0 << " is not array!" << std::endl;
      return nullptr;
    }
    int i = std::stoi(tmp0);
    return _getOptValue(tmp1, &(*v)[i], str0 + "." + tmp0);
  } else {
    if (!(v->HasMember(tmp0.c_str()))) {
      if(vvv) std::cout << "Member " << str0+tmp0 << " does not exist!" << std::endl;
      return nullptr;
    }
    return _getOptValue(tmp1, &(*v)[tmp0.c_str()], str0 + "." + tmp0);
  }
}
void Analysis::JSONReader::appendDoc(const Analysis::JSONReader::ReadingType type, const std::string str) {
  if(type==fromFile) ReadFromFile(str);
  else if(type==fromStr) ReadFromStr(str);
  else throw std::invalid_argument("Invalid type! It must be one of fromFile or fromStr!");
}
const rapidjson::Value *Analysis::JSONReader::getOptValue(const std::string str1) const {
  for(auto pDoc: pDocs) {
    auto pV = _getOptValue(str1, pDoc);
    if(pV!=nullptr) return pV;
  }
  return nullptr;
}
Analysis::JSONReader::~JSONReader() {
  for(auto pDoc: pDocs) {
    delete pDoc;
  }
}
Analysis::JSONReader::JSONReader(const bool v): vvv(v) {
}
