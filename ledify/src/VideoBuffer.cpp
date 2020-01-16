#include "VideoBuffer.h"

VideoBuffer::VideoBuffer(size_t numLeds, QObject *parent)
    : QObject(parent), m_numLeds(numLeds) {
    for (int i = 0; i < c_buffers; i++) {
        m_buffer[i] = new uint32_t[numLeds];
    }
}

VideoBuffer::~VideoBuffer() {
    for (int i = 0; i < c_buffers; i++) {
        delete m_buffer[i];
    }
}

void VideoBuffer::start() {
    while (!finishing) {
        m_renderCallback(m_buffer[0], m_numLeds);
        m_drawerCallback(m_buffer[0], m_numLeds);
    }
}

void VideoBuffer::stop() {
    finishing = true;
}

size_t VideoBuffer::numLeds() const
{
    return m_numLeds;
}
