#pragma once
#include "ANSI.h"
#include <string>


struct RCONPacket {
    unsigned char* data;
    unsigned int size;
};

class RCON {
private:
    int _socket;
    bool _auth = false;
public:
    RCON();
    RCON(std::string host, int port);
    RCON(std::string host, int port, std::string pass);

    ~RCON();

    bool connectRCON(std::string host, int port);
    bool authenticate(std::string password);
    void disconnectRCON();
    inline bool checkConnection() { return _auth && _socket != 0; }

    std::string sendCommand(const std::string& cmd, bool print = false);
    std::string readResponse();
private: // maybe protected?
    RCONPacket createPacket(std::string cmd, int type);
    void destroyPacket(RCONPacket& packet);
    void printPacket(RCONPacket& packet);
    void printPacketHex(RCONPacket& packet);

    
    virtual std::string constructString(unsigned char* data, unsigned int size);
};