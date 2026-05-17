#include "../../include/common/Types.h"
#include "../../include/devices/Router.h"

Router::Router() : Device() {
    type = ROUTER_DEVICE;
    nextSequenceNumber = 0;
}

Router::Router(int id, string name)
    : Device(id, name, ROUTER_DEVICE, Address("00:00:00:00:00:00")) {
    nextSequenceNumber = 0;
}

RouterInterface* Router::getInterfaceByName(const string& interfaceName) {
    for (int i = 0; i < interfaces.size(); i++) {
        if (interfaces[i].name == interfaceName) {
            return &interfaces[i];
        }
    }
    return NULL;
}

const RouterInterface* Router::getInterfaceBySender(Device* sender) const {
    for (int i = 0; i < interfaces.size(); i++) {
        if (interfaces[i].connectedDevice == sender) {
            return &interfaces[i];
        }
    }
    return NULL;
}

const RouterInterface* Router::getInterfaceByMac(const Address& macAddress) const {
    for (int i = 0; i < interfaces.size(); i++) {
        if (interfaces[i].macAddress == macAddress) {
            return &interfaces[i];
        }
    }
    return NULL;
}

const RouterInterface* Router::getInterfaceByIp(const IPv4Address& ipAddress) const {
    for (int i = 0; i < interfaces.size(); i++) {
        if (interfaces[i].ipAddress == ipAddress) {
            return &interfaces[i];
        }
    }
    return NULL;
}

RouteEntry* Router::findBestRoute(const IPv4Address& destinationIP) {
    RouteEntry* bestRoute = NULL;

    for (int i = 0; i < routingTable.size(); i++) {
        RouteEntry& route = routingTable[i];

        if (destinationIP.getNetworkAddress(route.subnetMask) != route.network) {
            continue;
        }

        if (bestRoute == NULL) {
            bestRoute = &route;
            continue;
        }

        int currentPrefixLength = route.subnetMask.getPrefixLength();
        int bestPrefixLength = bestRoute->subnetMask.getPrefixLength();

        if (currentPrefixLength > bestPrefixLength) {
            bestRoute = &route;
            continue;
        }

        if (currentPrefixLength == bestPrefixLength && route.metric < bestRoute->metric) {
            bestRoute = &route;
        }
    }

    return bestRoute;
}

bool Router::resolveArp(const RouterInterface& routerInterface, const IPv4Address& targetIP) {
    string arpKey = targetIP.toString();

    if (arpTable.find(arpKey) != arpTable.end()) {
        return true;
    }

    if (routerInterface.connectedDevice == NULL) {
        SimulationLogger::error(name + " interface " + routerInterface.name + " is not connected.");
        return false;
    }

    SimulationLogger::info(
        name + " broadcasting ARP request on " + routerInterface.name
        + " for " + targetIP.toString()
    );

    Frame arpRequest = Frame::createArpRequest(
        routerInterface.macAddress,
        routerInterface.ipAddress,
        targetIP,
        nextSequenceNumber
    );

    routerInterface.connectedDevice->receiveFrame(arpRequest, this);
    return arpTable.find(arpKey) != arpTable.end();
}

void Router::processArpRequest(const Frame& frame, const RouterInterface& ingressInterface) {
    ArpMessage arpMessage = frame.getArpMessage();
    arpTable[arpMessage.senderIP.toString()] = arpMessage.senderMac;

    if (arpMessage.targetIP != ingressInterface.ipAddress) {
        return;
    }

    SimulationLogger::info(
        name + " responding to ARP request for interface "
        + ingressInterface.name + " (" + ingressInterface.ipAddress.toString() + ")"
    );

    Frame reply = Frame::createArpReply(
        ingressInterface.macAddress,
        arpMessage.senderMac,
        ingressInterface.ipAddress,
        arpMessage.senderIP,
        arpMessage.senderMac,
        nextSequenceNumber
    );

    ingressInterface.connectedDevice->receiveFrame(reply, this);
    nextSequenceNumber++;
}

void Router::processArpReply(const Frame& frame) {
    ArpMessage arpMessage = frame.getArpMessage();
    arpTable[arpMessage.senderIP.toString()] = arpMessage.senderMac;

    SimulationLogger::info(
        name + " updated ARP cache: "
        + arpMessage.senderIP.toString() + " -> "
        + arpMessage.senderMac.getMacAddress()
    );
}

