#defualt engine
RPI = $(patsubst %, src/%, RpiScreen.o RpiKeyboard.o RpiTiming.o)

ENGINE = $(RPI)


CC = g++

CFLAGS = -Wall -g
LFLAGS = -Wall

engine: $(ENGINE)
	$(CC) $(CFLAGS) $(ENGINE:.o=.cpp) src/engine.cpp -o build/engine


rpi: ENGINE = $(RPI)
rpi: engine

RpiScreen.o: src/RpiScreen.cpp src/RpiScreen.h src/Screen.h

clean:
	$(RM) *.o src/*.o -v
	$(RM) build/* -v
