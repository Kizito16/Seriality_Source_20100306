#include <iostream>

#include "seriality.h"

using namespace std;

int main ()
{
  Seriality serial;
  serial.begin("/dev/tty.usbserial-A4001o0F", 9600);
	cout << serial.ports().front();
  //serial.writeByte(0x31);
	serial.write("1");
	serial.write(24.7);
	usleep(100 * 1000);
	cout << "available:" << serial.available() << "\n";
	cout << serial.read() << "\n";
  //cout << serial.readByte() << "\n";
	cout << "available:" << serial.available() << "\n";
	cout << serial.read() << "\n";
  //cout << serial.readByte() << "\n";
	cout << "available:" << serial.available() << "\n";
	serial.end();
	return 0;
}