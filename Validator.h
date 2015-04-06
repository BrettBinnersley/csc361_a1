#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "MessageFromClient.h"

class Validator{
private:
  //Private data
  std::string reqLine;
  std::string filename;
  std::string fileData;
  std::string relFilePath;

  std::string method;
  std::string httpVersion;
  bool validRequest;
  bool fexist;

  MessageFromClient* clientMsg;
  int socketDesc;
  void extractRawDataFromFile();  //Reads whatever is in <filename> into the fileData string

public:
  Validator(MessageFromClient* , int);
  void validateClientRequest(std::string directoryPath);    //Attempts to see what the clients request was, and its validity (200, 400, 402 etc.)
                                                            //Returns an empty string when no errors occured, else returns the error in a string.
  std::string getRequestLine() const;
  std::string getFilename() const;
  std::string getFilePath()const;
  std::string getMethod() const;
  std::string getHttpVersion() const;
  bool isValidRequest() const;
  bool fileExists() const;
  int getSocketDesc() const;
  const char* getData() const; //Note: Data is returned as a pointer as it may be very large, and we don't want to push around data everywhere.
};


#endif