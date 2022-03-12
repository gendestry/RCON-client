#include "RCON.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <string.h>
#include <sstream>

// RCON packet types
#define SERVERDATA_RESPONSE_VALUE 0
#define SERVERDATA_EXECCOMMAND 2
#define SERVERDATA_AUTH_RESPONSE 2
#define SERVERDATA_AUTH 3

// RCON packet structure
#define PSIZE_OFFSET 0
#define PID_OFFSET 4
#define PTYPE_OFFSET 8
#define PBODY_OFFSET 12

/*
default packet struct:
 - uint32_t  packet-size     4B
 - uint32_t  packet-id       4B
 - uint32_t  packet-type     4B
 - char*     packet-data     minimum 1B '\0'
 - char      null            1B '\0'
*/

RCON::RCON() {}

RCON::RCON(std::string host, int port) {
    if(connectRCON(host, port))
        std::cout << "Connected to " << ANSI::italic << host << ":" << port << ANSI::reset << std::endl;
    else 
        std::cout << ANSI::fred << "Connection to " << ANSI::italic << host << ":" << port << ANSI::reset <<  ANSI::fred <<  " failed!" <<  ANSI::reset << std::endl;
}

RCON::RCON(std::string host, int port, std::string pass) {
    if(connectRCON(host, port))
        std::cout << "Connected to " << ANSI::italic << host << ":" << port << ANSI::reset << std::endl;
    else {
        std::cout << ANSI::fred << "Connection to " << ANSI::italic << host << ":" << port << ANSI::reset <<  ANSI::fred <<  " failed!" <<  ANSI::reset <<std::endl;
        return;
    }

    if(authenticate(pass))
        std::cout << "Access granted!" << std::endl;
    else
        std::cout << ANSI::fred << "Incorrect password!" << ANSI::reset << std::endl;
}

RCON::~RCON() {
    disconnectRCON();
}

bool RCON::connectRCON(std::string host, int port) {
    sockaddr_in serv_addr;
    hostent *server;

    _socket = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(host.c_str());

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if(connect(_socket, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        return false;

    return true;
}

bool RCON::authenticate(std::string pass) {
    _auth = false;
    if(_socket == 0)
        return false;
    auto packet = createPacket(pass, SERVERDATA_AUTH);
    write(_socket, packet.data, packet.size);
    destroyPacket(packet);

    unsigned char buffer[14];
    memset(buffer, 0, 14);
    read(_socket, buffer, 14);

    if(*(int*)(buffer + PID_OFFSET) < 0)
        return false;

    _auth = true;
    return true;
}

void RCON::disconnectRCON() {
	close(_socket);
}

RCONPacket RCON::createPacket(std::string cmd, int type) {
    unsigned int size = cmd.length() + 14;
    unsigned char* data = new unsigned char[size];
    memset(data, 0, size);

    data[PSIZE_OFFSET] = cmd.length() + 10;
    data[PID_OFFSET] = 1;
    data[PTYPE_OFFSET] = type;
    for(int i = 0; i < cmd.length(); i++)
        data[PBODY_OFFSET + i] = (unsigned char)cmd.at(i);
    
    RCONPacket packet;
    packet.data = data;
    packet.size = size;

    return packet;
}

void RCON::destroyPacket(RCONPacket& packet) {
    delete packet.data;
}

void RCON::printPacket(RCONPacket& packet) {
    printPacketHex(packet);
}

void RCON::printPacketHex(RCONPacket& packet) {
    std::cout << std::hex;
    for(int i = 0; i < packet.size; i++)
        std::cout << (int)packet.data[i] << " ";
    
    std::cout << std::dec << std::endl;
}

std::string RCON::sendCommand(const std::string& cmd, bool print) {
    if(!checkConnection())
        return "";
    auto packet = createPacket(cmd, SERVERDATA_EXECCOMMAND);
    write(_socket, packet.data, packet.size);
    destroyPacket(packet);

    if(print) {
        std::string ret = readResponse();
        std::cout << ret << std::endl;
        return ret;
    }
    
    return readResponse();
}

std::string RCON::readResponse() {
    if(!checkConnection())
        return "";
    // Get packet size
    unsigned char bsize[4];
    memset(bsize, 0, 4);
    read(_socket, bsize, 4);

    int size = *(int*)bsize + 4;

    // Get response
    unsigned char buffer[size];
    memset(buffer, 0, size);
    read(_socket, buffer + 4, size);
    memcpy(buffer, bsize, 4);

    return constructString(buffer, size);
}

std::string RCON::constructString(unsigned char* buffer, unsigned int size) {
    return std::string((char*)(buffer + 12));
}
