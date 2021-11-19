CXX=g++

CFLAGS=-g -lpthread

all: client server

server: ./src/server/*.cpp ./src/server/*.h ./src/*.cpp ./src/*.h
	$(CXX) $(CFLAGS) -o server ./src/*.cpp ./src/server/*.cpp

client: ./src/client/*.cpp ./src/client/*.h ./src/*.cpp ./src/*.h
	$(CXX) $(CFLAGS) -o client ./src/*.cpp ./src/client/*.cpp

clean:
	rm -f server client *.o
