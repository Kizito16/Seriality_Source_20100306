// Seriality: Serial Connectivity Plug-in
// Copyright 2010 Nicholas Zambetti
// Creative Commons Attribution-Share Alike 3.0
//
// Based on original sources from the Google Nixysa Project
// Copyright 2009 Google Inc.
// Apache License, Version 2.0
//

extern "C" {
  #include "serial.h"
}

#include <string>
#include <sstream>

#include "seriality.h"

Seriality::Seriality()
{
  api_version_ = 1.2;
  port_ = "";
  speed_ = 0;
  refreshPorts();
}

bool Seriality::begin(std::string port, int speed)
{
  char* portChars = new char[port.size() + 1];
  strncpy(portChars, port.c_str(), port.size());
  portChars[port.size()] = '\0';
  if (0 < port_.length()) {
    return false;
  }
  serialOpen(portChars, speed);
  delete portChars;
  if (SERIAL_FAIL == serialStatus()) {
    return false;
  }
  port_ = port;
  speed_ = serialGetSpeed();
  return true;
}

bool Seriality::begin(std::string port)
{
  return begin(port, 9600);
}

void Seriality::end()
{
  port_ = "";
  speed_ = 0;
  serialClose();
}

int Seriality::readByte()
{
  int out = serialRead();
  out &= 0xFF;
  return out;
}

std::string Seriality::read()
{
  std::string in = "";
  char inChar = serialRead();
  if (SERIAL_SUCCESS == serialStatus()) {
    in.push_back(inChar);
  }
  return in;
}

std::string Seriality::readLine()
{
  std::string in = "";
  char inChar;
  while (1) {
    inChar = serialRead();
    if (SERIAL_SUCCESS != serialStatus()) {
      continue;
    }
    if ('\r' == inChar) {
      continue;
    }
    if ('\n' == inChar) {
      if (0 == in.length()) {
        continue;
      }
      break;
    }
    in.push_back(inChar);
  }
  return in;
}

std::string Seriality::readJSON()
{
  std::string in = "";
  int braces = 0;
  char inChar;
  while (1) {
    inChar = serialRead();
    if (SERIAL_SUCCESS != serialStatus()) {
      continue;
    }
    if ((0 == in.length()) && ('{' != inChar)) {
      in = "";
      return in;
    }
    if ('{' == inChar) {
      ++braces;
    }
    if ('}' == inChar) {
      --braces;
    }
    if (0 > braces) {
      in = "";
      return in;
    }
    in.push_back(inChar);
    if (0 == braces) {
      break;
    }
  }
  return in;
}

std::string Seriality::readUntil(int until)
{
  std::string in = "";
  char inChar;
  char untilChar = (char) until;
  while (1) {
    inChar = serialRead();
    if (SERIAL_SUCCESS != serialStatus()) {
      continue;
    }
    if (inChar == untilChar) {
      break;
    }
    in.push_back(inChar);
  }
  return in;
}

bool Seriality::writeByte(int out)
{
  serialWrite((const char*) &out, 1);
  if (SERIAL_FAIL == serialStatus()) {
    return false;
  }
  return true;
}

bool Seriality::write(std::string out)
{
  int length = out.size();
  serialWrite(out.c_str(), length);
  if (SERIAL_FAIL == serialStatus()) {
    return false;
  }
  return true;
}

bool Seriality::write(int out)
{
  std::stringstream ss;
  ss << out;
  return write(ss.str());
}

bool Seriality::write(float out)
{
  std::stringstream ss;
  ss << out;
  return write(ss.str());
}

bool Seriality::write(double out)
{
  std::stringstream ss;
  ss << out;
  return write(ss.str());
}

int Seriality::available()
{
  return serialAvailable();
}

void Seriality::refreshPorts()
{
  char* command = "ls /dev/tty.*";
  char line[128];
  FILE* pipe;

  while (!ports_.empty()) {
    ports_.pop_back();
  }

  if (!(pipe = (FILE*)popen(command,"r"))) {
    return;
  }

  while (fgets(line, sizeof line, pipe)) {
    if ('\n' == line[strlen(line) - 1]) {
      line[strlen(line) - 1] = '\0';
    }
    if (NULL == strstr(line, "usbserial")) {
      ports_.push_back(line);
    }
    else {
      ports_.insert(ports_.begin(), line);
    }
  }

  pclose(pipe);
}

std::vector<std::string> Seriality::ports() const
{
  return ports_;
}

std::string Seriality::port() const
{
  return port_;
}

int Seriality::speed() const
{
  return speed_;
}

double Seriality::api_version() const
{
  return api_version_;
}

