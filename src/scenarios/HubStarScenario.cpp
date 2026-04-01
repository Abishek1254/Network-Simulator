#include "../../include/scenarios/HubStarScenario.h"
#include "../../include/devices/EndDevice.h"
#include "../../include/devices/Hub.h"
#include "../../include/services/DomainAnalyzer.h"
#include <iostream>
using namespace std;

void HubStarScenario::run() {
    cout << "\n========== Hub Star Scenario ==========\n";

    EndDevice pc1(1, "PC1", Address("AA:AA:AA:AA:AA:01"));
    EndDevice pc2(2, "PC2", Address("AA:AA:AA:AA:AA:02"));
    EndDevice pc3(3, "PC3", Address("AA:AA:AA:AA:AA:03"));
    EndDevice pc4(4, "PC4", Address("AA:AA:AA:AA:AA:04"));
    EndDevice pc5(5, "PC5", Address("AA:AA:AA:AA:AA:05"));

    Hub hub1(10, "Hub1");

    pc1.addConnection(&hub1);
    pc2.addConnection(&hub1);
    pc3.addConnection(&hub1);
    pc4.addConnection(&hub1);
    pc5.addConnection(&hub1);

    hub1.addConnection(&pc1);
    hub1.addConnection(&pc2);
    hub1.addConnection(&pc3);
    hub1.addConnection(&pc4);
    hub1.addConnection(&pc5);

    pc1.sendData("Message from PC1 to PC3 through hub", Address("AA:AA:AA:AA:AA:03"), true);
    pc3.sendData("ACK from PC3 to PC1 through hub", Address("AA:AA:AA:AA:AA:01"), false);

    DomainAnalyzer::printDomainReport(
        "Hub Star Topology",
        DomainAnalyzer::getCollisionDomainsForHubStar(5),
        DomainAnalyzer::getBroadcastDomainsForHubStar(5)
    );

    cout << "======================================\n";
}