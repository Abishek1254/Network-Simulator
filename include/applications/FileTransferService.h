#ifndef FILE_TRANSFER_SERVICE_H
#define FILE_TRANSFER_SERVICE_H

#include <map>
#include <string>
#include "ApplicationService.h"
using namespace std;

class FileTransferService : public ApplicationService {
private:
    map<string, string> files;

public:
    void addFile(const string& fileName, const string& content);
    string getServiceName() const;
    int getWellKnownPort() const;
    ApplicationMessage handleRequest(const ApplicationMessage& request, const string& hostName);
};

#endif
