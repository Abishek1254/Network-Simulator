#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include <sstream>
#include <string>
#include <vector>
#include "../common/Types.h"
#include "Address.h"
#include "IPv4Address.h"
using namespace std;

class IPPacket {
private:
    IPv4Address sourceIP;
    IPv4Address destinationIP;
    string payload;
    int ttl;

public:
    IPPacket() : sourceIP(), destinationIP(), payload(""), ttl(16) {}

    IPPacket(IPv4Address src, IPv4Address dest, string data, int packetTtl = 16)
        : sourceIP(src), destinationIP(dest), payload(data), ttl(packetTtl) {}

    IPv4Address getSourceIP() const { return sourceIP; }
    IPv4Address getDestinationIP() const { return destinationIP; }
    string getPayload() const { return payload; }
    int getTTL() const { return ttl; }

    void decrementTTL() {
        if (ttl > 0) {
            ttl--;
        }
    }

    string toString() const {
        stringstream ss;
        ss << "[SRC_IP=" << sourceIP.toString()
           << ", DEST_IP=" << destinationIP.toString()
           << ", TTL=" << ttl
           << ", PAYLOAD=\"" << payload << "\"]";
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
