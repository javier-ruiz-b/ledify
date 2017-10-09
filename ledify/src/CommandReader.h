#pragma once

#include "definitions.h"

#define COMMAND_PREFIX      "C"
#define PREFIX_SEPARATOR    '+'
#define COMMAND_MAX_LENGTH  48
#define COMMAND_TERMINATION '\n'

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
    bool writeChar(char c);

private:
    void reset();

private:
    char m_bufferedCommand[COMMAND_MAX_LENGTH + 1];
    unsigned char m_prefixIndex;
    unsigned char m_commandIndex;

    friend class SerialParserTest;
};
