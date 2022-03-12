# Simple RCON Client

A simple rcon client implementation written in C++.  
Implemented to be used on minecraft servers, but can be used elsewhere.

If you want just a RCON client then use the `RCON` class, otherwise you can use the `MinecraftRCON` class which adds support for their escape characters.


## Configuration & Building

Check the *main.cpp* to set your credentials and server location.  
To build just run `make`. There is also a way to install it to `/bin/mc-rcon` by running `make install`. (You can also change the install location).