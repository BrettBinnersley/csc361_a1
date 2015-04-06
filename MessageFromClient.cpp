/*

A Simple web server - Using C++
Csc 361 programming assigment #1

Brett Binnersley
V00776751


MessageFromClient.cpp
Desc: Class for descriving messages from the client
*/

#include "MessageFromClient.h"
#include <time.h>
#include <string>

//Constructor
MessageFromClient::MessageFromClient(struct sockaddr_in* caddr, std::string data, socklen_t length)
{
    port = ntohs(caddr->sin_port);
    clientLength = length;
    clientIP = inet_ntoa(caddr->sin_addr);
    bufferData = data;
    clientAddress = caddr;
}

//Public getters to be used by other classes
int MessageFromClient::getPort() const
{
  return port;
}

socklen_t MessageFromClient::getClientLength() const
{
  return clientLength;
}

std::string MessageFromClient::getClientIP() const
{
  return clientIP;
}

std::string MessageFromClient::getBufferData() const
{
  return bufferData;
}

struct sockaddr_in* MessageFromClient::getClientAddress() const
{
  return clientAddress;
}