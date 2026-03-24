#ifndef SIMULATION_LOGGER_H
#define SIMULATION_LOGGER_H

#include <iostream>
#include <string>
using namespace std;

class SimulationLogger {
public:
    static void log(string message);
    static void info(string message);
    static void warn(string message);
    static void error(string message);
};

#endif