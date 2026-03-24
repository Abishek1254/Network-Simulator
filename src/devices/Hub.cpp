#include "../../include/common/Types.h"
#include "../../include/devices/Hub.h"

Hub::Hub() : Device() {
}

Hub::Hub(int id, string name)
    : Device(id, name, HUB, Address("00:00:00:00:00:00")) {
}

void Hub::sendFrame(Frame frame, Device* sender) {
    receiveFrame(frame, sender);
}

void Hub::receiveFrame(Frame frame, Device* sender) {
    SimulationLogger::info(name + " received frame and is broadcasting it.");

    for (int i = 0; i < connections.size(); i++) {
        if (connections[i] != sender) {
            connections[i]->receiveFrame(frame, this);
        }
    }
}