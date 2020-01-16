#pragma once
#include <QObject>
#include <functional>
#include <thread>
#include <mutex>

class VideoBuffer : public QObject {
    Q_OBJECT

public:
    explicit VideoBuffer(size_t numLeds, QObject *parent = nullptr);
    virtual ~VideoBuffer();

    void registerRenderCallback(std::function<bool(uint32_t *, size_t)> callback) {
        m_renderCallback = callback;
    }

    void registerDrawerCallback(std::function<bool(uint32_t *, size_t)> callback) {
        m_drawerCallback = callback;
    }

    void start();
    void stop();

    size_t numLeds() const;

private:
    void renderingLoop();
    void drawingLoop();

private:
    constexpr static int c_buffers = 3;

    bool finishing = false;
    uint32_t *m_buffer[c_buffers];
    std::mutex m_bufferBusy[c_buffers];
    size_t m_numLeds;

    std::function<bool(uint32_t *, size_t)> m_renderCallback;
    std::function<bool(uint32_t *, size_t)> m_drawerCallback;

    std::thread *m_renderingLoop;
    std::thread *m_drawingLoop;
};
