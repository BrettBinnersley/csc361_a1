
/*

A Simple web server - Using C++
Csc 361 programming assigment #1

Brett Binnersley
V00776751


ServerLogic.cpp
Desc: Contains functionality to initialize, run and shutdown the server.
*/

#include "ServerLogic.h"
#include "MessageToClient.h"
#include "MessageFromClient.h"
#include "LogResults.h"
#include "Validator.h"
#include <cstring>
#include <sys/stat.h>


//Variables that only ServerLogic cares about.
#define MAXFILENAMELENGTH 1024
int socketDesc;
int portNumber;
struct sockaddr_in serverAddress;
struct sockaddr_in clientAddress;
std::string directoryPath;


//Not defined in the header file - only needed in this source file.
int isReadyToReadInput(int socketDesc)
{
  while (1)
  {
    char read_buffer[MAX_BUFFER_LEN];
    fd_set readDesc;
    FD_ZERO(&readDesc);

    FD_SET(STDIN_FILENO, &readDesc);
    FD_SET(socketDesc, &readDesc);
    int retval = select(socketDesc + 1, &readDesc, NULL, NULL, NULL);
    if(retval <= 0)
    {
      return retval; //Error occured or request timed out.
    }
    else
    {
      if(FD_ISSET(STDIN_FILENO, &readDesc) && (fgets(read_buffer, MAX_BUFFER_LEN, stdin) != NULL) && strchr(read_buffer, 'q') != NULL)
      {
        return 1; // Q was pressed
      }
      else if(FD_ISSET(socketDesc, &readDesc))
      {  // recv buffer readDescdy
        return 2;
      }
    }
  }
  return -1;
}

//Attempt to start the server. It will return a negative number on failure or 0 on success
int StartServer(int argumentCnt, char* arguments[])
{

  //Ensure minimum amount of arguments were passed
  if (argumentCnt < 3)
  {
    printf( "Usage: %s <port> <directory>\n", arguments[0] );
    fprintf(stderr,"Error: Wrong number of parameters passed\n");
    return -1;
  }

  //Create socket and gracefully handle any exceptions
  socketDesc = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketDesc == -1)
  {
    perror("Server: Error creating the socket");
    return -1;
  }

  portNumber = atoi(arguments[1]);

  //Give it the correct directory & Ensure that it is infact a directory. (It has to exist, and it has to be a directory)
  directoryPath = std::string(arguments[2]);

  struct stat fileInfo;
  bool dirExists = (stat(directoryPath.c_str(), &fileInfo) == 0);

  if(dirExists == 0)
  {
    printf("Server: That directory does not exist. Ensure that you enter an existing filepath.");
    return -1; //File does not exist. This will throw the client a 404 error
  }
  else
  {
    if(!S_ISDIR(fileInfo.st_mode))
    {
      printf("Server: The specified directory path was to a file. Ensure that it is to a directory.");
      return -1;
    }
  }


  //Bind the socket with the address information so we know where to recieve the information from.
  bzero((char *) &serverAddress, sizeof(serverAddress));    //Zero the memory
  serverAddress.sin_family = AF_INET;                       //We always want AF_INET
  serverAddress.sin_addr.s_addr = INADDR_ANY;//inet_addr("10.10.1.100"); //Servers Address
  serverAddress.sin_port = htons(portNumber);               //Port

  //Bind the socket with the address information and gracefully handle exceptions
  int bindSuccess = bind(socketDesc, (struct sockaddr* ) &serverAddress, sizeof(serverAddress));
  if (bindSuccess < 0)
  {
    close(socketDesc);
    perror("Server: Error on binding");
    return -1;
  }
  return 0;
}



//This will run the server, it will listen for requests from the client and gracefully handle them
int ServerRunningLoop()
{
  while(1)  //Always run
  {
    printf("Server: Waiting for messages...\n");
    socklen_t clientLength = sizeof(clientAddress);
    int inputVal = isReadyToReadInput(socketDesc);  //Read input from the keyboard or the socket.
    if(inputVal == 1) //Q was pressed. Something / somebody requested that this process be terminated.
    {
      exit(1);
      return 0;
    }

    //Send information to ClientAddress
    char buffer[MAX_BUFFER_LEN]; //data buffer
    int numberOfBytes = recvfrom(socketDesc, buffer, MAX_BUFFER_LEN - 1 , 0, (struct sockaddr *)&clientAddress, &clientLength);
    if (numberOfBytes == -1)
    {
      perror("Server: Error on recvfrom()!");
      return -1;
    }
    buffer[numberOfBytes] = '\0'; //Null terminate the buffer.
    //printf("RAWSTRING: %s\n", buffer); // Debug message. Uncomment to see the raw data being sent to the client.

    //Got a message from the client.
    MessageFromClient msgFromClient(&clientAddress, buffer, clientLength);

    //Find out if the request is good or not
    Validator validReq(&msgFromClient, socketDesc);
    validReq.validateClientRequest(directoryPath);

    //Send results back to the client
    MessageToClient msgToClient(&msgFromClient, &validReq);
    msgToClient.send();

    //Log Results on the server side
    LogResults logResults(&msgFromClient, &validReq, &msgToClient);
    logResults.logOnServer();

  }
  return 0;
}



//Shutdown the server
int ShutdownServer()
{
  close(socketDesc);
  return 0;
}
