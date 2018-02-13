#include <iostream>
#include <stdint.h>
#include <string.h>
#include <vector>
using namespace std;

class HTTPRequest {
 private:
  string _URL;
  string _path;

 public:
  HTTPRequest(string url, string msg);
  HTTPRequest(vector<uint8_t> ss);

  string getUrl() { return _URL; }
  string getPath() { return _path; }

  vector<uint8_t> encode();
};
