#ifndef END_DEVICE_H
#define END_DEVICE_H

#include <iostream>
#include <map>
#include <string>
#include "Device.h"
#include "../core/IPv4Address.h"
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
    IPv4Address ipAddress;
    IPv4Address subnetMask;
    IPv4Address defaultGateway;
    map<string, Address> arpTable;
    bool networkConfigured;

    bool resolveArp(const IPv4Address& targetIP);
    bool isForThisHost(const IPv4Address& destinationIP) const;
    bool transmitFrameInternal(Frame& frame, bool applyFlowControl, const string& preparationMessage);

public:
    EndDevice();
    EndDevice(int id, string name, Address macAddress);

    void setAccessControl(IAccessControl* accessControl);
    void setErrorControl(IErrorControl* errorControl);
    void setFlowControl(IFlowControl* flowControl);
    void configureNetwork(IPv4Address ipAddress, IPv4Address subnetMask, IPv4Address defaultGateway = IPv4Address::any());
    IPv4Address getIpAddress() const;
    IPv4Address getSubnetMask() const;
    void printArpTable() const;

    void sendData(string data, Address destinationMac, bool isFrame);
    void sendIPData(string data, IPv4Address destinationIP);
    void sendFrame(Frame frame, Device* sender = NULL);
    void receiveFrame(Frame frame, Device* sender = NULL);
};

#endif
