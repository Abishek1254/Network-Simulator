#ifndef APPLICATION_SERVICE_H
#define APPLICATION_SERVICE_H

#include <string>
#include "../core/NetworkData.h"
using namespace std;

class ApplicationService {
public:
    virtual ~ApplicationService() {}

    virtual string getServiceName() const = 0;
    virtual int getWellKnownPort() const = 0;
    virtual ApplicationMessage handleRequest(const ApplicationMessage& request, const string& hostName) = 0;
};

#endif
