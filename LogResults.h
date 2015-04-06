#ifndef LOG_RESULTS_H
#define LOG_RESULTS_H

#include "MessageFromClient.h"
#include "Validator.h"
#include "MessageToClient.h"

class LogResults
{
  private:
    MessageFromClient* msgFromClient;
    Validator* validator;
    MessageToClient* msgToClient;

  public:
    LogResults(MessageFromClient* , Validator*, MessageToClient*);
    void logOnServer();
};

#endif