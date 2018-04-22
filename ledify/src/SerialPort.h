#pragma once

class SerialPort {

public:
    SerialPort();
    int begin(const char *portName, int speed);
    void end();
    bool available();
    char read();
    void write(unsigned char b);
    void writeString(const char *s);

private:
    int m_fd;
    bool m_isRead;

};

