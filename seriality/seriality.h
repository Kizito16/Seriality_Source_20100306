// Seriality: Serial Connectivity Plug-in
// Copyright 2010 Nicholas Zambetti
// Creative Commons Attribution-Share Alike 3.0
//
// Based on original sources from the Google Nixysa Project
// Copyright 2009 Google Inc.
// Apache License, Version 2.0
//

#ifndef SERIALITY_H
#define SERIALITY_H

#include <string>
#include <vector>

class Seriality {
  public:
    Seriality();
    bool begin(std::string port, int speed);
    bool begin(std::string port);
    void end();
    int readByte();
    std::string read();
    std::string readLine();
    std::string readJSON();
    std::string readUntil(int until);
    bool writeByte(int out);
    bool write(std::string out);
    bool write(int out);
    bool write(float out);
    bool write(double out);
    int available();
    void refreshPorts();
    std::vector<std::string> ports() const;
    std::string port() const;
    int speed() const;
    double api_version() const;
  private:
    std::vector<std::string> ports_;
    std::string port_;
    int speed_;
    double api_version_;
};

#endif