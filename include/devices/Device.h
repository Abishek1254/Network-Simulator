#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <vector>
#include "../common/Types.h"
#include "../core/Address.h"
#include "../core/Frame.h"
using namespace std;

class Device {
protected:
    int id;
    string name;
    DeviceType type;
    Address macAddress;
    vector<Device*> connections;

public:
    Device();
    Device(int id, string name, DeviceType type, Address macAddress);
    virtual ~Device();

    int getId() const;
    string getName() const;
    DeviceType getType() const;
    Address getMacAddress() const;

    void addConnection(Device* device);
    vector<Device*> getConnections() const;

    virtual void sendFrame(Frame frame, Device* sender = NULL) = 0;
    virtual void receiveFrame(Frame frame, Device* sender = NULL) = 0;
};

#endif