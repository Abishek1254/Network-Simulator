#include "../../include/common/Types.h"
#include "../../include/devices/EndDevice.h"

EndDevice::EndDevice() : Device() {
    accessControl = NULL;
    errorControl = NULL;
    flowControl = NULL;
    nextSequenceNumber = 0;
}

EndDevice::EndDevice(int id, string name, Address macAddress)
    : Device(id, name, END_DEVICE, macAddress) {
    accessControl = NULL;
    errorControl = NULL;
    flowControl = NULL;
    nextSequenceNumber = 0;
}

void EndDevice::setAccessControl(IAccessControl* accessControl) {
    this->accessControl = accessControl;
}

void EndDevice::setErrorControl(IErrorControl* errorControl) {
    this->errorControl = errorControl;
}

void EndDevice::setFlowControl(IFlowControl* flowControl) {
    this->flowControl = flowControl;
}

void EndDevice::sendData(string data, Address destinationMac, bool isFrame) {
    Frame frame(macAddress, destinationMac, data, nextSequenceNumber);
    if(isFrame)
        SimulationLogger::info(name + " is preparing to send frame: " + frame.toString());
    else
        SimulationLogger::info(name + " is preparing to send ACK:" + frame.toString());

    if (errorControl != NULL) {
        errorControl->applyErrorControl(frame);
    }

    if (accessControl != NULL) {
        if (!accessControl->canTransmit(frame)) {
            SimulationLogger::warn(name + " could not send frame due to access control.");
            return;
        }
    }

    if (flowControl != NULL) {
        if (!flowControl->sendWithFlowControl(frame)) {
            SimulationLogger::warn(name + " could not send frame due to flow control.");
            return;
        }
    }

    if (connections.size() == 0) {
        SimulationLogger::error(name + " has no connection to send data.");
        return;
    }

    SimulationLogger::info(name + " is sending frame: " + frame.toString());
    connections[0]->receiveFrame(frame, this);

    nextSequenceNumber++;
}

void EndDevice::sendFrame(Frame frame, Device* sender) {
    if (connections.size() == 0) {
        SimulationLogger::error(name + " has no connection to send frame.");
        return;
    }

    connections[0]->receiveFrame(frame, this);
}

void EndDevice::receiveFrame(Frame frame, Device* sender) {
    if (frame.getDestinationMac() == macAddress || frame.isBroadcast()) {
        if (errorControl != NULL) {
            if (!errorControl->checkError(frame)) {
                SimulationLogger::error(name + " discarded frame due to parity error.");
                return;
            }
        }

        SimulationLogger::info(name + " received frame: " + frame.toString());
        cout << name << " received data: " << frame.getPayload() << endl;
    } else {
        SimulationLogger::info(name + " ignored frame not meant for it.");
    }
}