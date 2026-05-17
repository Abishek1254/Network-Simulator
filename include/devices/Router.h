#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Device.h"
#include "../core/NetworkData.h"
#include "../core/SimulationLogger.h"
using namespace std;

struct RouterInterface {
    string name;
    IPv4Address ipAddress;
    IPv4Address subnetMask;
    Address macAddress;
    Device* connectedDevice;

    RouterInterface()
        : name(""), ipAddress(), subnetMask(), macAddress(), connectedDevice(NULL) {}

    RouterInterface(string interfaceName, IPv4Address ip, IPv4Address mask, Address mac)
        : name(interfaceName), ipAddress(ip), subnetMask(mask), macAddress(mac), connectedDevice(NULL) {}
};

class Router : public Device {
private:
    vector<RouterInterface> interfaces;
    vector<RouteEntry> routingTable;
    map<string, Address> arpTable;
    int nextSequenceNumber;

    RouterInterface* getInterfaceByName(const string& interfaceName);
    const RouterInterface* getInterfaceBySender(Device* sender) const;
    const RouterInterface* getInterfaceByMac(const Address& macAddress) const;
    const RouterInterface* getInterfaceByIp(const IPv4Address& ipAddress) const;
    RouteEntry* findBestRoute(const IPv4Address& destinationIP);
    bool resolveArp(const RouterInterface& routerInterface, const IPv4Address& targetIP);
    void processArpRequest(const Frame& frame, const RouterInterface& ingressInterface);
    void processArpReply(const Frame& frame);
    void processIPFrame(const Frame& frame);
    void processRoutingUpdate(const Frame& frame, const RouterInterface& ingressInterface);
    void forwardPacket(IPPacket packet);
    string getRouteSourceName(RouteSource source) const;

public:
    Router();
    Router(int id, string name);

    void addInterface(string interfaceName, IPv4Address ipAddress, IPv4Address subnetMask, Address macAddress);
    void connectInterface(string interfaceName, Device* device);
    void addStaticRoute(IPv4Address network, IPv4Address subnetMask, IPv4Address nextHop, string outgoingInterface, int metric = 1);
    void sendRipUpdates();
    void printRoutingTable() const;

    void sendFrame(Frame frame, Device* sender = NULL);
    void receiveFrame(Frame frame, Device* sender = NULL);
};

#endif
