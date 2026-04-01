#include "../../include/scenarios/HybridScenario.h"
#include "../../include/devices/EndDevice.h"
#include "../../include/devices/Hub.h"
#include "../../include/devices/Switch.h"
#include "../../include/services/DomainAnalyzer.h"
#include <iostream>
using namespace std;

void HybridScenario::run() {
    cout << "\n========== Hybrid Scenario ==========\n";
    cout << "Two star topologies using hubs, connected through a switch\n\n";

    EndDevice a1(1, "A1", Address("AA:AA:AA:AA:AA:01"));
    EndDevice a2(2, "A2", Address("AA:AA:AA:AA:AA:02"));
    EndDevice a3(3, "A3", Address("AA:AA:AA:AA:AA:03"));
    EndDevice a4(4, "A4", Address("AA:AA:AA:AA:AA:04"));
    EndDevice a5(5, "A5", Address("AA:AA:AA:AA:AA:05"));

    EndDevice b1(6, "B1", Address("BB:BB:BB:BB:BB:01"));
    EndDevice b2(7, "B2", Address("BB:BB:BB:BB:BB:02"));
    EndDevice b3(8, "B3", Address("BB:BB:BB:BB:BB:03"));
    EndDevice b4(9, "B4", Address("BB:BB:BB:BB:BB:04"));
    EndDevice b5(10, "B5", Address("BB:BB:BB:BB:BB:05"));

    Hub hub1(100, "Hub1");
    Hub hub2(200, "Hub2");
    Switch sw1(300, "Switch1");

    a1.addConnection(&hub1);
    a2.addConnection(&hub1);
    a3.addConnection(&hub1);
    a4.addConnection(&hub1);
    a5.addConnection(&hub1);

    hub1.addConnection(&a1);
    hub1.addConnection(&a2);
    hub1.addConnection(&a3);
    hub1.addConnection(&a4);
    hub1.addConnection(&a5);

    b1.addConnection(&hub2);
    b2.addConnection(&hub2);
    b3.addConnection(&hub2);
    b4.addConnection(&hub2);
    b5.addConnection(&hub2);

    hub2.addConnection(&b1);
    hub2.addConnection(&b2);
    hub2.addConnection(&b3);
    hub2.addConnection(&b4);
    hub2.addConnection(&b5);

    hub1.addConnection(&sw1);
    hub2.addConnection(&sw1);

    sw1.addConnection(&hub1);
    sw1.addConnection(&hub2);

    a1.sendData("Message from A1 to B4", Address("BB:BB:BB:BB:BB:04"), true);
    b4.sendData("ACK from B4 to A1", Address("AA:AA:AA:AA:AA:01"), false);

    b3.sendData("Message from B3 to A2", Address("AA:AA:AA:AA:AA:02"), true);
    a2.sendData("ACK from A2 to B3", Address("BB:BB:BB:BB:BB:03"), false);


    DomainAnalyzer::printDomainReport(
        "Two Hub Stars Connected Through Switch",
        DomainAnalyzer::getCollisionDomainsForTwoHubStarUsingSwitch(5),
        DomainAnalyzer::getBroadcastDomainsForTwoHubStarUsingSwitch(5)
    );

    cout << "====================================\n";
}