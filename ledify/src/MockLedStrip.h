#pragma once

#include "ILedStrip.h"

#include "Layer.h"

class MockLedStrip : public ILedStrip {
public:
    MockLedStrip(uint32_t numLeds) : ILedStrip(numLeds) {
        m_buffer = new uint32_t[numLeds];
    }

    virtual ~MockLedStrip() override {
        delete[] m_buffer;
    }

    void initialize () override {}
    void deinitialize () override {}

    void draw (uint32_t *ledBuffer) override {
        m_buffer = ledBuffer;
        m_anyLedOn = false;
        for (uint32_t i = 0; i < m_numLeds; i++) {
            if (m_buffer[i] != 0) {
                m_anyLedOn = true;
                break;
            }
        }
    }
    bool isAnyLedOn() override { return m_anyLedOn; }

private:
    bool m_anyLedOn = false;
    uint32_t *m_buffer;
};
