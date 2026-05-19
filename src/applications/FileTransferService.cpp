#include "../../include/applications/FileTransferService.h"

void FileTransferService::addFile(const string& fileName, const string& content) {
    files[fileName] = content;
}

string FileTransferService::getServiceName() const {
    return "FileTransferService";
}

int FileTransferService::getWellKnownPort() const {
    return 21;
}

ApplicationMessage FileTransferService::handleRequest(const ApplicationMessage& request, const string& hostName) {
    if (request.operation != "GET") {
        return ApplicationMessage(
            getServiceName(),
            "ERROR",
            "Unsupported operation on " + hostName,
            true,
            "FAILED"
        );
    }

    if (files.find(request.payload) == files.end()) {
        return ApplicationMessage(
            getServiceName(),
            "FILE_NOT_FOUND",
            request.payload + " not present on " + hostName,
            true,
            "FAILED"
        );
    }

    return ApplicationMessage(
        getServiceName(),
        "FILE_DATA",
        files[request.payload],
        true,
        "OK"
    );
}
