// #include <iostream>
// #include <stdint.h>
// #include <string.h>
// #include <vector>
// #include <map>
// Response.
#include "HTTPResponse.h"

#include <bits/stdc++.h>

HTTPResponse::HTTPResponse(string statusCode, string location, vector<uint8_t> content) {
  _statusCode = statusCode;
  _location = location;
  _content = content;

  _time = std::chrono::system_clock::now();
}

void HTTPResponse::setStatusCode(string statusCode){
  _statusCode = statusCode;
}

string HTTPResponse::getStatusCode(){
  return _statusCode;
}

void HTTPResponse:: setLocation(string location){
  _location = location;
}

// std::string getCode(ResponseType type) {
//   string ans;

//   switch (type) {
//     case Created:               ans = "201  Created";
//     case Accepted:              ans = "202  Accepted";
//     case No_Content:            ans = "204  No Content";
//     case Moved_Permanently:     ans = "301  Moved Permanently";
//     case Moved_Temporarily:     ans = "302  Moved Temporarily";
//     case Not_Modified:          ans = "304  Not Modified";
//     case Bad_Request:           ans = "400  Bad Request";
//     case Unauthorized:          ans = "401  Unauthorized";
//     case Forbidden:             ans = "403  Forbidden";
//     case Not_Found:             ans = "404  Not Found";
//     case Internal_Server_Error: ans = "500  Internal Server Error";
//     case Not_Implemented:       ans = "501  Not Implemented";
//     case Bad_Gateway:           ans = "502  Bad Gateway";
//     case Service_Unavailable:   ans = "503  Service Unavailable";
//   }

//   return ans;
// }

vector<uint8_t> HTTPResponse:: encode(){
  string header =
      "HTTP/1.0 " + _statusCode + "\r\n" +
      "Location: " + _location + "\r\n" +
      "Date: " + "\r\n" +
      "Server: gws" + "\r\n" +
      "Content-Length: " + to_string(_content.size()) + "\r\n" +
      "\r\n";

  // Possibly doesn't end in 0?
  vector<uint8_t> vec(header.begin(), header.end());
  return vec;
}

HTTPResponse::HTTPResponse(vector<uint8_t> ss) {
  // Split into 2 lines.
  string s(ss.begin(),ss.end());
  vector<string> lines = HTTPMsg::splitLines(s);

  vector<string> l1 = splitWords(lines[0]);
  const string get = "GET";

  if (l1.size() < 2 ||  l1[0] != "GET") {
    // Invalid request..
  }

  string path = l1[1];

  // Header starts from line 1, until the first \r\n\r\n.
  // Should be a blank line actually when split...
  auto enditt = find(lines.begin()+1, lines.end(), "");
  auto params = parseHeader(vector<string>(lines.begin()+1, enditt));

  cout << "path: " << path << endl;
  cout << "host: " << params["Host:"] << endl;
}
