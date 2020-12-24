#pragma once
#include "Layer.h"
#include <Color.h>
#include <ColorLayer.h>
#include <QRandomGenerator>

class ChristmasLayer : public Layer {
public:
    ChristmasLayer(quint32 lightsToShow, int lightSize, int brightness, int fadeTime, int onTime);

    virtual void draw(QVector<quint32> &buffer) override;
    virtual bool animationFinished() override { return false; }

private:
    Color &randomLightColor();

private:
    QVector<QSharedPointer<Layer>> m_layers;

    QRandomGenerator m_rand;
    QVector<Color> m_lightColors;
    int m_lightSize = 8;
    int m_brightness = 150;
    int m_lightMaxDelay = 3000;
    int m_lightFadeTime = 1000;
    int m_lightOnTime = 1000;

    QSharedPointer<Layer> m_black;
    QSharedPointer<Layer> m_aditionLayer;

    QSharedPointer<Layer> createLight(int lightIndex, int numLeds);
};
