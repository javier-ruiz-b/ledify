#include "Config.h"

Config::Config(QObject *parent) : QObject(parent) {}

Config *Config::m_instance;
Config *Config::createInstance(QObject *parent) {
    m_instance = new Config(parent);
    return instance();
}

Config *Config::instance() {
    return m_instance;
}

quint32 Config::ledCount() const {
    return m_ledCount;
}

void Config::setLedCount(quint32 ledCount){
    if (m_ledCount == ledCount)
        return;

    m_ledCount = ledCount;
    emit ledCountChanged(m_ledCount);
}
