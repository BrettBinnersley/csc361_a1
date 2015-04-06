
/*

A Simple web server - Using C++
Csc 361 programming assigment #1

Brett Binnersley
V00776751


Validator.cpp
Desc: Given a request from a client, this will validate whether or not the http request was valid,
and if it was it will contain the information about the requested object

*/
#include "Validator.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <utility>
#include <sys/stat.h>
#include <algorithm>
#include <iterator>
#include <string.h>


Validator::Validator(MessageFromClient* msg, int descOfSocket)
{
  //Sep 12 12:00:00 10.10.1.1:5678 GET / HTTP/1.0; HTTP/1.0 200 OK; /tmp/www/index.html
  clientMsg = msg;
  validRequest = false;
  fexist = false;
  socketDesc = descOfSocket;
  method = "";
  httpVersion = "";
  filename = "";
  relFilePath = "";
  reqLine = "";
}

//Public getters

std::string Validator::getRequestLine() const
{
  return reqLine;
}

std::string Validator::getFilename() const
{
  return filename;
}

const char* Validator::getData() const //Note: Filedata is a pointer as it may be large. We don't want to copy it everywhere.
{
  return fileData.c_str();
}

std::string Validator::getMethod() const
{
  return method;
}

std::string Validator::getHttpVersion() const
{
  return httpVersion;
}

bool Validator::isValidRequest() const
{
  return validRequest;
}

int Validator::getSocketDesc() const
{
  return socketDesc;
}

bool Validator::fileExists() const
{
  return fexist;
}

std::string Validator::getFilePath() const
{
  return relFilePath;
}

//Private functions
void Validator::validateClientRequest(std::string dirPath)
{
  //Turn std::string into a c string so we can tokenize it on whitespace
  std::string request(clientMsg->getBufferData());
  std::istringstream f(request);
  std::vector<std::string> lines;
  std::string line;

  //Add all the request data to the lines vector.
  while (std::getline(f, line)) { //Breaks at a \n
    //We were passed requests with a \r\n for the endings of the headers
    char c = line.at(line.length() - 1);
    while(c == '\r')
    {
      line.erase(line.length() - 1, 1); //So erase the '\r' from the end of the string. We don't want to be playing around with it.
      c = line.at(line.length() - 1);
    }

    lines.push_back(line);
  }

  //Line#[0] should* be equal to the method, request URI, HTTP Version. If it isn't we will throw a 400 error
  char* clientBufferData = strdup(lines.at(0).c_str()); //Duplicate the string so that we can modify it (can't modify a const char*)


  char* posTok = strtok (clientBufferData," \t");
  method = std::string(posTok);

  posTok = strtok (NULL, " \t");
  if(posTok != NULL)  //Only set if the filename if it is found. Else leave it as be (nullstring) which will exit gracefully below.
    filename = std::string(posTok);

  posTok = strtok (NULL, " \n\t\0");
  if(posTok != NULL) //Only set if the HTTPVersion if it is found. Else leave it as be (nullstring) which will exit gracefully below.
    httpVersion = std::string(posTok);


  reqLine = method; //Set the requestLine to the buffer that we read in.
  free(clientBufferData); //Release the memory that was copied

  //See if we were passed more than 3 arguments. If we were, throw a 400 error.
  posTok = strtok (NULL, " \r\n\t\0"); //There should be nothing left other than spaces!
  if(posTok != NULL)
  {
    return;
  }

  //Note: URI (filename) is case sensitive. method & http version are not, so we can convert them to lower case and treat them like so.
  std::transform(method.begin(), method.end(), method.begin(), ::tolower);
  std::transform(httpVersion.begin(), httpVersion.end(), httpVersion.begin(), ::tolower);

  if(method.compare("get") != 0)
  {
    return; //Invalidate request. BAD REQUEST - error 400. A GET command was not issued
  }

  if(filename.at(0) != '/')
  {
    return; //Invalidate request. BAD REQUEST - error 400. URI Error. Needs to start with a '/'
  }

  if(filename.c_str()[filename.length() - 1] == '/')
  {
    filename.append("index.html");  //A filename ending with a '/' implies we should look for index.html in that folder.
  }

  if(httpVersion.compare("http/1.0") != 0)
  {
    return; //Invalidate request. BAD REQUEST - error 400. Http version 1.0 was not selected
  }

  //Valid request is set to true - No more 400 errors can be encountered. 404 if file is not found, else 200 OK
  validRequest = true;

  //See if the filename contains any "../" If it does, throw a
  if(filename.find("../") != std::string::npos)
  {
    return; //Found at least 1 ../, throw a 404 not found!
  }

  //Set the filepath
  relFilePath = dirPath;
  relFilePath.append(filename);
  struct stat fileInfo;
  fexist = (stat(relFilePath.c_str(), &fileInfo) == 0);

  if(fexist == 0)
  {
    return; //File does not exist. This will throw the client a 404 error
  }
  else
  {
    if(S_ISDIR(fileInfo.st_mode)) //Client tried to open up a folder. Note: We can note open up folders!
    {
      fexist = false; //Tell the server the file is not avaible (404 - File not found will be given)
      return;
    }
  }
  //Request made it this far. The information must be valid.
  extractRawDataFromFile();
}

void Validator::extractRawDataFromFile()
{
  std::ifstream input(relFilePath.c_str(), std::ios::in | std::ios::binary);
  if (input)
  {
    std::string fileContents;
    input.seekg(0, std::ios::end);
    fileContents.resize(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(&fileContents[0], fileContents.size());
    input.close();
    fileData = fileContents; //May want to optimize this. It could* be slow. zzz
  }
  else
  {
    printf("Error: Server couldn't open the requested file.");
  }

}


