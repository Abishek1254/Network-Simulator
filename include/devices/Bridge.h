#ifndef BRIDGE_H
#define BRIDGE_H

#include <iostream>
#include <string>
#include <map>
#include "Device.h"
#include "../core/SimulationLogger.h"
using namespace std;

class Bridge : public Device {
private:
    map<string, Device*> macTable;

public:
    Bridge();
    Bridge(int id, string name);

    void sendFrame(Frame frame, Device* sender = NULL);
    void receiveFrame(Frame frame, Device* sender = NULL);
};

#endif