// #include <iostream>
// #include <stdint.h>
// #include <string.h>
// #include <vector>
// #include <map>
// Response.
#include "HTTPRequest.h"

#include <bits/stdc++.h>

// using namespace std;

HTTPRequest::HTTPRequest (string path, string url) {
  _path = path;
  _URL = url;
}

vector<uint8_t> HTTPRequest:: encode(){
  string header = "GET /" + _path +" HTTP/1.0\r\n" +
                  "Host: " + _URL + "\r\n\r\n";

  // Possibly doesn't end in 0?
  vector<uint8_t> vec(header.begin(), header.end());
  return vec;
}

HTTPRequest::HTTPRequest(vector<uint8_t> ss){
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
  auto params = parseHeader(vector<string>(lines.begin()+1, lines.end()));

  _path = path;
  _URL = params["Host:"];

  cout << "path: " << path << endl;
  cout << "host: " << params["Host:"] << endl;
}
