#pragma once

#include <QString>
#include <QSet>

/**
 * @brief Parses byte by byte the output of the serial interface.
 * Expects commands with following structure:
 *      "C+" + COMMAND_NAME + '\n'
 *  It resets automatically for the next command.
 * Example:
 *      C+POWEROFF
 *      C+COLOR=255,255,255,255
 */
class CommandReader {
public:
    CommandReader();

    const char *command() const;

    //! Returns true when a command is ready to be read
    bool receivedChar(char c);

private:
    inline void reset();
    inline int commandIndex();

private:
    static const QSet<char> s_ignoreChars;
    static const QSet<char> s_terminateChars;
    static const QString c_prefix;
    static const int c_maxLength = 64;

private:
    char m_receivedCommand[c_maxLength + 1];
    unsigned char m_receivedIndex = 0;

    friend class SerialParserTest;
};
