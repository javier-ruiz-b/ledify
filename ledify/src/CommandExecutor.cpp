#include "CommandExecutor.h"
#include <QLoggingCategory>
#include "LayerController.h"
#include "FpsCalculator.h"
#include "TimeControl.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "RandomLayer.h"
#include <SpotLayer.h>
#include <Color.h>

Q_LOGGING_CATEGORY(EXECUTOR, "ledify.executor", QtWarningMsg)

#define funcWrapper(func) [=](const QStringList &args, QString &response) { this->func(args, response); }
#define expects(expectedArgs, args) if (args.count() != expectedArgs) { qCWarning(EXECUTOR) << "Expected" << expectedArgs << "instead of" << args.count(); return; }

CommandExecutor::CommandExecutor(LayerController *layers, FpsCalculator *fpsCalculator)
    : m_layers(layers), m_fpsCalculator(fpsCalculator) {
    m_commandToFunction.insert("SET", funcWrapper(cSet));
    m_commandToFunction.insert("RANDOM", funcWrapper(cRandom));
    m_commandToFunction.insert("COLOR", funcWrapper(cColor));
    m_commandToFunction.insert("FADETO", funcWrapper(cFadeTo));
    m_commandToFunction.insert("FADE", funcWrapper(cFade));
    m_commandToFunction.insert("FPS", funcWrapper(cFps));
    m_commandToFunction.insert("TIME", funcWrapper(cTime));
    m_commandToFunction.insert("RESET", funcWrapper(cReset));
    m_commandToFunction.insert("OFF", funcWrapper(cOff));
    m_commandToFunction.insert("ON", funcWrapper(cOn));
    m_commandToFunction.insert("ONIFNIGHT", funcWrapper(cOnIfNight));
    m_commandToFunction.insert("COPY", funcWrapper(cCopy));
    m_commandToFunction.insert("MOVE", funcWrapper(cMove));
}

bool CommandExecutor::parseCommand(const QString &command, const QStringList &args, QString &response) {
    if (!m_commandToFunction.contains(command)) {
        qCCritical(EXECUTOR) << "Unknown command:" << command;
        return false;
    }
    m_commandToFunction[command](args, response);
    return true;
}

void CommandExecutor::cSet(const QStringList &args, QString &) {
    expects(1, args);
    m_layers->setAsRoot(args[0].toUShort());
}

void CommandExecutor::cColor(const QStringList &args, QString &) {
    expects(5, args);
    m_layers->addTo(args[0].toUShort(),
                    new ColorLayer(Color(args[1].toUShort(),
                                         args[2].toUShort(),
                                         args[3].toUShort(),
                                         args[4].toUShort())));

}

void CommandExecutor::cRandom(const QStringList &, QString &) {
//    auto currentLayer = m_layers->rootLayer();
    auto randomIndex = m_layers->add(new RandomLayer());
    auto fadeIndex = m_layers->addFadeLayerFromCurrent(randomIndex, Interpolator::InterpolatorDecelerate, 0, 1000);
    m_layers->setAsRoot(fadeIndex);
    auto colorIndex = m_layers->addColorLayer(60, 40, 5, 100);
//    m_layers->add(new FadeLayer());
    auto fadeBackIndex = m_layers->addFadeLayerFromCurrent(colorIndex, Interpolator::InterpolatorDecelerate, 8000, 1000);
    m_layers->setAsRoot(fadeBackIndex);
}

void CommandExecutor::cFadeTo(const QStringList &args, QString &) {
    expects(5, args);
    m_layers->addFadeLayerFromCurrent(args[0].toUShort(),
            args[1].toUShort(),
            static_cast<Interpolator::Type>(args[2].toUShort()),
            args[3].toUShort(),
            args[4].toUShort());
}

void CommandExecutor::cFade(const QStringList &args, QString &) {
    expects(6, args);
    m_layers->addFadeLayer(args[0].toUShort(),
            args[1].toUShort(),
            args[2].toUShort(),
            static_cast<Interpolator::Type>(args[3].toUShort()),
            args[4].toUShort(),
            args[5].toUShort());
}

void CommandExecutor::cFps(const QStringList &args, QString &) {
    expects(1, args);
    m_fpsCalculator->setEnabled(args[0].toUShort() != 0);
}

void CommandExecutor::cTime(const QStringList &, QString &response) {
    response = QString::number(TimeControl::instance()->millis()) + "ms";
}

void CommandExecutor::cReset(const QStringList &, QString &) {
    m_layers->reset();
}

void CommandExecutor::cCopy(const QStringList &args, QString &) {
    m_layers->copy(args[0].toUShort(), args[1].toUShort());
}

void CommandExecutor::cMove(const QStringList &args, QString &) {
    m_layers->move(args[0].toUShort(), args[1].toUShort());
}

void CommandExecutor::cOff(const QStringList &, QString &) {
    auto colorIndex = m_layers->add(new ColorLayer(Color(0, 0, 0 ,0)));
    auto fadeIndex = m_layers->addFadeLayerFromCurrent(colorIndex, Interpolator::InterpolatorDecelerate, 0, 2000);
    m_layers->setAsRoot(fadeIndex);
}

void CommandExecutor::cOn(const QStringList &, QString &) {
    auto spotIndex = m_layers->add(new SpotLayer(Color(203, 80, 1, 203), 150, 100, Interpolator::InterpolatorLinear));
    auto fadeIndex = m_layers->addFadeLayerFromCurrent(spotIndex, Interpolator::InterpolatorAccelerate, 0, 2000);
    m_layers->setAsRoot(fadeIndex);
}

void CommandExecutor::cOnIfNight(const QStringList &args, QString &response) {
    if (!m_dayTime.isDay()) {
        cOn(args, response);
    } else {
        qCDebug(EXECUTOR) << "Not turning on, it's currently day.";
    }
}

