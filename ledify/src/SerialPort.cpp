#ifdef __linux__
#include "SerialPort.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

SerialPort::SerialPort() {
    m_fd = -1;
    m_isRead = false;
}

int SerialPort::begin(const char *portName, int speed, Parity parity) {
    m_fd = open (portName, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
    if (m_fd < 0) {
        logerr("error %d opening %s: %s", errno, portName, strerror (errno));
        return -1;
    }
    if (!setInterfaceAttributes(m_fd, speed, static_cast<int>(parity))) {
        return -1;
    }
    setBlocking(m_fd, false);

    return 0;
}

void SerialPort::end() {
    close(m_fd);
}

bool SerialPort::available() {
    if(m_isRead) {
        return true;
    }
    ssize_t rdlen = ::read(m_fd, &m_readByte, 1);
    m_isRead = (rdlen > 0);
    return (rdlen > 0);
}

char SerialPort::read() {
    while (!m_isRead) {
        available();
    }
    m_isRead = false;
    return m_readByte;
}

void SerialPort::write(byte b) {
    if (::write(m_fd, &b, 1) < 0) {
        logerr("Coudn't write %u", static_cast<uint32>(b));
    }
}

int SerialPort::setInterfaceAttributes(int fd, int speed, int parity) {
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
            logerr ("error %d from tcgetattr", errno);
            return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
            logerr ("error %d from tcsetattr", errno);
            return -1;
    }
    return 0;
}

void SerialPort::setBlocking(int fd, bool shouldBlock) {
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        logerr ("error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN]  = shouldBlock ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        logerr ("error %d setting term attributes", errno);
    }
}
#endif
