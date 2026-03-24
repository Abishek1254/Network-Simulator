#include "../../include/scenarios/SwitchScenario.h"
#include "../../include/devices/EndDevice.h"
#include "../../include/devices/Switch.h"
#include "../../include/protocols/access/CSMACD.h"
#include "../../include/protocols/error/ParityErrorControl.h"
#include "../../include/protocols/flow/GoBackNFlowControl.h"
#include "../../include/services/DomainAnalyzer.h"
#include <iostream>
using namespace std;

void SwitchScenario::run() {
    cout << "\n========== Switch Scenario ==========\n";
    cout << "Five end devices connected to a switch\n";
    cout << "Demonstrating address learning, access control, error control, and flow control\n\n";

    EndDevice pc1(1, "PC1", Address("AA:AA:AA:AA:AA:01"));
    EndDevice pc2(2, "PC2", Address("AA:AA:AA:AA:AA:02"));
    EndDevice pc3(3, "PC3", Address("AA:AA:AA:AA:AA:03"));
    EndDevice pc4(4, "PC4", Address("AA:AA:AA:AA:AA:04"));
    EndDevice pc5(5, "PC5", Address("AA:AA:AA:AA:AA:05"));

    Switch sw1(20, "Switch1");

    CSMACD csma1, csma2, csma3, csma4, csma5;
    ParityErrorControl parity1, parity2, parity3, parity4, parity5;
    GoBackNFlowControl gbn1(4), gbn2(4), gbn3(4), gbn4(4), gbn5(4);

    pc1.setAccessControl(&csma1);
    pc1.setErrorControl(&parity1);
    pc1.setFlowControl(&gbn1);

    pc2.setAccessControl(&csma2);
    pc2.setErrorControl(&parity2);
    pc2.setFlowControl(&gbn2);

    pc3.setAccessControl(&csma3);
    pc3.setErrorControl(&parity3);
    pc3.setFlowControl(&gbn3);

    pc4.setAccessControl(&csma4);
    pc4.setErrorControl(&parity4);
    pc4.setFlowControl(&gbn4);

    pc5.setAccessControl(&csma5);
    pc5.setErrorControl(&parity5);
    pc5.setFlowControl(&gbn5);

    pc1.addConnection(&sw1);
    pc2.addConnection(&sw1);
    pc3.addConnection(&sw1);
    pc4.addConnection(&sw1);
    pc5.addConnection(&sw1);

    sw1.addConnection(&pc1);
    sw1.addConnection(&pc2);
    sw1.addConnection(&pc3);
    sw1.addConnection(&pc4);
    sw1.addConnection(&pc5);

    cout << "\n--- Transmission 1: PC1 -> PC3 ---\n";
    pc1.sendData("1010101", Address("AA:AA:AA:AA:AA:03"));

    cout << "\n--- Transmission 2: PC3 -> PC1 ---\n";
    pc3.sendData("1110001", Address("AA:AA:AA:AA:AA:01"));

    cout << "\n--- Transmission 3: PC1 -> PC5 ---\n";
    pc1.sendData("1100110", Address("AA:AA:AA:AA:AA:05"));

    cout << "\n--- Transmission 4: PC5 -> PC2 ---\n";
    pc5.sendData("0011001", Address("AA:AA:AA:AA:AA:02"));

    cout << "\n";
    sw1.printMACTable();

    DomainAnalyzer::printDomainReport(
        "Switch Star Topology",
        DomainAnalyzer::getCollisionDomainsForSwitchStar(5),
        DomainAnalyzer::getBroadcastDomainsForSwitchStar(5)
    );

    cout << "=====================================\n";
}