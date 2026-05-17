#include "../../../include/protocols/error/ParityErrorControl.h"
#include "../../../include/core/SimulationLogger.h"

void ParityErrorControl::applyErrorControl(Frame& frame) {
    string data = frame.getPayload();
    int count = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i] == '1') {
            count++;
        }
    }

    int parityBit = count % 2;
    frame.setParityBit(parityBit);

    SimulationLogger::info("Parity Error Control applied. Parity bit = " + to_string(parityBit));
}

bool ParityErrorControl::checkError(Frame frame) {
    string data = frame.getPayload();
    int count = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i] == '1')
        count++;
    }

    int expectedParity = count % 2;

    if (expectedParity == frame.getParityBit()) {
        SimulationLogger::info("Parity check passed.");
        return true;
    } 
    else {
        SimulationLogger::warn("Parity check failed.");
        return false;
    }
}

string ParityErrorControl::getProtocolName() {
    return "Parity Error Control";
}