#pragma once

#include <QString>
#include <QHash>
#include <functional>

class LayerController;
class FpsCalculator;

class CommandExecutor {
public:
    CommandExecutor(LayerController *layers, FpsCalculator *fpsCalculator);

    bool parseCommand(const QString &command, const QStringList &args, QString &response);

public:
    void cOff(const QStringList &args, QString &);
    void cOn(const QStringList &args, QString &);

private:
    void cSet(const QStringList &args, QString &response);
    void cColor(const QStringList &args, QString &response);
    void cRandom(const QStringList &, QString &response);
    void cFadeTo(const QStringList &args, QString &response);
    void cFade(const QStringList &args, QString &response);
    void cFps(const QStringList &args, QString &response);
    void cTime(const QStringList &args, QString &response);
    void cReset(const QStringList &args, QString &);

private:
    const int c_relayGpioPin = 29;
    LayerController *m_layers;
    FpsCalculator *m_fpsCalculator;

    QHash<QString, std::function<void(const QStringList &, QString &response)>> m_commandToFunction;
};