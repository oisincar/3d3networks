#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "HTTPMsg.h"

std::map<std::string, std::string> HTTPMsg::parseHeader(std::vector<std::string> lines) {
  std::map<std::string, std::string> params;

  for (int i = 0; i < (int)lines.size(); i++) {
    std::vector<std::string> li = splitWords(lines[i]);

    if (li.size() >= 2) {
      params[li[0]] = li[1];
    }
  }

  return params;
}

// Split string into lines.
std::vector<std::string> HTTPMsg::splitLines(const std::string& s) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token))
  {
    tokens.push_back(token);
  }
  return tokens;
}

// Split string into words.
std::vector<std::string> HTTPMsg::splitWords(const std::string& s) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, ' '))
  {
    tokens.push_back(token);
  }
  return tokens;
}

// Virtual func.
std::vector<uint8_t> HTTPMsg::encode() {
  std::vector<uint8_t> a;
  return a;
}
