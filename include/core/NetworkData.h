#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include <sstream>
#include <string>
#include <vector>
#include "../common/Types.h"
#include "Address.h"
#include "IPv4Address.h"
using namespace std;

struct ApplicationMessage {
    string serviceName;
    string operation;
    string payload;
    bool response;
    string status;

    ApplicationMessage()
        : serviceName(""), operation(""), payload(""), response(false), status("OK") {}

    ApplicationMessage(string service, string appOperation, string data, bool isResponse = false, string appStatus = "OK")
        : serviceName(service), operation(appOperation), payload(data), response(isResponse), status(appStatus) {}

    string toString() const {
        stringstream ss;
        ss << "[SERVICE=" << serviceName
           << ", OP=" << operation
           << ", TYPE=" << (response ? "RESPONSE" : "REQUEST")
           << ", STATUS=" << status
           << ", DATA=\"" << payload << "\"]";
        return ss.str();
    }
};

struct TransportSegment {
    int sourcePort;
    int destinationPort;
    int sequenceNumber;
    int acknowledgementNumber;
    TransportProtocol protocol;
    bool acknowledgement;
    ApplicationMessage applicationMessage;

    TransportSegment()
        : sourcePort(0),
          destinationPort(0),
          sequenceNumber(0),
          acknowledgementNumber(0),
          protocol(TCP_GBN_PROTOCOL),
          acknowledgement(false),
          applicationMessage() {}

    TransportSegment(
        int srcPort,
        int destPort,
        int seq,
        TransportProtocol transportProtocol,
        ApplicationMessage message,
        bool isAcknowledgement = false,
        int ackNumber = 0
    ) : sourcePort(srcPort),
        destinationPort(destPort),
        sequenceNumber(seq),
        acknowledgementNumber(ackNumber),
        protocol(transportProtocol),
        acknowledgement(isAcknowledgement),
        applicationMessage(message) {}

    string getProtocolName() const {
        if (protocol == UDP_PROTOCOL) {
            return "UDP";
        }
        return "TCP-GBN";
    }

    string toString() const {
        stringstream ss;
        ss << "[PROTO=" << getProtocolName()
           << ", SRC_PORT=" << sourcePort
           << ", DEST_PORT=" << destinationPort
           << ", SEQ=" << sequenceNumber
           << ", ACK=" << acknowledgementNumber
           << ", ACK_ONLY=" << (acknowledgement ? "YES" : "NO")
           << ", APP=" << applicationMessage.toString()
           << "]";
        return ss.str();
    }
};

class IPPacket {
private:
    IPv4Address sourceIP;
    IPv4Address destinationIP;
    string payload;
    int ttl;
    bool hasTransportPayload;
    TransportSegment transportSegment;

public:
    IPPacket() : sourceIP(), destinationIP(), payload(""), ttl(16), hasTransportPayload(false), transportSegment() {}

    IPPacket(IPv4Address src, IPv4Address dest, string data, int packetTtl = 16)
        : sourceIP(src),
          destinationIP(dest),
          payload(data),
          ttl(packetTtl),
          hasTransportPayload(false),
          transportSegment() {}

    IPPacket(IPv4Address src, IPv4Address dest, const TransportSegment& segment, int packetTtl = 16)
        : sourceIP(src),
          destinationIP(dest),
          payload(segment.applicationMessage.payload),
          ttl(packetTtl),
          hasTransportPayload(true),
          transportSegment(segment) {}

    IPv4Address getSourceIP() const { return sourceIP; }
    IPv4Address getDestinationIP() const { return destinationIP; }
    string getPayload() const { return payload; }
    int getTTL() const { return ttl; }
    bool containsTransportSegment() const { return hasTransportPayload; }
    TransportSegment getTransportSegment() const { return transportSegment; }

    void decrementTTL() {
        if (ttl > 0) {
            ttl--;
        }
    }

    string toString() const {
        stringstream ss;
        ss << "[SRC_IP=" << sourceIP.toString()
           << ", DEST_IP=" << destinationIP.toString()
           << ", TTL=" << ttl;

        if (hasTransportPayload) {
            ss << ", TRANSPORT=" << transportSegment.toString();
        } else {
            ss << ", PAYLOAD=\"" << payload << "\"";
        }

        ss << "]";
        return ss.str();
    }
};

struct ArpMessage {
    IPv4Address senderIP;
    Address senderMac;
    IPv4Address targetIP;
    Address targetMac;

    ArpMessage()
        : senderIP(), senderMac(), targetIP(), targetMac() {}

    ArpMessage(IPv4Address srcIP, Address srcMac, IPv4Address destIP, Address destMac)
        : senderIP(srcIP), senderMac(srcMac), targetIP(destIP), targetMac(destMac) {}

    string toString() const {
        stringstream ss;
        ss << "[SENDER_IP=" << senderIP.toString()
           << ", SENDER_MAC=" << senderMac.getMacAddress()
           << ", TARGET_IP=" << targetIP.toString()
           << ", TARGET_MAC=" << targetMac.getMacAddress()
           << "]";
        return ss.str();
    }
};

struct RouteAdvertisement {
    IPv4Address network;
    IPv4Address subnetMask;
    int metric;

    RouteAdvertisement() : network(), subnetMask(), metric(0) {}
    RouteAdvertisement(IPv4Address routeNetwork, IPv4Address mask, int routeMetric)
        : network(routeNetwork), subnetMask(mask), metric(routeMetric) {}
};

struct RoutingUpdate {
    IPv4Address sourceRouterIP;
    vector<RouteAdvertisement> routes;

    string toString() const {
        stringstream ss;
        ss << "[SOURCE_ROUTER=" << sourceRouterIP.toString()
           << ", ROUTES=" << routes.size() << "]";
        return ss.str();
    }
};

struct RouteEntry {
    IPv4Address network;
    IPv4Address subnetMask;
    IPv4Address nextHop;
    string outgoingInterface;
    int metric;
    RouteSource source;

    RouteEntry()
        : network(), subnetMask(), nextHop(), outgoingInterface(""), metric(0), source(CONNECTED_ROUTE) {}

    RouteEntry(
        IPv4Address routeNetwork,
        IPv4Address mask,
        IPv4Address routeNextHop,
        string interfaceName,
        int routeMetric,
        RouteSource routeSource
    ) : network(routeNetwork),
        subnetMask(mask),
        nextHop(routeNextHop),
        outgoingInterface(interfaceName),
        metric(routeMetric),
        source(routeSource) {}

    string toString() const {
        stringstream ss;
        ss << network.toString() << "/" << subnetMask.getPrefixLength()
           << " via " << (nextHop.isZero() ? "DIRECT" : nextHop.toString())
           << " dev " << outgoingInterface
           << " metric " << metric;
        return ss.str();
    }
};

#endif
