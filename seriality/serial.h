// Seriality: Serial Connectivity Plug-in
// Copyright 2010 Nicholas Zambetti
// Creative Commons Attribution-Share Alike 3.0
//
// Adapted from arduino_serial.c by Tod E. Kurt, tod@todbot.com
// Copyleft 2006
//

#ifndef SERIAL_H
#define SERIAL_H

  #define SERIAL_SUCCESS 0
  #define SERIAL_FAIL -1
    
  void serialOpen(const char* path, int speed);
  void serialClose();
  void serialWrite(const char* out, int length);
  void serialWriteByte(char out);
  char serialRead();
  int serialStatus();
  int serialAvailable();
  int serialGetSpeed();

#endif
