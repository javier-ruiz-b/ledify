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

void CommandReaderTest::emptyCommandWhenWrittingJustC() {
    bool result = writeCommand("C\n");

    QVERIFY(result);
    QCOMPARE(m_tested->command(), "");
}

void CommandReaderTest::failsWhenCommandTooLong() {
    // max. 32 + \n = 33. Counting after AT+.
    bool result = writeCommand("C+0123456789012345678901234567890123\n");

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
    for (int i = 0; i < command.length(); i++) {
        lastResult = m_tested->write(command[i]);
    }
    return lastResult;
}

QTEST_APPLESS_MAIN(CommandReaderTest)
