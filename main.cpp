#include <iostream>
#include "include/scenarios/DedicatedLinkScenario.h"
#include "include/scenarios/HubStarScenario.h"
#include "include/scenarios/SwitchScenario.h"
#include "include/scenarios/HybridScenario.h"
using namespace std;

int main() {
    int choice;

    while (true) {
        cout << "\n========= Network Simulator =========\n";
        cout << "1. Dedicated Link Scenario\n";
        cout << "2. Hub Star Scenario\n";
        cout << "3. Switch Scenario\n";
        cout << "4. Hybrid Scenario\n";
        cout << "5. Run All Scenarios\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting simulator.\n";
            break;
        } else if (choice == 1) {
            DedicatedLinkScenario::run();
        } else if (choice == 2) {
            HubStarScenario::run();
        } else if (choice == 3) {
            SwitchScenario::run();
        } else if (choice == 4) {
            HybridScenario::run();
        } else if (choice == 5) {
            DedicatedLinkScenario::run();
            HubStarScenario::run();
            SwitchScenario::run();
            HybridScenario::run();
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}