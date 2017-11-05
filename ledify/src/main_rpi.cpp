#ifdef __linux__
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <ws2811.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "definitions.h"
#include "main_rpi.h"
#include "SerialPort.h"
#include "LedStripController.h"
#include "LayerController.h"

#define NUM_LEDS 300
#define GPIO_PIN 18
#define DMA 5
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_GRBW

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

int main(int argc, char **argv) {
    char *virtSerial = 0;
    if ((argc > 2) && (strcmp(argv[1], "-D") == 0)) {
        virtSerial = argv[2];
        print("Override serial port: %s\n", virtSerial);
    }
    if (!setup(virtSerial)) {
        return 1;
    }
    while (running) {
        loop();
    }
    cleanup();
    return 0;
}

static void ctrl_c_handler(int signum) {
    (void)(signum);
    printf("Terminating...\n");
    running = false;
}

static void setup_handlers(void) {
    struct sigaction sa;
    sa.sa_handler = ctrl_c_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

bool setup(char *virtSerial) {
    if (virtSerial) {
        serial.begin(virtSerial, 9600);
    } else {
        serial.begin("/dev/ttyAMA0", 9600);
    }

    if (wiringPiSetup () == -1) {
        logerr("Unable to start wiringPi: %s\n", strerror (errno)) ;
        return false;
    }

    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
        return false;
    }

    LayerController &layerControl = controller.layerController();
    layerControl.addColorLayer(0, 0, 0, 0, 0);
    layerControl.addColorLayer(1, 60, 40, 5, 100);
    layerControl.addFadeLayer(2, 0, 1, 0, FadeLayer::InterpolatorAccelerate, 5000);
    layerControl.setAsRootLayer(2);

    setup_handlers();
    return true;
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

    // 90 frames /sec
    usleep(1000000 / 90);
}

#endif

