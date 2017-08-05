#include "SerialParserTest.h"
#include "SerialParser.h"

#include <iostream>

void SerialParserTest::init() {
    m_tested = new SerialParser();
}

void SerialParserTest::cleanup() {
    delete m_tested;
}

void SerialParserTest::parsesCommand() {
    bool result = writeCommand("C+POWEROFF\n");

    QVERIFY(result);
    QCOMPARE(m_tested->command(), "POWEROFF");
}

void SerialParserTest::emptyCommandWhenWrittingJustC() {
    bool result = writeCommand("C\n");

    QVERIFY(result);
    QCOMPARE(m_tested->command(), "");
}

void SerialParserTest::failsWhenCommandTooLong() {
    // max. 32 + \n = 33. Counting after AT+.
    bool result = writeCommand("C+0123456789012345678901234567890123\n");

    QVERIFY(!result);
}

void SerialParserTest::failsWhenWrittingCommandsWithoutPrefixes() {
    QVERIFY(!writeCommand("POWEROFF\n"));
    QVERIFY(!writeCommand("OFF\n"));
    QVERIFY(!writeCommand("COLOR\n"));
}

void SerialParserTest::failsWhenWrittingCommandsWithoutEndingLine() {
    QVERIFY(!writeCommand("C+POWEROFF"));
    QVERIFY(!writeCommand("C+COLOR"));
    QVERIFY(!writeCommand("C+OFF"));
}

void SerialParserTest::failsWhenWrittingSeparatorAndNoCommand() {
    QVERIFY(!writeCommand("C+\n"));
}

bool SerialParserTest::writeCommand(std::string command) {
    bool lastResult = false;
    for (int i = 0; i < command.length(); i++) {
        lastResult = m_tested->write(command[i]);
    }
    return lastResult;
}

QTEST_APPLESS_MAIN(SerialParserTest)
