#include "../../include/common/Types.h"
#include "../../include/devices/Device.h"

Device::Device() {
    id = 0;
    name = "";
    type = END_DEVICE;
    macAddress = Address();
}

Device::Device(int id, string name, DeviceType type, Address macAddress) {
    this->id = id;
    this->name = name;
    this->type = type;
    this->macAddress = macAddress;
}

Device::~Device() {
}

int Device::getId() const {
    return id;
}

string Device::getName() const {
    return name;
}

DeviceType Device::getType() const {
    return type;
}

Address Device::getMacAddress() const {
    return macAddress;
}

void Device::addConnection(Device* device) {
    connections.push_back(device);
}

vector<Device*> Device::getConnections() const {
    return connections;
}