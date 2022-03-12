#include "MinecraftRCON.h"
#include <iostream>

int main() {
    MinecraftRCON rcon("192.168.64.150", 25575, "rcon");
    rcon.runDefaultInterface();

    return 0;
}
