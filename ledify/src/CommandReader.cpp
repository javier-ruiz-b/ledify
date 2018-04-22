#include "CommandReader.h"
#include <QtCore>

const QSet<char> CommandReader::s_ignoreChars = {'\r','\0','\t',' '};
const QSet<char> CommandReader::s_terminateChars = {'\n',';'};
const QString CommandReader::c_prefix = "C+";

CommandReader::CommandReader() {
    reset();
}

const char *CommandReader::command() const {
    return m_receivedCommand;
}

bool CommandReader::receivedChar(char c) {
    if (s_ignoreChars.contains(c)) {
        return false;
    }
    if (s_terminateChars.contains(c)) {
        int index = m_receivedIndex;
        m_receivedCommand[commandIndex()] = '\0';
        reset();
        return index > c_prefix.size();
    } else if (m_receivedIndex < c_prefix.size()) {
        if (c == c_prefix[m_receivedIndex]) {
            m_receivedIndex++;
            return false;
        }
    } else if (commandIndex() >= c_maxLength) {
        reset();
        return false;
    } else {
        m_receivedCommand[commandIndex()] = c;
        m_receivedIndex++;
        return false;
    }
    reset();
    return false;
}

void CommandReader::reset() {
    m_receivedIndex = 0;
}

int CommandReader::commandIndex() {
    return qMax(m_receivedIndex - c_prefix.size(), 0);
}
