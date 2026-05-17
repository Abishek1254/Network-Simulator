#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <string>
#include "../common/Types.h"
#include "Address.h"
#include "NetworkData.h"
using namespace std;

class Frame {
private:
    FrameType frameType;
    Address sourceMac;
    Address destinationMac;
    string payload;
    int sequenceNumber;
    int parityBit;
    IPPacket ipPacket;
    ArpMessage arpMessage;
    RoutingUpdate routingUpdate;

public:
    Frame();
    Frame(Address src, Address dest, string data, int seq = 0);

    static Frame createIPFrame(Address src, Address dest, const IPPacket& packet, int seq = 0);
    static Frame createArpRequest(Address src, IPv4Address senderIP, IPv4Address targetIP, int seq = 0);
    static Frame createArpReply(
        Address src,
        Address dest,
        IPv4Address senderIP,
        IPv4Address targetIP,
        Address targetMac,
        int seq = 0
    );
    static Frame createRoutingUpdate(Address src, Address dest, const RoutingUpdate& update, int seq = 0);

    FrameType getFrameType() const;
    Address getSourceMac() const;
    Address getDestinationMac() const;
    string getPayload() const;
    int getSequenceNumber() const;
    int getParityBit() const;
    IPPacket getIPPacket() const;
    ArpMessage getArpMessage() const;
    RoutingUpdate getRoutingUpdate() const;

    void setParityBit(int bit);
    bool isBroadcast() const;

    string toString() const;
};

#endif
