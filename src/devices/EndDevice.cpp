#include "../../include/common/Types.h"
#include "../../include/devices/EndDevice.h"

EndDevice::EndDevice() : Device() {
    accessControl = NULL;
    errorControl = NULL;
    flowControl = NULL;
    transportFlowControl = NULL;
    nextSequenceNumber = 0;
    nextTransportSequenceNumber = 0;
    ipAddress = IPv4Address();
    subnetMask = IPv4Address();
    defaultGateway = IPv4Address();
    networkConfigured = false;
    nextEphemeralPort = 49152;
}

EndDevice::EndDevice(int id, string name, Address macAddress)
    : Device(id, name, END_DEVICE, macAddress) {
    accessControl = NULL;
    errorControl = NULL;
    flowControl = NULL;
    transportFlowControl = NULL;
    nextSequenceNumber = 0;
    nextTransportSequenceNumber = 0;
    ipAddress = IPv4Address();
    subnetMask = IPv4Address();
    defaultGateway = IPv4Address();
    networkConfigured = false;
    nextEphemeralPort = 49152;
}

void EndDevice::setAccessControl(IAccessControl* accessControl) {
    this->accessControl = accessControl;
}

void EndDevice::setErrorControl(IErrorControl* errorControl) {
    this->errorControl = errorControl;
}

void EndDevice::setFlowControl(IFlowControl* flowControl) {
    this->flowControl = flowControl;
}

void EndDevice::setTransportFlowControl(IFlowControl* transportFlowControl) {
    this->transportFlowControl = transportFlowControl;
}

void EndDevice::configureNetwork(IPv4Address ipAddress, IPv4Address subnetMask, IPv4Address defaultGateway) {
    this->ipAddress = ipAddress;
    this->subnetMask = subnetMask;
    this->defaultGateway = defaultGateway;
    networkConfigured = true;

    SimulationLogger::info(
        name + " configured with IP " + ipAddress.toString()
        + "/" + to_string(subnetMask.getPrefixLength())
        + " gateway " + defaultGateway.toString()
    );
}

IPv4Address EndDevice::getIpAddress() const {
    return ipAddress;
}

IPv4Address EndDevice::getSubnetMask() const {
    return subnetMask;
}

void EndDevice::registerApplication(ApplicationService* applicationService) {
    if (applicationService == NULL) {
        return;
    }

    applicationServices[applicationService->getWellKnownPort()] = applicationService;
    SimulationLogger::info(
        name + " bound application " + applicationService->getServiceName()
        + " to well-known port " + to_string(applicationService->getWellKnownPort())
    );
}

int EndDevice::openEphemeralPort(const string& processName) {
    int allocatedPort = nextEphemeralPort;
    nextEphemeralPort++;
    clientProcesses[allocatedPort] = processName;

    SimulationLogger::info(
        name + " assigned ephemeral port " + to_string(allocatedPort)
        + " to process " + processName
    );

    return allocatedPort;
}

bool EndDevice::transmitFrameInternal(Frame& frame, bool applyFlowControl, const string& preparationMessage) {
    SimulationLogger::info(name + " is preparing to send " + preparationMessage + ": " + frame.toString());

    if (errorControl != NULL) {
        errorControl->applyErrorControl(frame);
    }

    if (accessControl != NULL) {
        if (!accessControl->canTransmit(frame)) {
            SimulationLogger::warn(name + " could not send frame due to access control.");
            return false;
        }
    }

    if (applyFlowControl && flowControl != NULL) {
        if (!flowControl->sendWithFlowControl(frame)) {
            SimulationLogger::warn(name + " could not send frame due to flow control.");
            return false;
        }
    }

    if (connections.size() == 0) {
        SimulationLogger::error(name + " has no connection to send data.");
        return false;
    }

    SimulationLogger::info(name + " is sending frame: " + frame.toString());
    connections[0]->receiveFrame(frame, this);
    return true;
}

