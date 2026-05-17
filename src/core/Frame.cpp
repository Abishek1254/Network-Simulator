#include "../../include/core/Frame.h"
#include <sstream>

namespace {
string getFrameTypeName(FrameType frameType) {
    if (frameType == DATA_FRAME) {
        return "DATA";
    }
    if (frameType == IP_FRAME) {
        return "IP";
    }
    if (frameType == ARP_REQUEST_FRAME) {
        return "ARP_REQUEST";
    }
    if (frameType == ARP_REPLY_FRAME) {
        return "ARP_REPLY";
    }
    return "ROUTING_UPDATE";
}
}

Frame::Frame() {
    frameType = DATA_FRAME;
    sourceMac = Address();
    destinationMac = Address();
    payload = "";
    sequenceNumber = 0;
    parityBit = 0;
    ipPacket = IPPacket();
    arpMessage = ArpMessage();
    routingUpdate = RoutingUpdate();
}

Frame::Frame(Address src, Address dest, string data, int seq) {
    frameType = DATA_FRAME;
    sourceMac = src;
    destinationMac = dest;
    payload = data;
    sequenceNumber = seq;
    parityBit = 0;
    ipPacket = IPPacket();
    arpMessage = ArpMessage();
    routingUpdate = RoutingUpdate();
}

Frame Frame::createIPFrame(Address src, Address dest, const IPPacket& packet, int seq) {
    Frame frame(src, dest, packet.getPayload(), seq);
    frame.frameType = IP_FRAME;
    frame.ipPacket = packet;
    return frame;
}

Frame Frame::createArpRequest(Address src, IPv4Address senderIP, IPv4Address targetIP, int seq) {
    Frame frame(src, Address::getBroadcastAddress(), "ARP_REQUEST", seq);
    frame.frameType = ARP_REQUEST_FRAME;
    frame.arpMessage = ArpMessage(senderIP, src, targetIP, Address());
    return frame;
}

Frame Frame::createArpReply(
    Address src,
    Address dest,
    IPv4Address senderIP,
    IPv4Address targetIP,
    Address targetMac,
    int seq
) {
    Frame frame(src, dest, "ARP_REPLY", seq);
    frame.frameType = ARP_REPLY_FRAME;
    frame.arpMessage = ArpMessage(senderIP, src, targetIP, targetMac);
    return frame;
}

Frame Frame::createRoutingUpdate(Address src, Address dest, const RoutingUpdate& update, int seq) {
    Frame frame(src, dest, "RIP_UPDATE", seq);
    frame.frameType = ROUTING_UPDATE_FRAME;
    frame.routingUpdate = update;
    return frame;
}

FrameType Frame::getFrameType() const {
    return frameType;
}

Address Frame::getSourceMac() const {
    return sourceMac;
}

Address Frame::getDestinationMac() const {
    return destinationMac;
}

string Frame::getPayload() const {
    return payload;
}

int Frame::getSequenceNumber() const {
    return sequenceNumber;
}

int Frame::getParityBit() const {
    return parityBit;
}

IPPacket Frame::getIPPacket() const {
    return ipPacket;
}

ArpMessage Frame::getArpMessage() const {
    return arpMessage;
}

RoutingUpdate Frame::getRoutingUpdate() const {
    return routingUpdate;
}

void Frame::setParityBit(int bit) {
    parityBit = bit;
}

bool Frame::isBroadcast() const {
    return destinationMac.isBroadcast();
}

string Frame::toString() const {
    stringstream ss;
    ss << "[TYPE=" << getFrameTypeName(frameType)
       << ", SRC=" << sourceMac.getMacAddress()
       << ", DEST=" << destinationMac.getMacAddress()
       << ", SEQ=" << sequenceNumber
       << ", PARITY=" << parityBit;

    if (frameType == IP_FRAME) {
        ss << ", IP=" << ipPacket.toString();
    } else if (frameType == ARP_REQUEST_FRAME || frameType == ARP_REPLY_FRAME) {
        ss << ", ARP=" << arpMessage.toString();
    } else if (frameType == ROUTING_UPDATE_FRAME) {
        ss << ", UPDATE=" << routingUpdate.toString();
    } else {
        ss << ", PAYLOAD=\"" << payload << "\"";
    }

    ss << "]";
    return ss.str();
}
