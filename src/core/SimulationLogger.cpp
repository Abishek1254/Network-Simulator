#include "../../include/core/SimulationLogger.h"

void SimulationLogger::log(string message) {
    cout << message << endl;
}

void SimulationLogger::info(string message) {
    cout << "[INFO] " << message << endl;
}

void SimulationLogger::warn(string message) {
    cout << "[WARN] " << message << endl;
}

void SimulationLogger::error(string message) {
    cout << "[ERROR] " << message << endl;
}