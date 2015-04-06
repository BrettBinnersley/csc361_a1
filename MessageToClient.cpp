/*

A Simple web server - Using C++
Csc 361 programming assigment #1

Brett Binnersley
V00776751


MessageToClient.cpp
Desc: Contains functionality to initialize, run and shutdown the server.
*/

#include "MessageToClient.h"
#include <cstring>
#include <stdio.h>
#include <algorithm>

//Constructor
MessageToClient::MessageToClient(MessageFromClient* msgx, Validator* valid)
{
    msgTime = "UnknownTime";
    msgFromClient = msgx;
    validator = valid;
    sent = false;
    respLine = "";
}

//Public functions
std::string MessageToClient::getTimeSent() const
{
    return msgTime;
}

int MessageToClient::send()
{
  //We want to send the data as: HTTP/1.0 <NUM> <MSG> <emptyline> <data>. No data will be sent if there is an error number 400 or 404
  int numberOfBytes = -1;
  std::string buffer(""); //Initially empty buffer
  if(validator->isValidRequest()) //Send a 200 or 404 in here (Valid requests)
  {
    if(validator->fileExists())
    {
      buffer = respLine = "HTTP/1.0 200 OK";
      //Send a 200 - OK Message
      buffer.append("\r\n\r\n");     //Buffer sent to the client needs line breaks. We don't want them on the console.
      buffer.append(validator->getData());            //Attach file data to the buffer.
    }
    else
    {
      buffer = respLine = "HTTP/1.0 404 Not Found";
    }
  }
  else  //Send a 400 -> BAD REQUEST.
  {
    buffer = respLine = "HTTP/1.0 400 Bad Request"; //Followed by a blank line (\r\n) as is required
  }

  buffer.append("\r\n");  //Add terminator to the buffer

  //Send the message to the client - in multiple packets if needed
  if(SendMessageInMultiplePackets(buffer.c_str(), buffer.length()) < 0)
  {
    return -1; //Error sending the message to the client
  }

  msgTime = TimeAsString(); //Server has processed the request. Update the time to be displayed on the screen.
  sent = true;
  return 0;
}

//Public getters
bool MessageToClient::didSend() const
{
  return sent;
}

std::string MessageToClient::getResponseLine() const
{
  return respLine;
}

//Private functions
int MessageToClient::SendMessageInMultiplePackets(const char* str, const size_t strLen)
{
  for(size_t x = 0; x<strLen; x+=MAX_BUFFER_LEN)
  {
    //Determine the number of bytes to send from the buffer. Smaller of [strLen - Place in array], [MAX_BUFFER_LEN]
    size_t bytesToSend = std::min(int(strLen - x), MAX_BUFFER_LEN);

    int numberOfBytes = sendto(validator->getSocketDesc(), str + x, bytesToSend, 0,
          (struct sockaddr *)msgFromClient->getClientAddress(), msgFromClient->getClientLength());

    if (numberOfBytes == -1)
    {
      perror("Server: Error in sendto()");  //Error sending messages to the client. This is BAD!
      return -1;
    }
  }
  return 0; //Successful
}

std::string MessageToClient::TimeAsString()
{
  time_t rawtime;
  struct tm* timeinfo;
  char buffer [256];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  //Requested format: MMM DD HH:MM:SS
  strftime (buffer, 256, "%b %d %I:%M:%S",timeinfo);

  return std::string(buffer);
}