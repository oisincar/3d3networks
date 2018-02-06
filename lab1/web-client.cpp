#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <sstream>
#include <stdint.h>

#include "HTTPRequest.h"

vector<u_int8_t> getMessage(int sockfd);

int main()
{
  // Setup sockets..
  int portno = 80;
  std::string host = "www.example.com";
  std::string path = "/index.html";

  HTTPRequest r(path, host, "");


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
  vector<uint8_t> enc = r.encode();
  send(sockfd, (char*)(&enc[0]), enc.size(),0);

  vector<u_int8_t> result = getMessage(sockfd);

  int byte_count = result.size();

  std::cout << "Recieved " << byte_count << " bytes of information." << std::endl;
  string res(result.begin(), result.end());
  std::cout << "Msg:\n" << res << std::endl;
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
