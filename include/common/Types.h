#ifndef TYPES_H
#define TYPES_H
using namespace std;

enum DeviceType {
    END_DEVICE,
    HUB,
    BRIDGE,
    SWITCH_DEVICE,
    ROUTER_DEVICE
};

enum FrameType {
    DATA_FRAME,
    IP_FRAME,
    ARP_REQUEST_FRAME,
    ARP_REPLY_FRAME,
    ROUTING_UPDATE_FRAME
};

enum RouteSource {
    CONNECTED_ROUTE,
    STATIC_ROUTE,
    RIP_ROUTE
};

enum TransportProtocol {
    UDP_PROTOCOL,
    TCP_GBN_PROTOCOL
};

#endif
