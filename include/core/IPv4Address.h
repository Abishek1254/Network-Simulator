#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <cstdint>
#include <string>
using namespace std;

class IPv4Address {
private:
    uint32_t value;

public:
    IPv4Address();
    IPv4Address(uint32_t rawValue);
    IPv4Address(string ipAddress);

    uint32_t toInt() const;
    string toString() const;
    bool isZero() const;

    IPv4Address getNetworkAddress(const IPv4Address& subnetMask) const;
    bool isInSameSubnet(const IPv4Address& other, const IPv4Address& subnetMask) const;
    int getPrefixLength() const;

    static IPv4Address any();

    bool operator==(const IPv4Address& other) const;
    bool operator!=(const IPv4Address& other) const;
    bool operator<(const IPv4Address& other) const;
};

#endif
