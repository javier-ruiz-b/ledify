#ifndef ARDUINO
#include <signal.h>
#include <unistd.h>

#include "definitions.h"
#include "main_rpi.h"
#include "SerialPort.h"
#include "rpi_ws281x/ws2811.h"
#include "LedStripController.h"

#define NUM_LEDS 300
#define GPIO_PIN 18
#define DMA 5
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_RGBW   // SK6812RGBW (NOT SK6812RGB)

ws2811_t ledStrip {
    .render_wait_time = 0,
    .device = nullptr,
    .rpi_hw = nullptr,
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = NUM_LEDS,
            .strip_type = STRIP_TYPE,
            .leds = nullptr,
            .brightness = 255,
            .wshift = 0,
            .rshift = 0,
            .gshift = 0,
            .bshift = 0,
            .gamma = 0
        }
    },
};

SerialPort serial;
LedStripController controller;

static void ctrl_c_handler(int signum) {
    (void)(signum);
    running = false;
}

static void setup_handlers(void) {
    struct sigaction sa;
    sa.sa_handler = ctrl_c_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}


void setup() {
    serial.begin("/dev/ttyAMA0", 9600, SerialPort::ParityNone);

    setup_handlers();
    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
        return;
    }
}

void cleanup() {
    ws2811_fini(&ledStrip);
    printf("Finished\n");
}

void loop() {

    if (serial.available()) {
        char c = serial.read();
        print("%c", c);
        controller.writeChar(c);
    }

    controller.draw(static_cast<uint32 *>(ledStrip.channel[0].leds), NUM_LEDS);

    ws2811_return_t errCode;
    if ((errCode = ws2811_render(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(errCode));
        running = false;
    }

    // 120 frames /sec
//    usleep(1000000 / 120);
}

#endif
