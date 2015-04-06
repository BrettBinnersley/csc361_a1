//Class used to process a message from the client
#ifndef MESSAGE_FROM_CLIENT_H
#define MESSAGE_FROM_CLIENT_H

#include "NetworkHeaders.h"
#include <string>

class MessageFromClient
{
  private:
    int port;
    socklen_t clientLength;
    std::string clientIP;
    std::string bufferData;
    struct sockaddr_in* clientAddress;

  public:
    MessageFromClient(struct sockaddr_in* clientAddress, std::string data, socklen_t length);
    int getPort() const;
    socklen_t getClientLength() const;
    std::string getClientIP() const;
    std::string getBufferData() const;
    struct sockaddr_in* getClientAddress() const;

};
#endif