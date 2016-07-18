

RPI = RpiScreen.o RpiKeyboard.o RpiTiming.o


CC = g++

CFLAGS = -Wall -g
LFLAGS = -Wall


clean:
	$(RM) *.o src/*.o

