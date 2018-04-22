#include "CommandReaderTest.h"
#include "CommandReader.h"

#include <iostream>

void CommandReaderTest::init() {
    m_tested = new CommandReader();
}

void CommandReaderTest::cleanup() {
    delete m_tested;
}

void CommandReaderTest::parsesCommand() {
    bool result = writeCommand("C+POWEROFF\n");

    QVERIFY(result);
    QCOMPARE(m_tested->command(), "POWEROFF");
}

void CommandReaderTest::parsesCommandCrLf() {
    bool result = writeCommand("C+POWEROFF\r\n");

    QVERIFY(result);
    QCOMPARE(m_tested->command(), "POWEROFF");
}

void CommandReaderTest::parsesCommandIgnoringInvalidChars() {
    bool result = writeCommand("  \t C+POWEROFF\t \r\n\0");

    QVERIFY(result);
    QCOMPARE(m_tested->command(), "POWEROFF");
}

void CommandReaderTest::emptyCommandWhenWrittingJustC() {
    bool result = writeCommand("C\n");

    QVERIFY(!result);
    QCOMPARE(m_tested->command(), "");
}

void CommandReaderTest::failsWhenCommandTooLong() {
    bool result = writeCommand("C+0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");

    QVERIFY(!result);
}

void CommandReaderTest::failsWhenWrittingCommandsWithoutPrefixes() {
    QVERIFY(!writeCommand("POWEROFF\n"));
    QVERIFY(!writeCommand("OFF\n"));
    QVERIFY(!writeCommand("COLOR\n"));
}

void CommandReaderTest::failsWhenWrittingCommandsWithoutEndingLine() {
    QVERIFY(!writeCommand("C+POWEROFF"));
    QVERIFY(!writeCommand("C+COLOR"));
    QVERIFY(!writeCommand("C+OFF"));
}

void CommandReaderTest::failsWhenWrittingSeparatorAndNoCommand() {
    QVERIFY(!writeCommand("C+\n"));
}

bool CommandReaderTest::writeCommand(std::string command) {
    bool lastResult = false;
    for (unsigned int i = 0; i < command.length(); i++) {
        lastResult = m_tested->receivedChar(command[i]);
    }
    return lastResult;
}

QTEST_APPLESS_MAIN(CommandReaderTest)
