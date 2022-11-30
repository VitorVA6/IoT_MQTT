all: sbc

sbc: sbc.c
	gcc -o sbc sbc.c -lpaho-mqtt3c -lwiringPi -lwiringPiDev -lcrypt -lm -lrt -Wall

clean:
	rm sbc