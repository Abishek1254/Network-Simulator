#include "../../include/services/DomainAnalyzer.h"

int DomainAnalyzer::getCollisionDomainsForHubStar(int deviceCount) {
    return 1;
}

int DomainAnalyzer::getBroadcastDomainsForHubStar(int deviceCount) {
    return 1;
}

int DomainAnalyzer::getCollisionDomainsForSwitchStar(int deviceCount) {
    return deviceCount;
}

int DomainAnalyzer::getBroadcastDomainsForSwitchStar(int deviceCount) {
    return 1;
}

int DomainAnalyzer::getCollisionDomainsForTwoHubStarUsingSwitch(int devicesPerHub) {
    return 2;
}

int DomainAnalyzer::getBroadcastDomainsForTwoHubStarUsingSwitch(int devicesPerHub) {
    return 1;
}

void DomainAnalyzer::printDomainReport(string topologyName, int collisionDomains, int broadcastDomains) {
    cout << "\n===== Domain Report: " << topologyName << " =====" << endl;
    cout << "Collision Domains: " << collisionDomains << endl;
    cout << "Broadcast Domains: " << broadcastDomains << endl;
    cout << "======================================" << endl;
}