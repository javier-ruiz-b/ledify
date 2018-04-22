#include "SerialPort.h"
#include <cerrno>
#include <cstring>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <QtDebug>


SerialPort::SerialPort() {
    m_fd = -1;
    m_isRead = false;
}

int SerialPort::begin(const char *portName, int speed) {
    m_fd = serialOpen(portName, speed);
    if (m_fd < 0) {
        qCritical() << "error " << errno
                    << "opening " << portName
                    << ":" << strerror (errno);
        return -1;
    }

    return 0;
}

void SerialPort::end() {
    serialClose(m_fd);
}

bool SerialPort::available() {
    return serialDataAvail(m_fd);
}

char SerialPort::read() {
    return static_cast<char>(serialGetchar(m_fd));
}

void SerialPort::write(unsigned char b) {
    serialPutchar(m_fd, b);
}

void SerialPort::writeString(const char *s) {
    serialPuts(m_fd, s);
}
