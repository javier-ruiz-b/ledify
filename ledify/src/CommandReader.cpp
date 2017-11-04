#include "CommandReader.h"

const char CommandReader::s_ignoreChars[] = {'\r','\0','\t',' '};
const char CommandReader::s_terminateChars[] = {'\n',';'};

CommandReader::CommandReader() {
    reset();
}

const char *CommandReader::command() const {
    return m_bufferedCommand;
}

bool CommandReader::writeChar(char c) {
    for (unsigned char i = 0; i < sizeof(s_ignoreChars); i++) {
        if (c == s_ignoreChars[i]) {
            return false;
        }
    }
    if (m_prefixIndex < SIZE_OF_STRING(COMMAND_PREFIX)) {
        if (c == COMMAND_PREFIX[m_prefixIndex]) {
            m_prefixIndex++;
            return false;
        }
    } else if (m_prefixIndex == SIZE_OF_STRING(COMMAND_PREFIX)) {
        if (c == PREFIX_SEPARATOR) {
            m_prefixIndex++;
            return false;
        }

        for (unsigned char i = 0; i < sizeof(s_terminateChars); i++) {
            if (c == s_terminateChars[i]) {
                m_bufferedCommand[0] = '\0';
                reset();
                return true;
            }
        }
    } else {
        for (unsigned char i = 0; i < sizeof(s_terminateChars); i++) {
            if (c == s_terminateChars[i]) {
                m_bufferedCommand[m_commandIndex] = '\0';
                reset();
                return (m_bufferedCommand[m_commandIndex] != '\0');
            }
        }
        if (m_commandIndex <= COMMAND_MAX_LENGTH) {
            m_bufferedCommand[m_commandIndex] = c;
            m_commandIndex++;
            return false;
        }
    }
    reset();
    return false;
}

void CommandReader::reset() {
    m_prefixIndex = 0;
    m_commandIndex = 0;
}
