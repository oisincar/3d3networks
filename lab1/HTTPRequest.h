#include <iostream>
#include <stdint.h>
#include <string.h>
#include <vector>
using namespace std;

class HTTPRequest{
 private:
  string _message;
  string _URL;
  string _path;

 public:
  HTTPRequest(string path, string url, string msg);
  vector<uint8_t> encode();
  HTTPRequest consume(vector<uint8_t> ss);
};
