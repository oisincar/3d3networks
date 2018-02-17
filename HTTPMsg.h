#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#ifndef HTTPMSG_H
#define HTTPMSG_H

class HTTPMsg {
 private:
  std::string _path;

 protected:
  static std::map<std::string, std::string> parseHeader(std::vector<std::string> lines);

  static std::vector<std::string> splitLines(const std::string& s);
  static std::vector<std::string> splitWords(const std::string& s);

 public:
  virtual std::vector<uint8_t> encode();
};

#endif
