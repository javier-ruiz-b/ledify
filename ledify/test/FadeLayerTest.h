#pragma once
#include <QObject>
#include <QSharedPointer>

class ColorLayer;
class FadeLayer;
class StartLayer;

class FadeLayerTest : public QObject {
    Q_OBJECT

public:
    explicit FadeLayerTest(QObject *parent = nullptr) : QObject(parent) {}

    void createFadeLayer();
    
private slots:
    void init();
    void cleanup();
    void showsSourceLayer();
    void showsDestinationLayer();
    void showsSourceLayerBeforeAnimationStarts();
    void finishesAndReplacesStartLayerChild();
    void calculatesLinearFadeMiddleValue();
    void calculatesAcceleratedFadeMiddleValue();
    void calculatesDeceleratedFadeMiddleValue();
    void checksDecelerateFade();

private:
    QSharedPointer<FadeLayer> m_tested;
    QSharedPointer<ColorLayer> m_blackColor;
    QSharedPointer<ColorLayer> m_whiteColor;
    StartLayer *m_startLayer;
};
