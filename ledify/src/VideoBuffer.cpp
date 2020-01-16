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
    m_renderingLoop = new std::thread(&VideoBuffer::renderingLoop, this);
    m_drawingLoop = new std::thread(&VideoBuffer::drawingLoop, this);
}

void VideoBuffer::stop() {
    finishing = true;
    for (int i = 0; i < c_buffers; i++)
        m_bufferBusy[i].unlock();

    m_renderingLoop->join();
    m_drawingLoop->join();
}

size_t VideoBuffer::numLeds() const {
    return m_numLeds;
}

void VideoBuffer::renderingLoop() {
    int index = 0;
    m_bufferBusy[index].lock();
    while (!finishing) {
        if (m_renderCallback(m_buffer[0], m_numLeds)) {
            int nextIndex = (index + 1) % c_buffers;
            m_bufferBusy[nextIndex].lock();
            m_bufferBusy[index].unlock();
            index = nextIndex;
        } else {
            constexpr auto waitDuration = std::chrono::duration<double, std::milli> (1000);
            std::this_thread::sleep_for(waitDuration);
        }
    }
}

void VideoBuffer::drawingLoop() {
    int index = 0;
    m_bufferBusy[index].lock();
    while (!finishing) {
        m_drawerCallback(m_buffer[0], m_numLeds);
        m_bufferBusy[index].unlock();
        index = (index + 1) % c_buffers;
        m_bufferBusy[index].lock();
    }
}
//void VideoBuffer::drawingLoop() {
//    while (!finishing) {
//        m_renderCallback(m_buffer[0], m_numLeds);
//        m_drawerCallback(m_buffer[0], m_numLeds);
//        constexpr double refreshRateInHz = 30;
//        constexpr auto waitDuration = std::chrono::duration<double, std::milli> (1000 / refreshRateInHz);
//        std::this_thread::sleep_for(waitDuration);
//    }
//}
