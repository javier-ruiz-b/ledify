#include "CommandExecutor.h"
#include <QLoggingCategory>
#include <QSharedPointer>
#include "LayerController.h"
#include "FpsCalculator.h"
#include "TimeControl.h"
#include "ColorLayer.h"
#include "AdditionLayer.h"
#include "FadeLayer.h"
#include "RandomLayer.h"
#include "SlideAnimationLayer.h"
#include "AdditionLayer.h"
#include "ChristmasLayer.h"
#include <SpotLayer.h>
#include <Color.h>

Q_LOGGING_CATEGORY(EXECUTOR, "ledify.executor", QtDebugMsg)

#define funcWrapper(func) [=](const QStringList &args, QString &response) { this->func(args, response); }
#define expects(expectedArgs, args) if (args.count() != expectedArgs) { qCWarning(EXECUTOR) << "Expected" << expectedArgs << "instead of" << args.count(); return; }
#define expectsAtLeast(expectedAtLeastArgs, args) if (args.count() < expectedAtLeastArgs) { qCWarning(EXECUTOR) << "Expected at least" << expectedAtLeastArgs << "instead of" << args.count(); return; }

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
    m_commandToFunction.insert("SPOT", funcWrapper(cSpot));
    m_commandToFunction.insert("ADD", funcWrapper(cAdd));
    m_commandToFunction.insert("CHRISTMAS", funcWrapper(cChristmas));
}

bool CommandExecutor::parseCommand(const QString &command, const QStringList &args, QString &response) {
    if (!m_commandToFunction.contains(command)) {
        qCCritical(EXECUTOR) << "Unknown command:" << command;
        return false;
    }
    m_commandToFunction[command](args, response);
    return true;
}

QString CommandExecutor::parseCommand(const QString &received) {
    qCDebug(EXECUTOR) << "Recv:" << received;

    QString response;
    auto lines = received.split(QRegularExpression("(;|\n| )"));
    for (auto line: lines) {
        auto commandAndArgs = line.split("=");
        if (commandAndArgs.empty()) {
            qCWarning(EXECUTOR) << "Received empty command";
            return "Empty command";
        }
        auto command = commandAndArgs.first();
        QStringList args;
        if (commandAndArgs.count() >= 2) {
            args = commandAndArgs[1].split(",");
        }

        parseCommand(command, args, response);
    }
    return response;
}

void CommandExecutor::cSet(const QStringList &args, QString &) {
    expects(1, args);
    m_layers->setAsRoot(args[0].toUShort());
}

void CommandExecutor::cColor(const QStringList &args, QString &response) {
    expects(5, args);
    m_layers->addTo(args[0].toUShort(), ColorLayer::createFromCommand(args, response));
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
    expects(3, args);
    m_layers->addTo(args[0].toUShort(),
                    new SlideAnimationLayer(m_layers->take(args[1].toUShort()),
                                            args[2].toFloat()));
}

void CommandExecutor::cSpot(const QStringList &args, QString &) {
    expects(8, args);
    m_layers->addTo(args[0].toUShort(),
            new SpotLayer(  Color(args[1].toUShort(),
                                  args[2].toUShort(),
                                  args[3].toUShort(),
                                  args[4].toUShort()),
                            args[5].toFloat(),
                            args[6].toFloat(),
            static_cast<Interpolator::Type>(args[7].toUShort())));
}

void CommandExecutor::cAdd(const QStringList &args, QString &) {
    expectsAtLeast(2, args);
    QVector<QSharedPointer<Layer>> layers;
    for (int i = 1; i < args.length(); i++) {
        auto layer = m_layers->take(args[i].toUShort());
        if (layer.isNull()) {
            qCDebug(EXECUTOR) << "Layer doesn't exist:" << args[i].toUShort();
            continue;
        }
        qCDebug(EXECUTOR) << "Taking layer" << args[i].toUShort();
        layers << layer;
    }
    m_layers->addTo(args[0].toUShort(),
            new AdditionLayer(layers));
}

void CommandExecutor::cChristmas(const QStringList &args, QString &) {
    expects(6, args);
    m_layers->addTo(args[0].toUShort(),
            new ChristmasLayer( args[1].toUInt(),
                                args[2].toInt(),
                                args[3].toInt(),
                                args[4].toInt(),
                                args[5].toInt()));

}

void CommandExecutor::cOff(const QStringList &, QString &) {
    auto fade = new FadeLayer(m_layers->current(),
                              QSharedPointer<Layer>(new ColorLayer(Color(0, 0, 0 ,0))),
                              Interpolator::InterpolatorDecelerate, 0, 1500);
    m_layers->setAsRoot(fade);
}

void CommandExecutor::cOn(const QStringList &, QString &) {
    QSharedPointer<Layer> red(new SpotLayer(Color(150, 0, 0, 0), 150, 75, Interpolator::InterpolatorLinear));
    QSharedPointer<Layer> green(new SpotLayer(Color(0, 150, 0, 0), 150, 75, Interpolator::InterpolatorLinear));
    QSharedPointer<Layer> blue(new SpotLayer(Color(0, 0, 150, 0), 150, 75, Interpolator::InterpolatorLinear));
    QSharedPointer<Layer> white(new SpotLayer(Color(0, 0, 0, 255), 150, 4, Interpolator::InterpolatorDecelerate));
    QSharedPointer<Layer> spot(new SpotLayer(Color(203, 80, 2, 203), 150, 150, Interpolator::InterpolatorLinear));

    auto redAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(red, 0.12f));
    auto redAnimation2 = QSharedPointer<Layer> (new SlideAnimationLayer(red, -0.12f));
    auto greenAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(green, -0.25f));
    auto blueAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(blue, 0.25f));
    auto whiteAnimation = QSharedPointer<Layer> (new SlideAnimationLayer(white, -0.1f));
    auto whiteAnimation2 = QSharedPointer<Layer> (new SlideAnimationLayer(white, 0.1f));

    auto allLayers = QSharedPointer<Layer> (new AdditionLayer({redAnimation, redAnimation2, greenAnimation, blueAnimation, whiteAnimation, whiteAnimation2}));
//    auto allLayers = QSharedPointer<Layer> (new AdditionLayer({red, green, blue, white}));

    auto fadeStart = new FadeLayer(m_layers->current(),
                              allLayers,
                              Interpolator::InterpolatorAccelerate, 0, 100);
    auto fadeEnd = new FadeLayer(QSharedPointer<Layer> (fadeStart),
                              spot,
                              Interpolator::InterpolatorAccelerate, 3000, 4000);
    m_layers->setAsRoot(fadeEnd);
}

void CommandExecutor::cOnIfNight(const QStringList &args, QString &response) {
    if (!m_dayTime.isDay() && !m_dayTime.isAfterMidnight()) {
        cOn(args, response);
    } else {
        qCDebug(EXECUTOR) << "Not turning on, it's currently day or after midnight.";
    }
}

