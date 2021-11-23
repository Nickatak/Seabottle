.PHONY: build
build:
	arm-linux-gnueabi-g++ -march=armv6 -Iincludes src/seabottle.cpp -o temp/seabottle.o -c
	arm-linux-gnueabi-g++ -march=armv6 -Iincludes src/main.cpp -o temp/main.o -c
	arm-linux-gnueabi-g++ -march=armv6 -o main temp/main.o temp/seabottle.o
