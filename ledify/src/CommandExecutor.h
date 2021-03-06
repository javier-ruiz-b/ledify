#pragma once

#include <QString>
#include <QHash>
#include <functional>
#include "Daytime.h"

class LayerController;
class FpsCalculator;

class CommandExecutor {
public:
    CommandExecutor(LayerController *layers, FpsCalculator *fpsCalculator);

    QString parseCommand(const QString &received);

public:
    void cOff(const QStringList &args, QString &response);
    void cOn(const QStringList &args, QString &response);
    void cOnIfNight(const QStringList &args, QString &response);

private:
    void cSet(const QStringList &args, QString &response);
    void cColor(const QStringList &args, QString &response);
    void cRandom(const QStringList &, QString &response);
    void cFadeTo(const QStringList &args, QString &response);
    void cFade(const QStringList &args, QString &response);
    void cFps(const QStringList &args, QString &response);
    void cTime(const QStringList &args, QString &response);
    void cReset(const QStringList &args, QString &response);
    void cCopy(const QStringList &args, QString &response);
    void cMove(const QStringList &args, QString &response);
    void cSlideAnimation(const QStringList &args, QString &response);
    void cSpot(const QStringList &args, QString &response);
    void cAdd(const QStringList &args, QString &response);
    void cChristmas(const QStringList &args, QString &response);

private:
    bool parseCommand(const QString &command, const QStringList &args, QString &response);

private:
    LayerController *m_layers;
    FpsCalculator *m_fpsCalculator;
    Daytime m_dayTime;

    QHash<QString, std::function<void(const QStringList &, QString &response)>> m_commandToFunction;
};
