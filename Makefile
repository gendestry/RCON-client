output = main
input =  RCON.cpp ANSI.cpp MinecraftRCON.cpp main.cpp

.phony: all clean install

all: $(output)

$(output): $(input)
	g++ -std=c++17 $^ -o $@

clean:
	rm $(output)

install:
	sudo cp $(output) /bin/mc-rcon