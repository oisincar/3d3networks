#include <string>
#include <stdio>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "HTTPRequest.h"
using namespace std;

int main(){
    string url;
    string path;
    vector<uint8_t> encodedStr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    cout<<"error connecting"<<endl;
    return 2;
    }

    if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
    cout<<"error getting sockname"<<endl;
    return 3;
   }
    cout<<"enter the path"<<endl;
    cin >>path;
    cout<<"enter url"<<endl;
    cin>>url;

    HTTPRequest *request = new HTTPRequest(path,url);
    encodedStr = request->encode();

    if (send(sockfd, encodedStr, encodedStr.size(), 0) == -1) {
      cout<<"error sending message"<<endl;

    }
        
  string fr_name = "/home/aryan/Desktop/progetto/final.txt";
	FILE *fr = fopen(fr_name, "a");
	if(fr == NULL)
		cout<<"the file can not be opened"<<endl;
	else
	{
		bzero(revbuf, LENGTH); 
		int fr_block_sz = 0;
	    while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
	    {
			int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
	        if(write_sz < fr_block_sz)
			{
	            error("File write failed.\n");
	        }
			bzero(revbuf, LENGTH);
			if (fr_block_sz == 0 || fr_block_sz != 512) 
			{
				break;
			}
		}
		if(fr_block_sz < 0)
        {
			if (errno == EAGAIN)
			{
				cout<<"time out"<<endl;
			}
			
		}
	    cout<<"OK recieve from serer"<<endl;
	    fclose(fr);
    
     
     close(sockfd);
    return 0;
}