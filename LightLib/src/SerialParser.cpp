#include "SerialParser.h"

SerialParser::SerialParser() {
    reset();
}

const char *SerialParser::command() const {
    return m_bufferedCommand;
}

bool SerialParser::write(char c) {
    if (m_prefixIndex < SIZE_OF_STRING(COMMAND_PREFIX)) {
        if (c == COMMAND_PREFIX[m_prefixIndex]) {
            m_prefixIndex++;
            return false;
        }
    } else if (m_prefixIndex == SIZE_OF_STRING(COMMAND_PREFIX)) {
        if (c == PREFIX_SEPARATOR) {
            m_prefixIndex++;
            return false;
        } else if (c == COMMAND_TERMINATION) {
            m_bufferedCommand[0] = '\0';
            reset();
            return true;
        }
    } else {
        if (c == COMMAND_TERMINATION) {
            m_bufferedCommand[m_commandIndex] = '\0';
            reset();
            return true;
        } else if (m_commandIndex <= COMMAND_MAX_LENGTH) {
            m_bufferedCommand[m_commandIndex] = c;
            m_commandIndex++;
            return false;
        }
    }
    reset();
    return false;
}

void SerialParser::reset() {
    m_prefixIndex = 0;
    m_commandIndex = 0;
}
