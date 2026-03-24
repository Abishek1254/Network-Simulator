#include "../../../include/protocols/access/CSMACD.h"
#include "../../../include/core/SimulationLogger.h"

CSMACD::CSMACD() {
    mediumBusy = false;
}

void CSMACD::setMediumBusy(bool busy) {
    mediumBusy = busy;
}

bool CSMACD::canTransmit(Frame frame) {
    if (mediumBusy) {
        SimulationLogger::warn("CSMA/CD: Medium is busy. Transmission blocked.");
        return false;
    }

    SimulationLogger::info("CSMA/CD: Medium is free. Transmission allowed.");
    return true;
}

string CSMACD::getProtocolName() {
    return "CSMA/CD";
}