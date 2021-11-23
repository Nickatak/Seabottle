.PHONY: build
build:
	arm-linux-gnueabi-g++ -Iincludes src/socket_handler.cpp -o temp/socket_handler.o -c
	arm-linux-gnueabi-g++ -Iincludes src/seabottle.cpp -o temp/seabottle.o -c
	arm-linux-gnueabi-g++ -Iincludes src/main.cpp -o temp/main.o -c
	arm-linux-gnueabi-g++ -march=armv6 -pthread -o main temp/main.o temp/seabottle.o temp/socket_handler.o
