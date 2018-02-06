#import <string>

enum RequestType { Put, Get, Post };

class HttpRequest {
 private:
  std::string url;
  // method something something;
  RequestType type;
  std::string message;

 public:
  HttpRequest(char buffer[]);
  HttpRequest PutRequest(std::string url, std::string message);
  HttpRequest GetRequest(std::string url);
};


// Dunno.. Maybe better layed out like this..
class HttpMsg {
 private:
  RequestType type;
  std::string message;
};
