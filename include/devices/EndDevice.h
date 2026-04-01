#ifndef END_DEVICE_H
#define END_DEVICE_H

#include <iostream>
#include <string>
#include "Device.h"
#include "../core/SimulationLogger.h"
#include "../protocols/access/IAccessControl.h"
#include "../protocols/error/IErrorControl.h"
#include "../protocols/flow/IFlowControl.h"
using namespace std;

class EndDevice : public Device {
private:
    IAccessControl* accessControl;
    IErrorControl* errorControl;
    IFlowControl* flowControl;
    int nextSequenceNumber;

public:
    EndDevice();
    EndDevice(int id, string name, Address macAddress);

    void setAccessControl(IAccessControl* accessControl);
    void setErrorControl(IErrorControl* errorControl);
    void setFlowControl(IFlowControl* flowControl);

    void sendData(string data, Address destinationMac, bool isFrame);
    void sendFrame(Frame frame, Device* sender = NULL);
    void receiveFrame(Frame frame, Device* sender = NULL);
};

#endif