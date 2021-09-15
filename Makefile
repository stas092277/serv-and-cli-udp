PORT = 53035
IDADRESS = 192.168.50.22
MAINDIR = /Users/staschebykin/Desktop/test/
COMMAND = rm
NAMEFILE = text.txt
FILEPATH = dir1


.PHONY: all clean

all: runserv

clean:
			rm -rf cli serv
cli: cli.cpp
			g++ -o cli cli.cpp
serv: serv.cpp
			g++ -o serv serv.cpp
runserv: serv
			./serv $(MAINDIR) $(PORT)
runcli: cli
			./cli $(IDADRESS) $(PORT) $(COMMAND) $(FILEPATH) $(NAMEFILE)
