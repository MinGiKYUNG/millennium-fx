CC=/usr/local/arm-linux-4.1.1/bin/arm-linux-gcc

CFLAGS += -mcpu=iwmmxt -mtune=iwmmxt -mabi=aapcs-linux  \
		-I./ -DDEBUG_BUILD

SOURCES = camera.c main.c overlay2.c
OBJECTS = ${SOURCES:.c=.o}

LFLAGS += -lpthread

OUT = camera_yuv_capture

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJECTS) $(LIBS)

clean: 
	rm -f camera *.o
