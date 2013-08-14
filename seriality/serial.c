// Seriality: Serial Connectivity Plug-in
// Copyright 2010 Nicholas Zambetti
// Creative Commons Attribution-Share Alike 3.0
//
// Adapted from arduino_serial.c by Tod E. Kurt, tod@todbot.com
// Copyleft 2006
//

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "serial.h"

static int file;
static int status;

void serialOpen(const char* path, int speed)
{
  struct termios toptions;
  speed_t baud;

  status = SERIAL_FAIL;

  file = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
  if (file == -1) {
    return;
  }

  if (tcgetattr(file, &toptions) < 0) {
    return;
  }

  switch (speed) {
    case 1200:
      baud = B1200; break;
    case 1800:
      baud = B1800; break;
    case 2400:
      baud = B2400; break;
    case 4800:
      baud = B4800; break;
    case 9600:
      baud = B9600; break;
    case 19200:
      baud = B19200; break;
    case 38400:
      baud = B38400; break;
    case 57600:
      baud = B57600; break;
    case 115200:
      baud = B115200; break;
    case 230400:
      baud = B230400; break;
    default:
      baud = B9600; break;
  }
  cfsetispeed(&toptions, baud);
  cfsetospeed(&toptions, baud);
  
  // 8N1
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;

  // disable flow control
  toptions.c_cflag &= ~CRTSCTS;
  toptions.c_iflag &= ~(IXON | IXOFF | IXANY);

  // enable read & disable ctrl lines
  toptions.c_cflag |= CREAD | CLOCAL;

  // raw mode with a timeout and no char minimum
  toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  toptions.c_oflag &= ~OPOST;
  toptions.c_cc[VMIN] = 0;
  toptions.c_cc[VTIME] = 20;
  
  if (tcsetattr(file, TCSANOW, &toptions) < 0) {
    return;
  }

  status = SERIAL_SUCCESS;
}

void serialClose()
{
  close(file);
}

void serialWrite(const char* out, int length)
{
  status = SERIAL_FAIL;
  if (length != write(file, out, length)) {
    return;
  }
  status = SERIAL_SUCCESS;
}

void serialWriteByte(char out)
{
  status = SERIAL_FAIL;
  if (1 != write(file, &out, 1)) {
    return;
  }
  status = SERIAL_SUCCESS;
}

char serialRead()
{
  char in;
  status = SERIAL_FAIL;
  if (0 >= read(file, &in, 1)) {
    return 0;
  }
  status = SERIAL_SUCCESS;
  return in;
}

int serialAvailable()
{
  int count = 0;
  ioctl(file, FIONREAD, &count);
  return count;
}

int serialStatus()
{
  return status;
}

int serialGetSpeed()
{
  struct termios toptions;
  speed_t baud;
  int speed = 0;
  if (tcgetattr(file, &toptions) < 0) {
    return 0;
  }
  if (cfgetospeed(&toptions) != cfgetispeed(&toptions)) {
    return 0;
  }
  baud = cfgetospeed(&toptions);
  switch (baud) {
    case B1200:
      speed = 1200; break;
    case B1800:
      speed = 1800; break;
    case B2400:
      speed = 2400; break;
    case B4800:
      speed = 4800; break;
    case B9600:
      speed = 9600; break;
    case B19200:
      speed = 19200; break;
    case B38400:
      speed = 38400; break;
    case B57600:
      speed = 57600; break;
    case B115200:
      speed = 115200; break;
    case B230400:
      speed = 230400; break;
  }
  return speed;
}