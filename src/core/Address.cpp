#include "../../include/core/Address.h"

Address::Address() {
    macAddress = "00:00:00:00:00:00";
}

Address::Address(string mac) {
    macAddress = mac;
}

string Address::getMacAddress() const {
    return macAddress;
}

bool Address::isBroadcast() const {
    return macAddress == "FF:FF:FF:FF:FF:FF";
}

Address Address::getBroadcastAddress() {
    return Address("FF:FF:FF:FF:FF:FF");
}

bool Address::operator==(const Address& other) const {
    return macAddress == other.macAddress;
}

bool Address::operator!=(const Address& other) const {
    return macAddress != other.macAddress;
}