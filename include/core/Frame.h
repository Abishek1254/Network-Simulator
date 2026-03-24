#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <string>
#include "Address.h"
using namespace std;

class Frame {
private:
    Address sourceMac;
    Address destinationMac;
    string payload;
    int sequenceNumber;
    int parityBit;

public:
    Frame();
    Frame(Address src, Address dest, string data, int seq = 0);

    Address getSourceMac() const;
    Address getDestinationMac() const;
    string getPayload() const;
    int getSequenceNumber() const;
    int getParityBit() const;

    void setParityBit(int bit);
    bool isBroadcast() const;

    string toString() const;
};

#endif