all:cli svr
cli:client.cpp
	g++ $^ -o $@
svr:server.cpp
	g++ $^ -o $@ -lpthread
clear:
	rm -rf cli svr
