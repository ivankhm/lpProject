S_SOURCES = FileManager/*.cpp
C_SOURCES = FileManagerClient/*.cpp
S_EX_NAME = Server.o
C_EX_NAME = Client.o
FLAGS = -Wall -std=c++11
COMPILE = g++

all: server.o client.o

server.o:
	$(COMPILE) $(FLAGS) -o $(S_EX_NAME) $(S_SOURCES)

client.o:
	$(COMPILE) $(FLAGS) -o $(C_EX_NAME) $(C_SOURCES)

clean:
	rm -rf *.o