bool EndDevice::sendIPPacket(const IPPacket& packet) {
    if (!networkConfigured) {
        SimulationLogger::error(name + " does not have IPv4 configuration.");
        return false;
    }

    IPv4Address destinationIP = packet.getDestinationIP();
    IPv4Address nextHopIP = destinationIP;

    if (!ipAddress.isInSameSubnet(destinationIP, subnetMask)) {
        if (defaultGateway.isZero()) {
            SimulationLogger::error(name + " has no default gateway to reach " + destinationIP.toString());
            return false;
        }

        nextHopIP = defaultGateway;
        SimulationLogger::info(
            name + " determined destination " + destinationIP.toString()
            + " is remote, using default gateway " + nextHopIP.toString()
        );
    } else {
        SimulationLogger::info(name + " determined destination " + destinationIP.toString() + " is on the local subnet.");
    }

    if (!resolveArp(nextHopIP)) {
        SimulationLogger::error(name + " could not resolve ARP for " + nextHopIP.toString());
        return false;
    }

    Address nextHopMac = arpTable[nextHopIP.toString()];
    Frame frame = Frame::createIPFrame(macAddress, nextHopMac, packet, nextSequenceNumber);

    if (transmitFrameInternal(frame, true, "IP packet")) {
        nextSequenceNumber++;
        return true;
    }

    return false;
}

bool EndDevice::resolveArp(const IPv4Address& targetIP) {
    string arpKey = targetIP.toString();

    if (arpTable.find(arpKey) != arpTable.end()) {
        return true;
    }

    if (connections.size() == 0) {
        SimulationLogger::error(name + " has no connection to perform ARP.");
        return false;
    }

    SimulationLogger::info(name + " broadcasting ARP request for IP " + targetIP.toString());
    Frame arpRequest = Frame::createArpRequest(macAddress, ipAddress, targetIP, nextSequenceNumber);
    if (!transmitFrameInternal(arpRequest, false, "ARP request")) {
        return false;
    }

    return arpTable.find(arpKey) != arpTable.end();
}

bool EndDevice::isForThisHost(const IPv4Address& destinationIP) const {
    return networkConfigured && destinationIP == ipAddress;
}

void EndDevice::printArpTable() const {
    cout << "\nARP Table of " << name << ":\n";
    for (map<string, Address>::const_iterator it = arpTable.begin(); it != arpTable.end(); it++) {
        cout << it->first << " -> " << it->second.getMacAddress() << endl;
    }
}

void EndDevice::sendData(string data, Address destinationMac, bool isFrame) {
    Frame frame(macAddress, destinationMac, data, nextSequenceNumber);
    string preparationMessage = isFrame ? "frame" : "ACK frame";
    if (transmitFrameInternal(frame, true, preparationMessage)) {
        nextSequenceNumber++;
    }
}

void EndDevice::sendIPData(string data, IPv4Address destinationIP) {
    IPPacket packet(ipAddress, destinationIP, data, 16);
    sendIPPacket(packet);
}

void EndDevice::sendApplicationData(
    IPv4Address destinationIP,
    int sourcePort,
    int destinationPort,
    const ApplicationMessage& applicationMessage,
    TransportProtocol protocol
) {
    TransportSegment segment(
        sourcePort,
        destinationPort,
        nextTransportSequenceNumber,
        protocol,
        applicationMessage
    );

    SimulationLogger::info(
        name + " application layer generated message: " + applicationMessage.toString()
    );
    SimulationLogger::info(
        name + " transport layer encapsulated message into segment: " + segment.toString()
    );

    IPPacket packet(ipAddress, destinationIP, segment, 16);
    Frame transportFrame = Frame::createIPFrame(macAddress, Address::getBroadcastAddress(), packet, nextSequenceNumber);

    if (transportFlowControl != NULL) {
        SimulationLogger::info(
            name + " transport layer applying sliding window protocol "
            + transportFlowControl->getProtocolName()
        );

        if (!transportFlowControl->sendWithFlowControl(transportFrame)) {
            SimulationLogger::warn(name + " transport layer blocked the segment transmission.");
            return;
        }
    }

    if (sendIPPacket(packet)) {
        nextTransportSequenceNumber++;
    }
}

