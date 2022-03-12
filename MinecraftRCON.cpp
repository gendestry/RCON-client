#include "MinecraftRCON.h"
#include <sstream>
#include <iostream>


void MinecraftRCON::runDefaultInterface() {
    if(!checkConnection()) {
        std::cerr << "Connection not established or incorrect auth data!" << std::endl;
        return;
    }
    std::cout << std::endl << ANSI::underline << ANSI::italic << "Command" << ANSI::reset << ": ";

    std::string cmd;
    while(std::getline(std::cin, cmd)) {
        if(cmd == "q")
            break;
        std::string ret = sendCommand(cmd);
        bool nl = ret.length() > 0 && ret.at(ret.length() - 1) != '\n';

        std::cout << ret << (nl ? "\n" : "") << std::endl;
        std::cout << ANSI::underline << ANSI::italic << "Command" << ANSI::reset << ": ";
    }

    std::cout << ANSI::italic << "Disconnected - Goodbye!" << ANSI::reset << std::endl;
}

std::string MinecraftRCON::constructString(unsigned char* buffer, unsigned int size) {
    return parseString(buffer, size);
}

std::string MinecraftRCON::parseString(unsigned char* buffer, unsigned int size) {
    std::stringstream ss;

    // assuming standard packet format, skip 12 bytes
    for(int i = 12; i < size - 2; i++) {
        if(buffer[i] == 0xc2 && buffer[i+1] == 0xa7) {  // found special character
            char code = buffer[i+2];
            i += 2;
            ss << getMCAnsiCode(code);
        }
        else {
            ss << buffer[i];
        }
    }
    ss << ANSI::reset;
    return ss.str();
}

std::string MinecraftRCON::getMCAnsiCode(char c) {
    static const std::string codes[] = {
        "\x1B[30m", "\x1B[34m", "\x1B[32m", "\x1B[36m", "\x1B[31m", "\x1B[35m", "\x1B[33m", "\x1B[32;2;160;160;160m", 
        "\x1B[32;2;70;70;70m", "\x1B[34m", "\x1B[32m", "\x1B[36m", "\x1B[31m", "\x1B[35m", "\x1B[33m", "\x1B[37m",
        ANSI::inverse, ANSI::bold, ANSI::striked, ANSI::underline, ANSI::italic, ANSI::reset, "\n"
    };

    /*static const std::string codes[] = {
        getAnsiColorCode(0,0,0), getAnsiColorCode(0,0,42), getAnsiColorCode(0,42,0), getAnsiColorCode(0,42,42), 
        getAnsiColorCode(42,0,0), getAnsiColorCode(42,0,42), getAnsiColorCode(42,42,0), getAnsiColorCode(42,42,42),
        
        getAnsiColorCode(21,21,21), getAnsiColorCode(21,21,63), getAnsiColorCode(21,63,21), getAnsiColorCode(21,63,63), 
        getAnsiColorCode(66,21,21), getAnsiColorCode(63,21,63), getAnsiColorCode(63,63,21), getAnsiColorCode(63,63,63), getAnsiColorCode(55, 53, 1),

        "\x1B[7m", "\x1B[1m", "\x1B[9m", "\x1B[4m", "\x1B[3m", "\x1B[0m", "\n"
    };*/

    auto getIndex = [](char c) {
        if(c >= '0' && c <= '9')
            return (int)(c - '0');
        else if(c >= 'a' && c <= 'f')
            return (int)(c - 'a') + 10;
        else if(c >= 'k' && c <= 'o')
            return (int)(c - 'k') + 16;
        else if(c == 'r')
            return 21;
        else if(c == '\n')
            return 22;

        return -1;
    };

    return codes[getIndex(c)];
}