#include "../../include/core/IPv4Address.h"
#include <sstream>
#include <vector>

IPv4Address::IPv4Address() {
    value = 0;
}

IPv4Address::IPv4Address(uint32_t rawValue) {
    value = rawValue;
}

IPv4Address::IPv4Address(string ipAddress) {
    value = 0;

    stringstream ss(ipAddress);
    string segment;
    vector<int> octets;

    while (getline(ss, segment, '.')) {
        octets.push_back(stoi(segment));
    }

    if (octets.size() != 4) {
        value = 0;
        return;
    }

    value = 0;
    for (int i = 0; i < 4; i++) {
        value = (value << 8) | (octets[i] & 255);
    }
}

uint32_t IPv4Address::toInt() const {
    return value;
}

string IPv4Address::toString() const {
    stringstream ss;
    ss << ((value >> 24) & 255) << "."
       << ((value >> 16) & 255) << "."
       << ((value >> 8) & 255) << "."
       << (value & 255);
    return ss.str();
}

bool IPv4Address::isZero() const {
    return value == 0;
}

IPv4Address IPv4Address::getNetworkAddress(const IPv4Address& subnetMask) const {
    return IPv4Address(value & subnetMask.value);
}

bool IPv4Address::isInSameSubnet(const IPv4Address& other, const IPv4Address& subnetMask) const {
    return (value & subnetMask.value) == (other.value & subnetMask.value);
}

int IPv4Address::getPrefixLength() const {
    int prefixLength = 0;
    uint32_t maskValue = value;

    while ((maskValue & 0x80000000) != 0) {
        prefixLength++;
        maskValue <<= 1;
    }

    return prefixLength;
}

IPv4Address IPv4Address::any() {
    return IPv4Address(0);
}

bool IPv4Address::operator==(const IPv4Address& other) const {
    return value == other.value;
}

bool IPv4Address::operator!=(const IPv4Address& other) const {
    return value != other.value;
}

bool IPv4Address::operator<(const IPv4Address& other) const {
    return value < other.value;
}