void Router::processIPFrame(const Frame& frame) {
    IPPacket packet = frame.getIPPacket();

    if (getInterfaceByIp(packet.getDestinationIP()) != NULL) {
        SimulationLogger::info(name + " received packet destined for the router itself: " + packet.toString());
        return;
    }

    forwardPacket(packet);
}

void Router::processRoutingUpdate(const Frame& frame, const RouterInterface& ingressInterface) {
    RoutingUpdate update = frame.getRoutingUpdate();
    bool tableChanged = false;

    for (int i = 0; i < update.routes.size(); i++) {
        RouteAdvertisement advertisement = update.routes[i];

        if (advertisement.metric >= 16) {
            continue;
        }

        IPv4Address learnedNetwork = advertisement.network;
        IPv4Address learnedMask = advertisement.subnetMask;
        int learnedMetric = advertisement.metric + 1;

        bool connectedRoute = false;
        for (int j = 0; j < routingTable.size(); j++) {
            if (routingTable[j].source == CONNECTED_ROUTE
                && routingTable[j].network == learnedNetwork
                && routingTable[j].subnetMask == learnedMask) {
                connectedRoute = true;
                break;
            }
        }

        if (connectedRoute) {
            continue;
        }

        bool protectedByExistingNonRipRoute = false;
        for (int j = 0; j < routingTable.size(); j++) {
            if (routingTable[j].source != RIP_ROUTE
                && routingTable[j].network == learnedNetwork
                && routingTable[j].subnetMask == learnedMask
                && routingTable[j].metric <= learnedMetric) {
                protectedByExistingNonRipRoute = true;
                break;
            }
        }

        if (protectedByExistingNonRipRoute) {
            continue;
        }

        bool updatedExisting = false;
        for (int j = 0; j < routingTable.size(); j++) {
            if (routingTable[j].source == RIP_ROUTE
                && routingTable[j].network == learnedNetwork
                && routingTable[j].subnetMask == learnedMask) {
                if (learnedMetric < routingTable[j].metric
                    || routingTable[j].nextHop == update.sourceRouterIP) {
                    routingTable[j].nextHop = update.sourceRouterIP;
                    routingTable[j].outgoingInterface = ingressInterface.name;
                    routingTable[j].metric = learnedMetric;
                    updatedExisting = true;
                    tableChanged = true;
                } else {
                    updatedExisting = true;
                }
                break;
            }
        }

        if (!updatedExisting) {
            routingTable.push_back(
                RouteEntry(
                    learnedNetwork,
                    learnedMask,
                    update.sourceRouterIP,
                    ingressInterface.name,
                    learnedMetric,
                    RIP_ROUTE
                )
            );
            tableChanged = true;
        }
    }

    if (tableChanged) {
        SimulationLogger::info(name + " updated routing table using RIP update from " + update.sourceRouterIP.toString());
    } else {
        SimulationLogger::info(name + " received RIP update but routing table remained unchanged.");
    }
}

void Router::forwardPacket(IPPacket packet) {
    if (packet.getTTL() <= 1) {
        SimulationLogger::warn(name + " dropped packet because TTL expired.");
        return;
    }

    RouteEntry* bestRoute = findBestRoute(packet.getDestinationIP());
    if (bestRoute == NULL) {
        SimulationLogger::error(name + " has no route to " + packet.getDestinationIP().toString());
        return;
    }

    RouterInterface* outgoingInterface = getInterfaceByName(bestRoute->outgoingInterface);
    if (outgoingInterface == NULL || outgoingInterface->connectedDevice == NULL) {
        SimulationLogger::error(name + " could not use outgoing interface " + bestRoute->outgoingInterface);
        return;
    }

    IPv4Address nextHopIP = bestRoute->nextHop.isZero() ? packet.getDestinationIP() : bestRoute->nextHop;

    SimulationLogger::info(
        name + " selected route "
        + bestRoute->toString()
        + " for destination " + packet.getDestinationIP().toString()
    );

    if (!resolveArp(*outgoingInterface, nextHopIP)) {
        SimulationLogger::error(name + " could not resolve next-hop MAC for " + nextHopIP.toString());
        return;
    }

    packet.decrementTTL();
    Address nextHopMac = arpTable[nextHopIP.toString()];
    Frame outboundFrame = Frame::createIPFrame(
        outgoingInterface->macAddress,
        nextHopMac,
        packet,
        nextSequenceNumber
    );

    SimulationLogger::info(
        name + " forwarding packet via " + outgoingInterface->name
        + " to next hop " + nextHopIP.toString()
    );

    outgoingInterface->connectedDevice->receiveFrame(outboundFrame, this);
    nextSequenceNumber++;
}

