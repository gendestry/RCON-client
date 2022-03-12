#pragma once
#include "RCON.h"

// Minecraft RCON client

class MinecraftRCON : public RCON {
public:
    MinecraftRCON() : RCON() {}
    MinecraftRCON(std::string host, int port) : RCON(host, port) {}
    MinecraftRCON(std::string host, int port, std::string pass) : RCON(host, port, pass) {}

    void runDefaultInterface();
private:
    std::string constructString(unsigned char* buffer, unsigned int size) override;
    std::string parseString(unsigned char* buffer, unsigned int size);
    std::string getMCAnsiCode(char c);
};