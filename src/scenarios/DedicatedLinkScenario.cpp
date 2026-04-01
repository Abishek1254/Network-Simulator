#include "../../include/scenarios/DedicatedLinkScenario.h"
#include "../../include/devices/EndDevice.h"
#include "../../include/core/SimulationLogger.h"
#include <iostream>
using namespace std;

void DedicatedLinkScenario::run() {
    cout << "\n========== Dedicated Link Scenario ==========\n";

    EndDevice pc1(1, "PC1", Address("AA:AA:AA:AA:AA:01"));
    EndDevice pc2(2, "PC2", Address("AA:AA:AA:AA:AA:02"));

    pc1.addConnection(&pc2);
    pc2.addConnection(&pc1);

    pc1.sendData("Hello over dedicated link", Address("AA:AA:AA:AA:AA:02"), true);

    cout << "=============================================\n";
}