#pragma once

#include "ILedStrip.h"

#include "Layer.h"
#include "Config.h"

class MockLedStrip : public ILedStrip {
public:
    MockLedStrip() :  ILedStrip() {
        auto numLeds = Config::instance()->ledCount();
        m_buffer.resize(numLeds);
    }

    void initialize () override {}
    void deinitialize () override {}
    void render (Layer *layer) override {
        auto numLeds = Config::instance()->ledCount();
        layer->draw(m_buffer);
        m_anyLedOn = false;
        for (uint32_t i = 0; i < numLeds; i++) {
            if (m_buffer[i] != 0) {
                m_anyLedOn = true;
                break;
            }
        }
    }
    bool isAnyLedOn() override { return m_anyLedOn; }

private:
    bool m_anyLedOn = false;
    QVector<quint32> m_buffer;
};
