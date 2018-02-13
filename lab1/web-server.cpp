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

#include <fstream>
// #include <string>
// #include <iostream>

#include "HTTPRequest.h"

const int MsgLen = 1024;
string readFile(string filename);

int main()
{

  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // allow others to reuse the address
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }

  // bind address to socket
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(40000);     // short, network byte order
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 2;
  }

  // set socket to listen status
  if (listen(sockfd, 1) == -1) {
    perror("listen");
    return 3;
  }

  // accept a new connection
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

  if (clientSockfd == -1) {
    perror("accept");
    return 4;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Accept a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;

  // read/write data from/into the connection
  // bool isEnd = false;
  char buf[MsgLen] = {0};
  std::stringstream ss;

  while(1) {

    int m_len = 0;
    if ((m_len = recv(clientSockfd, buf, MsgLen, 0)) == -1) {
      perror("recv");
      return 5;
    }

    ss << buf << std::endl;
    std::cout << buf << std::endl;


    vector<uint8_t> vec(buf, buf + m_len);
    HTTPRequest request(vec);


    string file = readFile("." + request.getPath());
  }
}

string readFile(string filename) {

  const char *s = filename.c_str();

  std::cout << "Getting request for file: " << s << std::endl;
  std::ifstream infile(s);
  if (!infile.good()) {
    cout << "Could not find file." << endl;
    return "";
  }
  std::stringstream file_ss;
  file_ss << infile.rdbuf();

  std::cout << "File found: " << file_ss.str() << endl;

  return file_ss.str();
}