string Router::getRouteSourceName(RouteSource source) const {
    if (source == CONNECTED_ROUTE) {
        return "CONNECTED";
    }
    if (source == STATIC_ROUTE) {
        return "STATIC";
    }
    return "RIP";
}

void Router::addInterface(string interfaceName, IPv4Address ipAddress, IPv4Address subnetMask, Address macAddress) {
    interfaces.push_back(RouterInterface(interfaceName, ipAddress, subnetMask, macAddress));
    routingTable.push_back(
        RouteEntry(
            ipAddress.getNetworkAddress(subnetMask),
            subnetMask,
            IPv4Address::any(),
            interfaceName,
            0,
            CONNECTED_ROUTE
        )
    );

    SimulationLogger::info(
        name + " added interface " + interfaceName
        + " with IP " + ipAddress.toString()
        + "/" + to_string(subnetMask.getPrefixLength())
    );
}

void Router::connectInterface(string interfaceName, Device* device) {
    RouterInterface* routerInterface = getInterfaceByName(interfaceName);
    if (routerInterface == NULL) {
        SimulationLogger::error(name + " does not have interface " + interfaceName);
        return;
    }

    routerInterface->connectedDevice = device;
    addConnection(device);
}

void Router::addStaticRoute(IPv4Address network, IPv4Address subnetMask, IPv4Address nextHop, string outgoingInterface, int metric) {
    routingTable.push_back(
        RouteEntry(network, subnetMask, nextHop, outgoingInterface, metric, STATIC_ROUTE)
    );

    SimulationLogger::info(
        name + " added static route " + network.toString()
        + "/" + to_string(subnetMask.getPrefixLength())
        + " via " + nextHop.toString()
        + " on " + outgoingInterface
    );
}

void Router::sendRipUpdates() {
    for (int i = 0; i < interfaces.size(); i++) {
        if (interfaces[i].connectedDevice == NULL) {
            continue;
        }

        RoutingUpdate update;
        update.sourceRouterIP = interfaces[i].ipAddress;

        for (int j = 0; j < routingTable.size(); j++) {
            update.routes.push_back(
                RouteAdvertisement(
                    routingTable[j].network,
                    routingTable[j].subnetMask,
                    routingTable[j].metric
                )
            );
        }

        Frame frame = Frame::createRoutingUpdate(
            interfaces[i].macAddress,
            Address::getBroadcastAddress(),
            update,
            nextSequenceNumber
        );

        SimulationLogger::info(name + " sending RIP update on " + interfaces[i].name);
        interfaces[i].connectedDevice->receiveFrame(frame, this);
        nextSequenceNumber++;
    }
}

void Router::printRoutingTable() const {
    cout << "\nRouting Table of " << name << ":\n";
    for (int i = 0; i < routingTable.size(); i++) {
        cout << routingTable[i].toString()
             << " [" << getRouteSourceName(routingTable[i].source) << "]" << endl;
    }
}

void Router::sendFrame(Frame frame, Device* sender) {
    receiveFrame(frame, sender);
}

void Router::receiveFrame(Frame frame, Device* sender) {
    const RouterInterface* ingressInterface = getInterfaceBySender(sender);
    if (ingressInterface == NULL) {
        SimulationLogger::warn(name + " received a frame from an unknown link.");
        return;
    }

    if (!frame.isBroadcast() && getInterfaceByMac(frame.getDestinationMac()) == NULL) {
        SimulationLogger::info(name + " ignored frame not addressed to any router interface.");
        return;
    }

    if (frame.getFrameType() == ARP_REQUEST_FRAME) {
        processArpRequest(frame, *ingressInterface);
        return;
    }

    if (frame.getFrameType() == ARP_REPLY_FRAME) {
        processArpReply(frame);
        return;
    }

    if (frame.getFrameType() == ROUTING_UPDATE_FRAME) {
        processRoutingUpdate(frame, *ingressInterface);
        return;
    }

    processIPFrame(frame);
}