void EndDevice::processTransportSegment(const IPPacket& packet) {
    TransportSegment segment = packet.getTransportSegment();
    SimulationLogger::info(
        name + " transport layer decapsulated segment: " + segment.toString()
    );

    if (segment.acknowledgement) {
        cout << name << " received transport ACK for port "
             << segment.destinationPort
             << " from " << packet.getSourceIP().toString() << endl;
        return;
    }

    if (applicationServices.find(segment.destinationPort) != applicationServices.end()) {
        ApplicationService* service = applicationServices[segment.destinationPort];
        SimulationLogger::info(
            name + " delivering segment to application service "
            + service->getServiceName()
            + " on port " + to_string(segment.destinationPort)
        );

        ApplicationMessage response = service->handleRequest(segment.applicationMessage, name);
        SimulationLogger::info(
            name + " application layer produced response: " + response.toString()
        );

        sendApplicationData(
            packet.getSourceIP(),
            segment.destinationPort,
            segment.sourcePort,
            response,
            segment.protocol
        );
        return;
    }

    if (clientProcesses.find(segment.destinationPort) != clientProcesses.end()) {
        cout << name << " delivered application response to process "
             << clientProcesses[segment.destinationPort]
             << " on port " << segment.destinationPort
             << ": " << segment.applicationMessage.toString()
             << " (from " << packet.getSourceIP().toString() << ")" << endl;
        return;
    }

    SimulationLogger::warn(
        name + " has no process bound to destination port "
        + to_string(segment.destinationPort)
    );
}

void EndDevice::sendFrame(Frame frame, Device* sender) {
    if (connections.size() == 0) {
        SimulationLogger::error(name + " has no connection to send frame.");
        return;
    }

    connections[0]->receiveFrame(frame, this);
}

void EndDevice::receiveFrame(Frame frame, Device* sender) {
    if (frame.getDestinationMac() != macAddress && !frame.isBroadcast()) {
        SimulationLogger::info(name + " ignored frame not meant for it.");
        return;
    }

    if (frame.getFrameType() == ARP_REQUEST_FRAME) {
        ArpMessage arpMessage = frame.getArpMessage();
        arpTable[arpMessage.senderIP.toString()] = arpMessage.senderMac;
        SimulationLogger::info(name + " learned ARP mapping " + arpMessage.senderIP.toString() + " -> " + arpMessage.senderMac.getMacAddress());

        if (isForThisHost(arpMessage.targetIP)) {
            SimulationLogger::info(name + " is replying to ARP request for " + arpMessage.targetIP.toString());
            Frame arpReply = Frame::createArpReply(macAddress, arpMessage.senderMac, ipAddress, arpMessage.senderIP, arpMessage.senderMac, nextSequenceNumber);
            if (transmitFrameInternal(arpReply, false, "ARP reply")) {
                nextSequenceNumber++;
            }
        }
        return;
    }

    if (frame.getFrameType() == ARP_REPLY_FRAME) {
        ArpMessage arpMessage = frame.getArpMessage();
        arpTable[arpMessage.senderIP.toString()] = arpMessage.senderMac;
        SimulationLogger::info(name + " updated ARP cache: " + arpMessage.senderIP.toString() + " -> " + arpMessage.senderMac.getMacAddress());
        return;
    }

    if (frame.getFrameType() == ROUTING_UPDATE_FRAME) {
        SimulationLogger::info(name + " ignored routing update frame.");
        return;
    }

    if (errorControl != NULL) {
        if (!errorControl->checkError(frame)) {
            SimulationLogger::error(name + " discarded frame due to parity error.");
            return;
        }
    }

    SimulationLogger::info(name + " received frame: " + frame.toString());

    if (frame.getFrameType() == IP_FRAME) {
        IPPacket packet = frame.getIPPacket();
        if (!isForThisHost(packet.getDestinationIP())) {
            SimulationLogger::info(name + " ignored IP packet addressed to " + packet.getDestinationIP().toString());
            return;
        }

        if (packet.containsTransportSegment()) {
            processTransportSegment(packet);
            return;
        }

        cout << name << " received IP data: " << packet.getPayload()
             << " (from " << packet.getSourceIP().toString() << ")" << endl;
        return;
    }

    cout << name << " received data: " << frame.getPayload() << endl;
}
