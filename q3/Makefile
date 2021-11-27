CXX=g++

CFLAGS=-g -pthread

all: client server

# server: ./src/server/*.cpp ./src/server/*.h ./src/*.cpp ./src/*.h
server: ./src/server/*.cpp ./src/*.cpp
	$(CXX) $(CFLAGS) -o server ./src/*.cpp ./src/server/*.cpp

# client: ./src/client/*.cpp ./src/client/*.h ./src/*.cpp ./src/*.h
client: ./src/client/*.cpp ./src/*.cpp
	$(CXX) $(CFLAGS) -o client ./src/*.cpp ./src/client/*.cpp

clean:
	rm -f server client *.o
