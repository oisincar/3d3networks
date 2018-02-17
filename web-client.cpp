// Run with ./web-client localhost
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <sstream>
#include <stdint.h>

#include "HTTPRequest.h"

vector<u_int8_t> getMessage(int sockfd);

int main(int argc, char **argv)
{
  if (argc < 2) {
    std::cout << "Please enter url to request." << endl
              << "Usage: ./web-client [URL:PORT/FILENAME]" << endl;
    return 0;
  }

  // Loop through all urls given.
  for (int i = 1; i < argc; i++) {

    std::string url(argv[i]);

    // Parse arguments.

    // Up until : is the host.
    size_t colonIx = url.find(':');
    std::string host = url.substr(0, colonIx);

    // From : to '/' (if there is one) is the portno.
    size_t slashIx = url.find('/', colonIx);
    std::string port = url.substr(colonIx+1, slashIx - colonIx-1);
    int portno = stoi(port);

    std::string file = "index.html";
    // If there is a slash/ it's not the last charachter.. Then we have a file.
    if (slashIx != std::string::npos && slashIx+1 < url.length() )
      file = url.substr(slashIx+1, url.length()-slashIx-1);

    cout << "Host: " << host << " Port: " << portno << " File: " << file << endl;

    // create a socket using TCP IP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Check of sockfd is not -1..?

    struct hostent *server = gethostbyname(host.c_str());
    // check server non-null..


    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portno);     // short, network byte order

    // Configure address.
    bcopy((char *)server->h_addr,
          (char *)&serverAddr.sin_addr.s_addr,
          server->h_length);

    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
      perror("connect");
      return 2;
    }

    // Send get request..


    // std::string path = "/index.html";

    HTTPRequest r(file, host);

    vector<uint8_t> enc = r.encode();
    send(sockfd, (char*)(&enc[0]), enc.size(),0);

    vector<u_int8_t> result = getMessage(sockfd);

    int byte_count = result.size();

    std::cout << "Recieved " << byte_count << " bytes of information." << std::endl;
    string res(result.begin(), result.end());
    std::cout << "Msg:\n" << res << std::endl;

  }
}

vector<u_int8_t> getMessage(int sockfd) {
  vector<uint8_t> result;
  char buf[20];
  // CARE: WRONG. Needs to handle overlapping requests..
  // Also may not insert a 0 at the end correctly.
  int n;
  while((n = recv(sockfd, buf, 20, 0)) > 0) {
    result.insert(result.end(), buf, buf+n);
  }
  return result;
}
