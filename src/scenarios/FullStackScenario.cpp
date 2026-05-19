#include "../../include/scenarios/FullStackScenario.h"
#include "../../include/applications/EchoService.h"
#include "../../include/applications/FileTransferService.h"
#include "../../include/devices/EndDevice.h"
#include "../../include/devices/Router.h"
#include "../../include/devices/Switch.h"
#include "../../include/protocols/access/CSMACD.h"
#include "../../include/protocols/error/ParityErrorControl.h"
#include "../../include/protocols/flow/GoBackNFlowControl.h"
#include <iostream>
using namespace std;

void FullStackScenario::run() {
    cout << "\n========== Full Stack Network Simulator ==========\n";
    cout << "Running physical, data link, network, transport, and application layers together\n\n";

    EndDevice pc1(101, "PC1", Address("AA:AA:AA:AA:10:01"));
    EndDevice pc4(104, "PC4", Address("AA:AA:AA:AA:10:04"));
    EndDevice pc2(102, "PC2", Address("BB:BB:BB:BB:20:02"));
    EndDevice pc3(103, "PC3", Address("CC:CC:CC:CC:30:03"));

    CSMACD csma1, csma2, csma3, csma4;
    ParityErrorControl parity1, parity2, parity3, parity4;
    GoBackNFlowControl linkGbn1(4), linkGbn2(4), linkGbn3(4), linkGbn4(4);
    GoBackNFlowControl transportGbn1(4), transportGbn2(4), transportGbn3(4), transportGbn4(4);

    pc1.setAccessControl(&csma1);
    pc1.setErrorControl(&parity1);
    pc1.setFlowControl(&linkGbn1);
    pc1.setTransportFlowControl(&transportGbn1);

    pc2.setAccessControl(&csma2);
    pc2.setErrorControl(&parity2);
    pc2.setFlowControl(&linkGbn2);
    pc2.setTransportFlowControl(&transportGbn2);

    pc3.setAccessControl(&csma3);
    pc3.setErrorControl(&parity3);
    pc3.setFlowControl(&linkGbn3);
    pc3.setTransportFlowControl(&transportGbn3);

    pc4.setAccessControl(&csma4);
    pc4.setErrorControl(&parity4);
    pc4.setFlowControl(&linkGbn4);
    pc4.setTransportFlowControl(&transportGbn4);

    pc1.configureNetwork(IPv4Address("192.168.1.10"), IPv4Address("255.255.255.0"), IPv4Address("192.168.1.1"));
    pc4.configureNetwork(IPv4Address("192.168.1.20"), IPv4Address("255.255.255.0"), IPv4Address("192.168.1.1"));
    pc2.configureNetwork(IPv4Address("172.16.5.10"), IPv4Address("255.255.255.0"), IPv4Address("172.16.5.1"));
    pc3.configureNetwork(IPv4Address("172.16.1.20"), IPv4Address("255.255.255.0"), IPv4Address("172.16.1.1"));

    EchoService echoServicePc4;
    EchoService echoServicePc3;
    FileTransferService fileServicePc2;
    fileServicePc2.addFile("routes.txt", "Connected, static, and RIP-learned routes are active.");
    fileServicePc2.addFile("report.txt", "Transport and application layers successfully integrated.");

    pc4.registerApplication(&echoServicePc4);
    pc3.registerApplication(&echoServicePc3);
    pc2.registerApplication(&fileServicePc2);

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

    cout << "\n--- Integrated Test 1: Baseline Network Layer Delivery (PC1 -> PC4) ---\n";
    pc1.sendIPData("Local subnet message from PC1 to PC4", IPv4Address("192.168.1.20"));

    cout << "\n--- Integrated Test 2: Application Echo on Local Subnet (PC1 -> PC4:7) ---\n";
    int echoClientPort = pc1.openEphemeralPort("EchoClient");
    pc1.sendApplicationData(
        IPv4Address("192.168.1.20"),
        echoClientPort,
        7,
        ApplicationMessage("EchoService", "ECHO", "Hello from PC1 on the same LAN")
    );

    cout << "\n--- Integrated Test 3: File Transfer over Static Routing (PC1 -> PC2:21) ---\n";
    int fileClientPort = pc1.openEphemeralPort("FileClient");
    pc1.sendApplicationData(
        IPv4Address("172.16.5.10"),
        fileClientPort,
        21,
        ApplicationMessage("FileTransferService", "GET", "routes.txt")
    );

    cout << "\nRouting tables before RIP convergence:\n";
    r1.printRoutingTable();
    r2.printRoutingTable();
    r3.printRoutingTable();

    cout << "\n--- Integrated Test 4: RIP Dynamic Routing Exchange ---\n";
    r3.sendRipUpdates();
    r2.sendRipUpdates();

    cout << "\nRouting tables after RIP convergence:\n";
    r1.printRoutingTable();
    r2.printRoutingTable();
    r3.printRoutingTable();

    cout << "\n--- Integrated Test 5: Echo over RIP-Learned Path (PC4 -> PC3:7) ---\n";
    int remoteEchoPort = pc4.openEphemeralPort("RemoteEchoClient");
    pc4.sendApplicationData(
        IPv4Address("172.16.1.20"),
        remoteEchoPort,
        7,
        ApplicationMessage("EchoService", "ECHO", "Cross-network request after RIP convergence")
    );

    cout << "\nARP tables after complete full-stack execution:\n";
    pc1.printArpTable();
    pc2.printArpTable();
    pc3.printArpTable();
    pc4.printArpTable();

    cout << "====================================================\n";
}
