#ifndef ECHO_SERVICE_H
#define ECHO_SERVICE_H

#include "ApplicationService.h"

class EchoService : public ApplicationService {
public:
    string getServiceName() const;
    int getWellKnownPort() const;
    ApplicationMessage handleRequest(const ApplicationMessage& request, const string& hostName);
};

#endif
