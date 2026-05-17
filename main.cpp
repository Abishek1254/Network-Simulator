#include <iostream>
#include "include/scenarios/DedicatedLinkScenario.h"
#include "include/scenarios/HubStarScenario.h"
#include "include/scenarios/SwitchScenario.h"
#include "include/scenarios/HybridScenario.h"
#include "include/scenarios/NetworkLayerScenario.h"

using namespace std;

int main() {

    // cout << "\n================ PHYSICAL LAYER ======================\n";
    // cout << "- Creating end devices and hubs\n";
    // cout << "- Creating connections (topology)\n";
    // cout << "- Sending and receiving data\n";

    // cout << "\n--- Test Case 1: Dedicated Link (2 End Devices) ---\n";
    // DedicatedLinkScenario::run();

    // cout << "\n--- Test Case 2: Hub Star Topology (5 Devices) ---\n";
    // HubStarScenario::run();

    // cout << "\n================ DATA LINK LAYER ====================\n";
    // cout << "- Bridge and Switch implementation\n";
    // cout << "- MAC Address Learning (Switch)\n";
    // cout << "- Error Control Protocol (Parity)\n";
    // cout << "- Access Control Protocol (CSMA/CD)\n";
    // cout << "- Sliding Window Flow Control (Go-Back-N)\n";

    // cout << "\n--- Test Case 3: Switch with 5 Devices ---\n";
    // SwitchScenario::run();

    // cout << "\n--- Test Case 4: Hybrid Topology ---\n";
    // cout << "Two Hub Stars Connected via Switch\n";
    // cout << "Demonstrating:\n";
    // cout << "- Communication across 10 devices\n";
    // cout << "- Broadcast & Collision Domains\n";

    // HybridScenario::run();

    cout << "\n================ NETWORK LAYER ======================\n";
    cout << "- Router creation and IPv4 configuration\n";
    cout << "- ARP-based next-hop MAC resolution\n";
    cout << "- Static routing and longest prefix match\n";
    cout << "- RIP-based dynamic routing\n";

    cout << "\n--- Test Case 5: Network Layer Integration ---\n";
    NetworkLayerScenario::run();

    return 0;
}
