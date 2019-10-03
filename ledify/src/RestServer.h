#pragma once
#include <QObject>
#include <QString>
#include <functional>

class RestServer : public QObject {
    Q_OBJECT

public:
    RestServer(QObject *parent = nullptr);
    void registerCallback(std::function<QString(QString &)> callback) { m_callback = callback; }


private:
    std::function<QString(QString &)> m_callback = [](QString &){return QString(); };

};
