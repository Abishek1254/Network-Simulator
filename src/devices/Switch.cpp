#include "../../include/common/Types.h"
#include "../../include/devices/Switch.h"

Switch::Switch() : Device() {
}

Switch::Switch(int id, string name)
    : Device(id, name, SWITCH_DEVICE, Address("00:00:00:00:00:00")) {
}

void Switch::sendFrame(Frame frame, Device* sender) {
    receiveFrame(frame, sender);
}

void Switch::receiveFrame(Frame frame, Device* sender) {
    string srcMac = frame.getSourceMac().getMacAddress();
    string destMac = frame.getDestinationMac().getMacAddress();

    macTable[srcMac] = sender;
    SimulationLogger::info(name + " learned MAC " + srcMac);

    if (frame.isBroadcast()) {
        SimulationLogger::info(name + " received broadcast frame, flooding.");

        for (int i = 0; i < connections.size(); i++) {
            if (connections[i] != sender) {
                connections[i]->receiveFrame(frame, this);
            }
        }
        return;
    }

    if (macTable.find(destMac) != macTable.end()) {
        Device* target = macTable[destMac];

        if (target == sender) {
            SimulationLogger::info(name + " filtered frame because source and destination are on same port side.");
            return;
        }

        SimulationLogger::info(name + " forwarding frame to known destination " + destMac);
        target->receiveFrame(frame, this);
    } else {
        SimulationLogger::info(name + " destination MAC unknown, flooding frame.");

        for (int i = 0; i < connections.size(); i++) {
            if (connections[i] != sender) {
                connections[i]->receiveFrame(frame, this);
            }
        }
    }
}

void Switch::printMACTable() const {
    cout << "\nMAC Table of " << name << ":\n";
    for (map<string, Device*>::const_iterator it = macTable.begin(); it != macTable.end(); it++) {
        cout << it->first << " -> " << it->second->getName() << endl;
    }
}