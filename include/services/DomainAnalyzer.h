#ifndef DOMAIN_ANALYZER_H
#define DOMAIN_ANALYZER_H

#include <iostream>
#include <string>
using namespace std;

class DomainAnalyzer {
public:
    static int getCollisionDomainsForHubStar(int deviceCount);
    static int getBroadcastDomainsForHubStar(int deviceCount);

    static int getCollisionDomainsForSwitchStar(int deviceCount);
    static int getBroadcastDomainsForSwitchStar(int deviceCount);

    static int getCollisionDomainsForTwoHubStarUsingSwitch(int devicesPerHub);
    static int getBroadcastDomainsForTwoHubStarUsingSwitch(int devicesPerHub);

    static void printDomainReport(string topologyName, int collisionDomains, int broadcastDomains);
};

#endif