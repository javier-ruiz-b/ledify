#ifdef __linux__
#pragma once
#include "definitions.h"

#include <termios.h>

#define PARITY_NONE     0
#define PARITY_ODD
#define PARITY_EVEN
#define PARITY_MARK
#define PARITY_SPACE

class SerialPort {
public:
    enum Parity {
        ParityNone = 0,
        ParityOdd = PARENB|PARODD,
        ParityEven = PARENB,
        ParityMark = PARENB|PARODD|CMSPAR,
        ParitySpace = PARENB|CMSPAR,
    };

public:
    SerialPort();
    int begin(const char *portName, int speed, Parity parity = ParityNone);
    void end();
    bool available();
    char read();
    void write(byte b);

private:
    int setInterfaceAttributes(int fd, int speed, int parity);
    void setBlocking(int fd, bool shouldBlock);

private:
    int m_fd;
    char m_readByte;
    bool m_isRead;

};

#endif
