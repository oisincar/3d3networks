// #include <iostream>
// #include <stdint.h>
// #include <string.h>
// #include <vector>
// #include <map>
#include "HTTPRequest.h"

#include <bits/stdc++.h>

using namespace std;

std::vector<std::string> splitLines(const std::string& s);
std::vector<std::string> splitWords(const std::string& s);

HTTPRequest:: HTTPRequest (string path, string url, string msg) {
  _path = path;
  _URL = url;
  _message = msg;
}

vector<uint8_t> HTTPRequest:: encode(){
  string header = "GET /" + _path +" HTTP/1.0\r\nHost: " + _URL + "\r\n\r\n";

  // Possibly doesn't end in 0?
  vector<uint8_t> vec(header.begin(),header.end());
  return vec;
}

HTTPRequest HTTPRequest:: consume(vector<uint8_t> ss){
  // Split into 2 lines.
  string s(ss.begin(),ss.end());
  vector<string> lines = splitLines(s);

  vector<string> l1 = splitWords(lines[0]);
  const string get = "GET";
  if (l1.size() < 2 ||  l1[0] != "GET") {
    // Invalid request..
  }

  string path = l1[1];


  // Parse all other parameters.
  map<string, string> params;

  for (int i = 1; i < lines.size(); i++) {
    vector<string> li = splitWords(lines[i]);

    if (li.size() >= 2) {
      params[li[0]] = li[1];
    }
  }

  cout << "path: " << path << endl;
  cout << "host: " << params["Host:"] << endl;

  return HTTPRequest(path, params["Host:"], "");
}

// Split string into lines.
std::vector<std::string> splitLines(const std::string& s) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token))
  {
    tokens.push_back(token);
  }
  return tokens;
}

// Split string into lines.
std::vector<std::string> splitWords(const std::string& s) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, ' '))
  {
    tokens.push_back(token);
  }
  return tokens;


}
