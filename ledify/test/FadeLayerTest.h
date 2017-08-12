#pragma once
#include <QObject>

class ColorLayer;
class FadeLayer;

class FadeLayerTest : public QObject {
    Q_OBJECT

public:
    explicit FadeLayerTest(QObject *parent = nullptr) : QObject(parent) {}

private slots:
    void init();
    void cleanup();
    void showsSourceLayer();
    void showsDestinationLayer();
    void calculatesLinearFadeMiddleValue();
    void calculatesAcceleratedFadeMiddleValue();

private:
    FadeLayer *m_tested;
    ColorLayer *m_blackColor;
    ColorLayer *m_whiteColor;
};
