#pragma once
#include <QObject>

class Config : public QObject {
    Q_OBJECT
    Q_PROPERTY(quint32 ledCount READ ledCount WRITE setLedCount NOTIFY ledCountChanged)

public:
    static Config* instance();
    static Config* createInstance(QObject *parent);

public:
    quint32 ledCount() const;

public slots:
    void setLedCount(quint32 ledCount);

signals:
    void ledCountChanged(quint32 ledCount);

private:
    explicit Config(QObject *parent = nullptr);
    Config(Config const&){};             // copy constructor is private
    Config& operator=(Config const&){ return *this; };  // assignment operator is private
    static Config* m_instance;

    quint32 m_ledCount = 300;
};
