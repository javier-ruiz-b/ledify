#pragma once
#include <QtTest>

class CommandReader;

class CommandReaderTest : public QObject {
    Q_OBJECT

public:
    CommandReaderTest() {}

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
    CommandReader *m_tested;
};
