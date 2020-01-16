#pragma once
#include <QObject>
#include <functional>

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

signals:

public slots:

private:
    bool finishing = false;
    constexpr static int c_buffers = 3;
    uint32_t *m_buffer[c_buffers];
    size_t m_numLeds;

    std::function<bool(uint32_t *, size_t)> m_renderCallback;
    std::function<bool(uint32_t *, size_t)> m_drawerCallback;

};
