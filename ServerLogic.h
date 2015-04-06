
/*

A Simple web server
Csc 361 programming assigment #1

Brett Binnersley
V00776751


ServerLogic.h
Desc: defines functions to initialize, run and shutdown the server.
*/

#ifndef SERVER_LOGIC_H
#define SERVER_LOGIC_H
#include "NetworkHeaders.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

//Functions used by the servers main loop
int StartServer(int argumentCnt, char* arguments[]); //Returns a positive number of success, a negative number on failure
int ServerRunningLoop();    //Runs the server
int ShutdownServer();       //Call when the server is finished running


#endif