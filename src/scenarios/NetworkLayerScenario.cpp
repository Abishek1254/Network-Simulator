#include "../../include/scenarios/NetworkLayerScenario.h"
#include "../../include/devices/EndDevice.h"
#include "../../include/devices/Router.h"
#include "../../include/devices/Switch.h"
#include "../../include/protocols/access/CSMACD.h"
#include "../../include/protocols/error/ParityErrorControl.h"
#include "../../include/protocols/flow/GoBackNFlowControl.h"
#include <iostream>
using namespace std;

void NetworkLayerScenario::run() {
    cout << "\n========== Network Layer Scenario ==========\n";
    cout << "Demonstrating IPv4 addressing, ARP, static routing, longest prefix matching, and RIP\n\n";

    EndDevice pc1(101, "PC1", Address("AA:AA:AA:AA:10:01"));
    EndDevice pc4(104, "PC4", Address("AA:AA:AA:AA:10:04"));
    EndDevice pc2(102, "PC2", Address("BB:BB:BB:BB:20:02"));
    EndDevice pc3(103, "PC3", Address("CC:CC:CC:CC:30:03"));

    CSMACD csma1, csma2, csma3, csma4;
    ParityErrorControl parity1, parity2, parity3, parity4;
    GoBackNFlowControl gbn1(4), gbn2(4), gbn3(4), gbn4(4);

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

    pc1.configureNetwork(IPv4Address("192.168.1.10"), IPv4Address("255.255.255.0"), IPv4Address("192.168.1.1"));
    pc4.configureNetwork(IPv4Address("192.168.1.20"), IPv4Address("255.255.255.0"), IPv4Address("192.168.1.1"));
    pc2.configureNetwork(IPv4Address("172.16.5.10"), IPv4Address("255.255.255.0"), IPv4Address("172.16.5.1"));
    pc3.configureNetwork(IPv4Address("172.16.1.20"), IPv4Address("255.255.255.0"), IPv4Address("172.16.1.1"));

    Switch lanA(401, "LAN-A-Switch");
    Switch lanB(402, "LAN-B-Switch");
    Switch lanC(403, "LAN-C-Switch");

    Router r1(501, "R1");
    Router r2(502, "R2");
    Router r3(503, "R3");

    r1.addInterface("g0/0", IPv4Address("192.168.1.1"), IPv4Address("255.255.255.0"), Address("00:11:22:33:44:01"));
    r1.addInterface("g0/1", IPv4Address("10.0.12.1"), IPv4Address("255.255.255.252"), Address("00:11:22:33:44:02"));

    r2.addInterface("g0/0", IPv4Address("10.0.12.2"), IPv4Address("255.255.255.252"), Address("00:22:33:44:55:01"));
    r2.addInterface("g0/1", IPv4Address("172.16.5.1"), IPv4Address("255.255.255.0"), Address("00:22:33:44:55:02"));
    r2.addInterface("g0/2", IPv4Address("10.0.23.1"), IPv4Address("255.255.255.252"), Address("00:22:33:44:55:03"));

    r3.addInterface("g0/0", IPv4Address("10.0.23.2"), IPv4Address("255.255.255.252"), Address("00:33:44:55:66:01"));
    r3.addInterface("g0/1", IPv4Address("172.16.1.1"), IPv4Address("255.255.255.0"), Address("00:33:44:55:66:02"));

    pc1.addConnection(&lanA);
    pc4.addConnection(&lanA);
    lanA.addConnection(&pc1);
    lanA.addConnection(&pc4);
    r1.connectInterface("g0/0", &lanA);
    lanA.addConnection(&r1);

    pc2.addConnection(&lanB);
    lanB.addConnection(&pc2);
    r2.connectInterface("g0/1", &lanB);
    lanB.addConnection(&r2);

    pc3.addConnection(&lanC);
    lanC.addConnection(&pc3);
    r3.connectInterface("g0/1", &lanC);
    lanC.addConnection(&r3);

    r1.connectInterface("g0/1", &r2);
    r2.connectInterface("g0/0", &r1);
    r2.connectInterface("g0/2", &r3);
    r3.connectInterface("g0/0", &r2);

    r1.addStaticRoute(IPv4Address("172.16.0.0"), IPv4Address("255.255.0.0"), IPv4Address("10.0.12.2"), "g0/1", 5);
    r1.addStaticRoute(IPv4Address("172.16.5.0"), IPv4Address("255.255.255.0"), IPv4Address("10.0.12.2"), "g0/1", 1);
    r2.addStaticRoute(IPv4Address("192.168.1.0"), IPv4Address("255.255.255.0"), IPv4Address("10.0.12.1"), "g0/0", 1);

    cout << "\n--- Test 5A: ARP on Local Network (PC1 -> PC4) ---\n";
    pc1.sendIPData("Local subnet message from PC1 to PC4", IPv4Address("192.168.1.20"));

    cout << "\n--- Test 5B: Static Routing + Longest Prefix Match (PC1 -> PC2) ---\n";
    pc1.sendIPData("Remote message using static route to PC2", IPv4Address("172.16.5.10"));

    cout << "\nRouting tables before RIP convergence:\n";
    r1.printRoutingTable();
    r2.printRoutingTable();
    r3.printRoutingTable();

    cout << "\n--- Test 5C: RIP Dynamic Routing Exchange ---\n";
    r3.sendRipUpdates();
    r2.sendRipUpdates();

    cout << "\nRouting tables after RIP convergence:\n";
    r1.printRoutingTable();
    r2.printRoutingTable();
    r3.printRoutingTable();

    cout << "\n--- Test 5D: RIP-based Delivery to New Network (PC1 -> PC3) ---\n";
    pc1.sendIPData("Dynamic route delivery from PC1 to PC3", IPv4Address("172.16.1.20"));

    pc1.printArpTable();
    pc2.printArpTable();
    pc3.printArpTable();
    pc4.printArpTable();

    cout << "===============================================\n";
}
