/*
 ============================================================================
 Name        : serial.h
 Author      : Team KKY
 Copyright   : 2012 Embedded Contest
 ============================================================================
 */
 
#ifndef SERIAL_H_
#define SERIAL_H_

#include <fcntl.h>
#include <termios.h>

//	define code of the motors in car
#define DM_ANGLE	0xF4
#define DM_SPEED	0xF5
#define CM_ANGLE	0xF6
#define CM_SPEED	0xF7

int fd_serial;

//	open serial
int open_serial() {
	struct termios oldtio, newtio;

	fd_serial = open("/dev/ttyS2", O_RDWR | O_NOCTTY);
	if (fd_serial < 0)
		return 0;

	tcgetattr(fd_serial, &oldtio);

	newtio.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd_serial, TCIFLUSH);
	tcsetattr(fd_serial, TCSANOW, &newtio);

	return 1;
}

//	close serial
void close_serial() {
	if (close(fd_serial) < 0) {
		printf("Failed to close serial port.\n");
	}
}

//	setting motor with value(data)
void setting_hl(unsigned char code, unsigned int data) {
	unsigned char high = (data >> 8) & 0xFF;
	unsigned char low = data & 0xFF;
	write(fd_serial, &code, 1);
	write(fd_serial, &high, 1);
	write(fd_serial, &low, 1);
	usleep(11000);
}

#endif /* SERIAL_H_ */
