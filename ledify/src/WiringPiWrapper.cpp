#include "WiringPiWrapper.h"
#include <wiringPi.h>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(WIRING_PI, "ledify.wiringPiWrapper", QtWarningMsg)

WiringPiWrapper::WiringPiWrapper() {}

void WiringPiWrapper::digitalWrite(int pin, int value) {
    switch(value) {
    case low:       value = LOW;        break;
    case high:      value = HIGH;       break;
    default:
        qCCritical(WIRING_PI) << "Unimplemented case in digitalWrite";
    }
    ::digitalWrite(pin, value);
}

void WiringPiWrapper::pinMode(int pin, int mode) {
    switch(value) {
    case input:     value = INPUT;      break;
    case output:    value = OUTPUT;     break;
    default:
        qCCritical(WIRING_PI) << "Unimplemented case in pinMode";
    }
    ::pinMode(pin, value);
}
