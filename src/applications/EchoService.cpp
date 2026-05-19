#include "../../include/applications/EchoService.h"

string EchoService::getServiceName() const {
    return "EchoService";
}

int EchoService::getWellKnownPort() const {
    return 7;
}

ApplicationMessage EchoService::handleRequest(const ApplicationMessage& request, const string& hostName) {
    return ApplicationMessage(
        getServiceName(),
        "ECHO_RESPONSE",
        "Echo from " + hostName + ": " + request.payload,
        true,
        "OK"
    );
}
