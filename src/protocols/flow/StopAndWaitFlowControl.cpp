#include "../../../include/protocols/flow/StopAndWaitFlowControl.h"
#include "../../../include/core/SimulationLogger.h"

StopAndWaitFlowControl::StopAndWaitFlowControl() {
    waitingForAck = false;
}

bool StopAndWaitFlowControl::sendWithFlowControl(Frame frame) {
    if (waitingForAck) {
        SimulationLogger::warn("Stop-and-Wait: Previous frame not acknowledged yet.");
        return false;
    }

    waitingForAck = true;
    SimulationLogger::info("Stop-and-Wait: Frame sent with sequence number " + to_string(frame.getSequenceNumber()));
    return true;
}

void StopAndWaitFlowControl::receiveAck() {
    waitingForAck = false;
    SimulationLogger::info("Stop-and-Wait: ACK received.");
}

string StopAndWaitFlowControl::getProtocolName() {
    return "Stop-and-Wait";
}