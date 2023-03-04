src  = $(wildcard src/*.cpp)

server:
	clang++ -pthread \
	-o server \
	$(src) \
	server.cpp

client:
	clang++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp \
	-o client \
	client.cpp

clean:
	rm -f server client