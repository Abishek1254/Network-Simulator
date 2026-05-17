#include "../../include/common/Types.h"
#include "../../include/devices/Bridge.h"

Bridge::Bridge() : Device() {
}

Bridge::Bridge(int id, string name)
    : Device(id, name, BRIDGE, Address("00:00:00:00:00:00")) {
}

void Bridge::sendFrame(Frame frame, Device* sender) {
    receiveFrame(frame, sender);
}

void Bridge::receiveFrame(Frame frame, Device* sender) {
    string srcMac = frame.getSourceMac().getMacAddress();
    string destMac = frame.getDestinationMac().getMacAddress();

    if (macTable.find(srcMac) == macTable.end()) {
        macTable[srcMac] = sender;
        SimulationLogger::info(name + " learned MAC " + srcMac);
    }

    if (frame.isBroadcast()) {
        SimulationLogger::info(name + " received broadcast frame, forwarding to all other connections.");

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
            SimulationLogger::info(name + " filtered frame. Source and destination are on same side.");
        } 
        else {
            SimulationLogger::info(name + " forwarding frame only to destination side.");
            target->receiveFrame(frame, this);
        }
    }
     else {
        SimulationLogger::info(name + " destination unknown, flooding frame.");

        for (int i = 0; i < connections.size(); i++) {
            if (connections[i] != sender) {
                connections[i]->receiveFrame(frame, this);
            }
        }
    }
}