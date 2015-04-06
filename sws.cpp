
/*

A Simple web server - Using C++
Csc 361 programming assigment #1

Brett Binnersley
V00776751


sws.c
Desc: A very lightweight entry point to the server
*/

#include <stdio.h>
#include "ServerLogic.h"
int main(int argumentCnt, char* arguments[])
{
  //Attempt to start the web server. If it fails, exit. Note: The error message is printed inside the StartServer() function if one is found
  if(StartServer(argumentCnt, arguments) < 0)
  {
    return -1;
  }
  printf("%s is running on UDP port %s and serving %s\n", arguments[0], arguments[1], arguments[2]);
  printf("press ‘q’ to quit ...\n");

  //Main logic loop. Listen for information from the clients. Do what a server does. You serve :D
  ServerRunningLoop();

  //We are done. Close the socket and return.
  return ShutdownServer();
}











