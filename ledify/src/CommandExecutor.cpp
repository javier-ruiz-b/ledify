#include "CommandExecutor.h"
#include <QLoggingCategory>
#include <QSharedPointer>
#include "LayerController.h"
#include "FpsCalculator.h"
#include "TimeControl.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "RandomLayer.h"
#include "SlideAnimationLayer.h"
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
    m_commandToFunction.insert("SLIDE", funcWrapper(cSlideAnimation));
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
    auto random = QSharedPointer<Layer>(new RandomLayer());
    QSharedPointer<Layer> firstFade(new FadeLayer(m_layers->current(), random,
                                   Interpolator::InterpolatorDecelerate, 0, 1000));

    auto fadeBack = new FadeLayer(firstFade, m_layers->current(),
                                  Interpolator::InterpolatorDecelerate, 8000, 1000);
    m_layers->setAsRoot(fadeBack);
}

void CommandExecutor::cFadeTo(const QStringList &args, QString &) {
    expects(5, args);
    m_layers->addTo(args[0].toUShort(),
          new FadeLayer(m_layers->current(),
                        m_layers->take(args[1].toUShort()),
                        static_cast<Interpolator::Type>(args[2].toUShort()),
                        args[3].toUShort(),
                        args[4].toUShort()));
}

void CommandExecutor::cFade(const QStringList &args, QString &) {
    expects(6, args);
    m_layers->addTo(args[0].toUShort(),
                    new FadeLayer(m_layers->take(args[1].toUShort()),
                                  m_layers->take(args[2].toUShort()),
                                  static_cast<Interpolator::Type>(args[3].toUShort()),
                                  args[4].toUShort(),
                                  args[5].toUShort()));
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
    expects(2, args);
    m_layers->copy(args[0].toUShort(), args[1].toUShort());
}

void CommandExecutor::cMove(const QStringList &args, QString &) {
    expects(2, args);
    m_layers->move(args[0].toUShort(), args[1].toUShort());
}

void CommandExecutor::cSlideAnimation(const QStringList &args, QString &) {
    expects(2, args);
    m_layers->addTo(args[0].toUShort(),
                    new SlideAnimationLayer(m_layers->current(),
                                 args[1].toFloat()));
}

void CommandExecutor::cOff(const QStringList &, QString &) {
    auto fade = new FadeLayer(m_layers->current(),
                              QSharedPointer<Layer>(new ColorLayer(Color(0, 0, 0 ,0))),
                              Interpolator::InterpolatorDecelerate, 0, 1500);
    m_layers->setAsRoot(fade);
}

void CommandExecutor::cOn(const QStringList &, QString &) {
    QSharedPointer<Layer> red(new SpotLayer(Color(203, 1, 1, 1), 50, 50, Interpolator::InterpolatorLinear));
    QSharedPointer<Layer> green(new SpotLayer(Color(1, 203, 1, 1), 200, 50, Interpolator::InterpolatorLinear));
    QSharedPointer<Layer> blue(new SpotLayer(Color(1, 1, 203, 1), 150, 150, Interpolator::InterpolatorLinear));
    QSharedPointer<Layer> spot(new SpotLayer(Color(203, 80, 1, 203), 150, 150, Interpolator::InterpolatorLinear));

    auto redAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(red, 0.25));
    auto greenAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(green, -0.25));
    auto blueAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(blue, 0.25));

    auto animation = QSharedPointer<Layer> (new FadeLayer(m_layers->current(),
                                  redAnimation,
                                  Interpolator::InterpolatorAccelerate, 0, 1000));
    animation = QSharedPointer<Layer> (new FadeLayer(animation,
                              greenAnimation,
                              Interpolator::InterpolatorAccelerate, 1000, 2000));
    animation = QSharedPointer<Layer> (new FadeLayer(animation,
                              blueAnimation,
                              Interpolator::InterpolatorAccelerate, 1500, 2500));

    auto fadeEnd = new FadeLayer(animation,
                              spot,
                              Interpolator::InterpolatorAccelerate, 2500, 4000);
    m_layers->setAsRoot(fadeEnd);
}

//void CommandExecutor::cOn(const QStringList &, QString &) {
//    QSharedPointer<Layer> spotLayer(new SpotLayer(Color(203, 80, 1, 203), 150, 150, Interpolator::InterpolatorLinear));
//    auto fade = new FadeLayer(m_layers->current(),
//                              spotLayer,
//                              Interpolator::InterpolatorAccelerate, 0, 2000);
//    m_layers->setAsRoot(fade);
//}

void CommandExecutor::cOnIfNight(const QStringList &args, QString &response) {
    if (!m_dayTime.isDay()) {
        cOn(args, response);
    } else {
        qCDebug(EXECUTOR) << "Not turning on, it's currently day.";
    }
}

