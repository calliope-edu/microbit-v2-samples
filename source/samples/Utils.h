#ifndef UTILS_H
#define UTILS_H

#include "MicroBit.h"

long map(long x, long in_min, long in_max, long out_min, long out_max);

extern bool event;
void leaveHandler(MicroBitEvent);

extern bool leave;
void eventHandler(MicroBitEvent);

void blinkImage(
    const MicroBitImage& image,
    const int pos = 0,
    const uint16_t delay = 200);

void blinkImageUntilEvent(
    const uint16_t source,
    const uint16_t value,
    const MicroBitImage& image,
    const int pos1 = 0,
    const int pos2 = 0,
    const uint16_t delay = 200);

// void moveImageUntilEvent(
//     const uint16_t source, 
//     const uint16_t value, 
//     const MicroBitImage &image,
//     const uint16_t delay = 30);

void moveImageUntilEvent(
    const uint16_t source,
    const uint16_t value,
    const MicroBitImage &image,
    const uint16_t offset = 3,
    const int asymetric = 0,
    const uint16_t delay = 30);

void shakeEvent(MicroBitEvent);

void LEDTrafficLight(int color);

void disableLEDs();

void startLEDs();

void turnLEDon(int start_index, int end_index);

void LEDcounter(int number);

void rainbow();

void rainbow2();

extern bool mute_mic;
void playsound(int freq);



#endif // UTILS_H