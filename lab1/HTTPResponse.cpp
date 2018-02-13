// #include <iostream>
// #include <stdint.h>
// #include <string.h>
// #include <vector>
// #include <map>
// Response.
#include "HTTPResponse.h"
#include "HTTPRequest.h"

#include <bits/stdc++.h>

// using namespace std;

std::vector<std::string> splitLines2(const std::string& s);
std::vector<std::string> splitWords2(const std::string& s);
map<string, string> parseHeader(vector<string> lines);

HTTPResponse::HTTPResponse(string statusCode, string location, vector<uint8_t> content) {
  _statusCode = statusCode;
  _location = location;
  _content = content;

  _time = std::chrono::system_clock::now();
}

// std::string getCode(ResponseType type) {
//   string ans;

//   // switch (type) {
//   //   case Created:               ans = "201  Created";
//   //   case Accepted:              ans = "202  Accepted";
//   //   case No_Content:            ans = "204  No Content";
//   //   case Moved_Permanently:     ans = "301  Moved Permanently";
//   //   case Moved_Temporarily:     ans = "302  Moved Temporarily";
//   //   case Not_Modified:          ans = "304  Not Modified";
//   //   case Bad_Request:           ans = "400  Bad Request";
//   //   case Unauthorized:          ans = "401  Unauthorized";
//   //   case Forbidden:             ans = "403  Forbidden";
//   //   case Not_Found:             ans = "404  Not Found";
//   //   case Internal_Server_Error: ans = "500  Internal Server Error";
//   //   case Not_Implemented:       ans = "501  Not Implemented";
//   //   case Bad_Gateway:           ans = "502  Bad Gateway";
//   //   case Service_Unavailable:   ans = "503  Service Unavailable";
//   // }

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
  vector<string> lines = splitLines2(s);

  vector<string> l1 = splitWords2(lines[0]);
  const string get = "GET";

  if (l1.size() < 2 ||  l1[0] != "GET") {
    // Invalid request..
  }

  string path = l1[1];

  auto params = parseHeader(lines);

  cout << "path: " << path << endl;
  cout << "host: " << params["Host:"] << endl;

  // return HTTPResponse(path, params["Host:"], "");
}

map<string, string> parseHeader(vector<string> lines) {
  // Parse all other parameters.
  std::map<string, string> params;

  for (int i = 1; i < (int)lines.size(); i++) {
    vector<string> li = splitWords2(lines[i]);

    if (li.size() >= 2) {
      params[li[0]] = li[1];
    }
  }

  return params;
}

// Split string into lines.
std::vector<std::string> splitLines2(const std::string& s) {
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
std::vector<std::string> splitWords2(const std::string& s) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, ' '))
  {
    tokens.push_back(token);
  }
  return tokens;
}
