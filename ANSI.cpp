#include "ANSI.h"
#include <sstream>

const std::string ANSI::reset = "\x1B[0m";
const std::string ANSI::bold = "\x1B[1m";
const std::string ANSI::faint = "\x1B[2m";
const std::string ANSI::italic = "\x1B[3m";
const std::string ANSI::underline = "\x1B[4m";
const std::string ANSI::inverse = "\x1B[5m";
const std::string ANSI::striked = "\x1B[6m";

const std::string ANSI::fblack = "\x1B[30m";
const std::string ANSI::fred = "\x1B[31m";
const std::string ANSI::fgreen = "\x1B[32m";
const std::string ANSI::fyellow = "\x1B[33m";
const std::string ANSI::fblue = "\x1B[34m";
const std::string ANSI::fmagenta = "\x1B[35m";
const std::string ANSI::fcyan = "\x1B[36m";
const std::string ANSI::fwhite = "\x1B[37m";

const std::string ANSI::bblack = "\x1B[40m";
const std::string ANSI::bred = "\x1B[41m";
const std::string ANSI::bgreen = "\x1B[42m";
const std::string ANSI::byellow = "\x1B[43m";
const std::string ANSI::bblue = "\x1B[44m";
const std::string ANSI::bmagenta = "\x1B[45m";
const std::string ANSI::bcyan = "\x1B[46m";
const std::string ANSI::bwhite = "\x1B[47m";

std::string ANSI::byColorCode(int r, int g, int b, bool fg) {
    std::stringstream ss;
    ss << "\x1B[" << (fg ? 3 : 4) << "8;2;" << r << ";" << g << ";"<< b << "m";
    return ss.str();
} 