#ifndef MESSAGE_TO_CLIENT_H
#define MESSAGE_TO_CLIENT_H

#include "NetworkHeaders.h"
#include "Validator.h"
#include "MessageFromClient.h"

class MessageToClient
{
  private:
    std::string msgTime;
    std::string TimeAsString();
    MessageFromClient* msgFromClient;
    Validator* validator;
    bool sent;
    std::string respLine;
    int SendMessageInMultiplePackets(const char* str, const size_t strLen); //Break the message up into smaller packets, and send them all the client
                                                                            //This will return a negative number if an error was encountered.
  public:
    MessageToClient(MessageFromClient* , Validator*);
    std::string getTimeSent() const;
    int send(); //Sends the message to the client. Will return a negative number on failure, 1 on success
    bool didSend() const;
    std::string getResponseLine() const;
};

#endif