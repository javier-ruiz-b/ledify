#ifndef ARDUINO
#pragma once

void setup();
void cleanup();
void loop();

static bool running = true;

int main (int, char **) {
    setup();
    while (running) {
        loop();
    }
    cleanup();
    return 0;
}


#endif // MAIN_RPI_H
