#include "ChristmasLayer.h"
#include <AdditionLayer.h>
#include <FadeLayer.h>
#include <SlideAnimationLayer.h>
#include <SpotLayer.h>
#include <StartLayer.h>

ChristmasLayer::ChristmasLayer(quint32 lightsToShow, int lightSize, int brightness, int fadeTime, int onTime)
    : m_lightSize(lightSize), m_brightness(brightness), m_lightFadeTime(fadeTime), m_lightOnTime(onTime) {
    m_lightColors = QVector<Color>{
            Color(m_brightness, 0, 0, 0), //red
            Color(m_brightness, 0, 0, 0), //red
            Color(0, m_brightness, 0, 0), //green
            Color(m_brightness*7/8, m_brightness*5/8, m_brightness/16, 0), // amber
            Color(m_brightness*7/8, m_brightness*5/8, m_brightness/16, 0), // amber
            Color(0, 0, 0, m_brightness), //white
            Color(m_brightness*2/3, 0, m_brightness/2, 0), //pink
            Color(0, m_brightness/2, m_brightness/2, 0),
            Color(0, 0, m_brightness, 0), //blue
            Color(0, 0, m_brightness, 0) //blue
    };
    m_black.reset(new ColorLayer(Color(0,0,0,0)));

    for (unsigned int i = 0; i < lightsToShow; i++) {
        m_layers.append(m_black);
    }
}


QSharedPointer<Layer> ChristmasLayer::createLight(int lightIndex, int numLeds) {
    auto lightsToShow = m_layers.size();
    auto distanceBetweenLights = numLeds / lightsToShow;
    auto position = (m_lightSize / 2) + (numLeds*lightIndex / lightsToShow);
    auto startTime = m_rand.bounded(0, m_lightMaxDelay);
    QSharedPointer<Layer> spot(new SpotLayer(randomLightColor(), position, m_lightSize, Interpolator::InterpolatorDecelerate));
    QSharedPointer<Layer> fadeStart(new FadeLayer(m_black, spot, Interpolator::InterpolatorDecelerate, 0, startTime + m_lightFadeTime));
    QSharedPointer<Layer> fadeEnd(new FadeLayer(fadeStart, m_black, Interpolator::InterpolatorAccelerate, startTime + m_lightFadeTime + m_lightOnTime, m_lightFadeTime));
    return QSharedPointer<Layer>(new StartLayer(fadeEnd));
}

void ChristmasLayer::draw(QVector<quint32> &buffer) {
//    QList<QSharedPointer<Layer>> newLayers;
    bool changed = false;
    auto lightsToShow = m_layers.size();
    for (int i = 0; i < lightsToShow; i++) {
        if (m_layers[i]->animationFinished()) {
            m_layers[i] = createLight(i, buffer.size());
            changed = true;
        }
   }
    //5 lights  per 60 * 1000 ms
//    auto lightsToAdd = m_lightsPerMin * time->millis() / (60*1000);

//    auto flyingStarsToAdd = time->millis() / (m_flyingStarsPerMin*60*1000);
//    for (auto i = m_addedFlyingStars; i < flyingStarsToAdd; i++) {
////add layers
//    }

    if (changed) {
        m_aditionLayer.reset(new AdditionLayer(m_layers));
    }
    m_aditionLayer->draw(buffer);

}

Color &ChristmasLayer::randomLightColor() {
    auto index = m_rand.bounded(m_lightColors.size() - 1);
    return m_lightColors[index];
}
