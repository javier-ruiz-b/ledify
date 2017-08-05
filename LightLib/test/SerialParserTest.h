#pragma once
#include <QtTest>

class SerialParser;

class SerialParserTest : public QObject {
    Q_OBJECT

public:
    SerialParserTest() {}

private slots:
    void init();
    void cleanup();

private slots: //tests
    void parsesCommand();
    void emptyCommandWhenWrittingJustC();
    void failsWhenCommandTooLong();
    void failsWhenWrittingCommandsWithoutPrefixes();
    void failsWhenWrittingCommandsWithoutEndingLine();
    void failsWhenWrittingSeparatorAndNoCommand();

private:
    bool writeCommand(std::string command);

private:
    SerialParser *m_tested;
};
