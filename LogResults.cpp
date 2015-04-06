
/*

A Simple web server - Using C++
Csc 361 programming assigment #1

Brett Binnersley
V00776751


LogResults.cpp
Desc: This will log a message on the server about the last message sent to the client
*/


#include "LogResults.h"
#include <stdio.h>

LogResults::LogResults(MessageFromClient* msgx, Validator* val, MessageToClient* msgy)
{
    msgFromClient = msgx;
    validator = val;
    msgToClient = msgy;
}


//MMM DD HH:MM:SS Client-IP:Client-Port request-line; response-line; [filename]
void LogResults::logOnServer()
{
  if(msgFromClient == NULL || validator == NULL || msgToClient == NULL)
  {
    printf("Error: Can't log invalid input\n");
    return;
  }

  //Only print out the file IF everything was successful and the message[s] got sent to the client
  std::string filename("");
  if(validator->isValidRequest() && validator->fileExists() && msgToClient->didSend())
  {
    filename = validator->getFilePath();
  }
  printf("%s %s:%d %s; %s; %s\n",
    msgToClient->getTimeSent().c_str(),
    msgFromClient->getClientIP().c_str(),
    msgFromClient->getPort(),
    validator->getRequestLine().c_str(),
    msgToClient->getResponseLine().c_str(),
    filename.c_str()
  );
}