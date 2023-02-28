server:
	g++ utils.cpp client.cpp NetAddress.cpp Socket.cpp -o client && \
	g++ utils.cpp server.cpp Epoll.cpp NetAddress.cpp Socket.cpp -o server
clean:
	rm server && rm client
