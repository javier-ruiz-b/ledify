#pragma once
#include "definitions.h"

class SerialPort {

public:
    SerialPort();
    int begin(const char *portName, int speed);
    void end();
    bool available();
    char read();
    void write(byte b);
    void writeString(const char *s);

private:
    int m_fd;
    char m_readByte;
    bool m_isRead;

};

