#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include <string>
using namespace std;

class Address {
private:
    string macAddress;

public:
    Address();
    Address(string mac);

    string getMacAddress() const;
    bool isBroadcast() const;

    static Address getBroadcastAddress();

    bool operator==(const Address& other) const;
    bool operator!=(const Address& other) const;
};

#endif