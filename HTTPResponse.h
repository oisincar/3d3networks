#include <iostream>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <ctime>
#include "HTTPMsg.h"

using namespace std;

enum ResponseType {
     Created
   , Accepted
   , No_Content
   , Moved_Permanently
   , Moved_Temporarily
   , Not_Modified
   , Bad_Request
   , Unauthorized
   , Forbidden
   , Not_Found
   , Internal_Server_Error
   , Not_Implemented
   , Bad_Gateway
   , Service_Unavailable
};

class HTTPResponse : HTTPMsg {
 private:
  // ResponseType _statusCode;
  string _statusCode;
  string _location;

  vector<uint8_t> _content;
  std::chrono::_V2::system_clock::time_point _time;

 public:

  HTTPResponse(string statusCode, string location, vector<uint8_t> content);
  HTTPResponse(vector<uint8_t> ss);
  vector<uint8_t> encode();
  HTTPResponse consume(vector<uint8_t> ss);
};
