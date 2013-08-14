#include <stdio.h>

#include "serial.h"

int main()
{
	serialOpen("/dev/tty.usbserial-A4001o0F", 9600);
  serialWriteByte(0x31);
	//serialport_write("1");
	usleep(100 * 1000);
	printf("available: %d \n", serialAvailable());
	printf("read: %c \n", serialRead());
	printf("available: %d \n", serialAvailable());
	printf("read: %c \n", serialRead());
	printf("available: %d \n", serialAvailable());
	serialClose();
	return 0;
}
