#ifndef ARDUINO
#pragma once

void setup();
void loop();

static bool running = true;

int main (int argc, char **argv) {
    setup();
    while (running) {
        loop();
    }
    return 0;
}


#endif // MAIN_RPI_H
