CC=/usr/local/arm-linux-4.1.1/bin/arm-linux-gcc

CFLAGS += -mcpu=iwmmxt -mtune=iwmmxt -mabi=aapcs-linux  \
		-I./ -DDEBUG_BUILD -I/usr/local/opencv/include/opencv

SOURCES = camera.c main.c overlay2.c
OBJECTS = ${SOURCES:.c=.o}

LIBS = -L/usr/local/opencv/lib

LFLAGS += -lpthread -lcxcore -lcv -lhighgui -lcvaux -lml

OUT = ctest

$(OUT): $(OBJECTS)
	$(CC) $(LIBS) $(LFLAGS) $(CFLAGS) -o $(OUT) $(OBJECTS)

clean: 
	rm -f ctest *.o
