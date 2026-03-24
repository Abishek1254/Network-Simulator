#include "../../include/core/Frame.h"
#include <sstream>

Frame::Frame() {
    sourceMac = Address();
    destinationMac = Address();
    payload = "";
    sequenceNumber = 0;
    parityBit = 0;
}

Frame::Frame(Address src, Address dest, string data, int seq) {
    sourceMac = src;
    destinationMac = dest;
    payload = data;
    sequenceNumber = seq;
    parityBit = 0;
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

void Frame::setParityBit(int bit) {
    parityBit = bit;
}

bool Frame::isBroadcast() const {
    return destinationMac.isBroadcast();
}

string Frame::toString() const {
    stringstream ss;
    ss << "[SRC=" << sourceMac.getMacAddress()
       << ", DEST=" << destinationMac.getMacAddress()
       << ", SEQ=" << sequenceNumber
       << ", PARITY=" << parityBit
       << ", PAYLOAD=\"" << payload << "\"]";
    return ss.str();
}