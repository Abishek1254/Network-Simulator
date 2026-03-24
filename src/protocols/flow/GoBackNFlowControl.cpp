#include "../../../include/protocols/flow/GoBackNFlowControl.h"
#include "../../../include/core/SimulationLogger.h"

GoBackNFlowControl::GoBackNFlowControl(int windowSize) {
    this->windowSize = windowSize;
    base = 0;
    nextSequenceNumber = 0;
}

bool GoBackNFlowControl::sendWithFlowControl(Frame frame) {
    if (nextSequenceNumber < base + windowSize) {
        SimulationLogger::info("Go-Back-N: Frame sent. Seq = " + to_string(frame.getSequenceNumber()));
        nextSequenceNumber++;
        return true;
    } else {
        SimulationLogger::warn("Go-Back-N: Window full. Cannot send frame now.");
        return false;
    }
}

void GoBackNFlowControl::receiveAck(int ackNumber) {
    if (ackNumber >= base) {
        base = ackNumber + 1;
        SimulationLogger::info("Go-Back-N: ACK received for seq " + to_string(ackNumber));
    }
}

string GoBackNFlowControl::getProtocolName() {
    return "Go-Back-N";
}

int GoBackNFlowControl::getBase() const {
    return base;
}

int GoBackNFlowControl::getNextSequenceNumber() const {
    return nextSequenceNumber;
}

int GoBackNFlowControl::getWindowSize() const {
    return windowSize;
}