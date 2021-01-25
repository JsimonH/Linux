all:cli svr
cli:client.cpp
	g++ $^ -o $@
svr:server.cpp
	g++ $^ -o $@
clear:
	rm -rf cli svr
