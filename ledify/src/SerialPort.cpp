#include "SerialPort.h"
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>


SerialPort::SerialPort() {
    m_fd = -1;
    m_isRead = false;
}

int SerialPort::begin(const char *portName, int speed) {
    m_fd = serialOpen(portName, speed);
    if (m_fd < 0) {
        logerr("error %d opening %s: %s", errno, portName, strerror (errno));
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

void SerialPort::write(byte b) {
    serialPutchar(m_fd, b);
}

void SerialPort::writeString(const char *s) {
    serialPuts(m_fd, s);
}
