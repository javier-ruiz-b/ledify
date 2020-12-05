#include "SmokeTest.h"
#include <LedStripController.h>
#include <MockLedStrip.h>
#include "AlwaysOnRelayController.h"
#include <TimeControl.h>
#include <QScopedPointer>

SmokeTest::SmokeTest(QObject *parent) : QObject(parent) {
    TimeControl::instance()->setMocked(true);
}

void SmokeTest::init() {

}

void SmokeTest::cleanup() {

}

void SmokeTest::readTestFiles() {
    QDirIterator it(":/SmokeTestFiles");
    while (it.hasNext()) {
        QFile file(it.next());
        file.open(QIODevice::ReadOnly);
        auto contents = QTextStream(&file).readAll();
        QVERIFY2(runTestFile(contents), file.fileName().toUtf8());
    }
}

bool SmokeTest::runTestFile(QString &contents) {
    const int cLedCount = 151;
    Config::createInstance(this)->setLedCount(cLedCount);

    TimeControl::instance()->setMillis(0);

    QScopedPointer<MockLedStrip> ledStrip(new MockLedStrip());
    AlwaysOnRelayController relayController;
    LedStripController tested(ledStrip.data(), &relayController);

    relayController.turnOn();
    QTest::qWait(0);

    for (auto command: contents.split("\n", QString::SkipEmptyParts)) {
        tested.parseReceivedString(command);
        tested.draw();
    }

    bool drawn = false;
    QVector<quint32> leds(cLedCount);
    memset(leds.data(), 0xFE, cLedCount * sizeof(uint32_t));
    connect(&tested, &LedStripController::drawPixels, this,
            [&drawn, &leds] (Layer *rootLayer) {
        rootLayer->draw(leds);
        drawn = true;
    });


    int lastDraw = 0;
    for (unsigned int i = 0; i < 1000*1000; i++) {
        TimeControl::instance()->setMillis(i);
        tested.draw();

        if (drawn) {
            lastDraw = static_cast<int>(i);
            drawn = false;
        }

        if (tested.animationFinished()) {
            if (lastDraw == 0) {
                qWarning() << "Didn't draw anything";
                return false;
            } else {
                break;
            }
        }
    }
    return true;
}



QTEST_APPLESS_MAIN(SmokeTest)
