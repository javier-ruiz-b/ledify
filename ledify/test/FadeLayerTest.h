#pragma once
#include <QObject>

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
    void finishesAndReplacesStartLayerChild();
    void calculatesLinearFadeMiddleValue();
    void calculatesAcceleratedFadeMiddleValue();
    void calculatesDeceleratedFadeMiddleValue();

private:
    FadeLayer *m_tested;
    ColorLayer *m_blackColor;
    ColorLayer *m_whiteColor;
    StartLayer *m_startLayer;
};
