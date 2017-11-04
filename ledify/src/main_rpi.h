#ifndef ARDUINO
#pragma once

void setup(char *virtSerial);
void cleanup();
void loop();

static bool running = true;

int main (int argc, char **argv);


#endif // MAIN_RPI_H
