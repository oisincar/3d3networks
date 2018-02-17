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
#include <thread>

#include "HTTPRequest.h"
#include "HTTPResponse.h"

const int MsgLen = 1024;
string readFile(string filename);
void handleRequest(int clientSockfd);

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


  while (true) {
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

    // Create a new thread to handle request.
    std::thread t = thread(handleRequest, clientSockfd);
    t.detach();
  }
}

void handleRequest(int clientSockfd) {
  cout << "New thread spawned to handle requests on fd: " << clientSockfd << std::endl;
  const int timeout = 3;

  // Set socket timeout:
  struct timeval tv;
  tv.tv_sec = timeout;
  tv.tv_usec = 0;
  setsockopt(clientSockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

  char buf[MsgLen] = {0};
  // std::stringstream ss;
  string message = "";

  // --------------------------HTTPRESPONSE FAIL HERE------------------------
  vector<uint8_t> empty_vec;
  HTTPResponse response("400 Bad Request", "", empty_vec);

  while(1) {

    int m_len = 0;
    if ((m_len = recv(clientSockfd, buf, MsgLen, 0)) == -1) {
      perror("recv");
      break;
    }
    if (m_len == 0) {
      cout << "Client disconnected, or no message recieved within timeout period (" << timeout << "s)" << std::endl;
      break;
    }

    string buf_s(buf);
    size_t endix = buf_s.find("\r\n");

    // Append up to the occurence of endix.
    message.append(buf_s, 0, endix);

    // If there was a terminator, respond to the message.
    if (endix != std::string::npos) {

      std::cout << "Got full message: " << buf << std::endl;

      // Convert to vector
      vector<uint8_t> vec(buf, buf + m_len);
      HTTPRequest request(vec);
      string file = readFile("." + request.getPath());

      // If file reading failed..
      // HTTPResponse 404..
      // response = ...(...);

      // else
      // successful response..
      // response = ...(...);

      break;
    }
  }

  // Send response, but split into submessages of a maximum length
  // of MsgLen.
  vector<uint8_t> resp = response.encode();
  string resp_s(resp.begin(), resp.end());
  cout << "Responding: " << resp_s << std::endl;
  for (int i = 0; i < resp_s.length(); i += MsgLen) {

    string substr = resp_s.substr(i, MsgLen);

    if (send(clientSockfd, substr.c_str(), substr.length(), 0) == -1) {
      break;
    }
  }

  // Close thread..
  cout << "Closing thread for fd: " << clientSockfd << std::endl;
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
