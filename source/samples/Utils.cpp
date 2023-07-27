#include "Utils.h"
#include "MicroBit.h"
#include "MicroBitImage.h"
#include "Images.h"

extern MicroBit uBit;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool event = false;
void eventHandler(MicroBitEvent)
{
    event = true;
}

bool leave = false;
void leaveHandler(MicroBitEvent)
{
    leave = true;
}

void blinkImage(const MicroBitImage &image, const int pos, const uint16_t delay)
{

    event = false;

    uBit.display.print(image, pos);

    for (uint32_t t = 0, i = 10; t < delay && !leave && !event; t += i)
    {
        uBit.sleep(i);
    }

    if (event)
    {
        return;
    }

    uBit.display.clear();

    for (uint32_t t = 0, i = 10; t < delay && !leave && !event; t += i)
    {
        uBit.sleep(i);
    }
}

void blinkImageUntilEvent(const uint16_t source, const uint16_t value, const MicroBitImage &image,
                          const int pos1, const int pos2, const uint16_t delay)
{
    uBit.messageBus.listen(source, value, eventHandler);

    event = false;

    for (uint8_t i = 0; !leave && !event; i++)
    {
        blinkImage(image, (i % 2) == 0 ? pos1 : pos2, delay);
    }

    uBit.messageBus.ignore(source, value, eventHandler);
}

// void moveImageUntilEvent(const uint16_t source, const uint16_t value, const MicroBitImage &image,
//                          const uint16_t delay)
// {
//     int direction = 1;
//     int offset = -3;
//     uBit.messageBus.listen(source, value, eventHandler);

//     event = false;

//     for (uint8_t i = 0; !leave && !event; i++)
//     {
//         for (int j = 0; j < 20; j++)
//         {

//             offset = offset + direction;
//             uBit.display.print(image, offset, 0, 0, delay);
//             if (offset >= 7)
//             {
//                 direction = -1;
//             }
//         }
//         direction = 1;
//     }
//     uBit.messageBus.ignore(source, value, eventHandler);
// }

void moveImageUntilEvent(
const uint16_t source,
const uint16_t value,
const MicroBitImage &image,
// const uint16_t cycles,
const uint16_t offset,
const int asymetric,
const uint16_t delay)
{
    int direction = 1;
    int x;

    uBit.messageBus.listen(source, value, eventHandler);

    event = false;

    for (uint8_t i = 0; !leave && !event; i++)
    {
        uBit.serial.send(x + "\r\n");
        x += direction; 
        if (x >= offset + asymetric)
            {
                direction = -1;
            } else if (x <= -offset ) {
                direction = 1;
            }
        uBit.display.print(image, x, 0, 0, delay);
    }
    uBit.messageBus.ignore(source, value, eventHandler);
}

// fire a accelerometer shake event with a 3G event
void shakeEvent(MicroBitEvent)
{
    MicroBitEvent evt(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE);
    uBit.messageBus.send(evt);
}

// LED functions
int brightness = 32;
const int strip_length = 3;
uint16_t pause = 300;

void LEDTrafficLight(int color)
{

    // Create a buffer to hold pixel data of the appropriate size
    ManagedBuffer b(strip_length * 3);
    b.fill(0);
    if (color == 0)
    {
        // turns 1st LED green
        b[0] = brightness;
    }
    else if (color == 1)
    {
        // turns 2nd LED yellow
        b[3] = brightness;
        b[4] = brightness;
    }
    else if (color == 2)
    {
        // turns 3rd LED red
        b[7] = brightness;
    }
    neopixel_send_buffer(uBit.io.RGB, b);
}

void disableLEDs()
{
    // Create a buffer to hold pixel data of the appropriate size
    ManagedBuffer b(strip_length * 3);
    b.fill(0);
    neopixel_send_buffer(uBit.io.RGB, b);
}

void startLEDs()
{
    // Brightness of pixels

    // Create a buffer to hold pixel data of the appropriate size
    ManagedBuffer b(strip_length * 3);

    // Fill the buffer with some data.
    // Neopixel typically has 3 bytes per pixel, for each primary colour
    // in GRB format (Green, then Red, then Blue).
    for (int i = 0; i < strip_length * 3; i++)
    {
        // Clear the buffer to zeroes. You don't need to do this, but it's often convenient.
        b.fill(0);
        b[i] = brightness;
        neopixel_send_buffer(uBit.io.RGB, b);
        // c = (c + 1) % 3;
        uBit.sleep(100);
        // Update the neopixel strip on pin P0.
    }
    for (int i = 0; i < strip_length; i++)
    {
        disableLEDs();
        uBit.sleep(pause / 2);
        b.fill(0);
        b[0 + i] = brightness;
        b[3 + i] = brightness;
        b[6 + i] = brightness;
        neopixel_send_buffer(uBit.io.RGB, b);
        // c = (c + 1) % 3;
        uBit.sleep(pause / 2);
        // Update the neopixel strip on pin P0.
        blinkImage(herz, 0, pause);
    }

    // Wait for a second.
}

void turnLEDon(int start_index, int end_index)
{
    brightness = 20;
    ManagedBuffer b(strip_length * 3);
    b.fill(0);
    for (int i = start_index * 3; i < end_index * 3; i++)
        b[i] = brightness;
    neopixel_send_buffer(uBit.io.RGB, b);
}

void LEDcounter(int number)
{
    ManagedBuffer b(strip_length * 3);
    b.fill(0);
    switch (number)
    {
    // 1
    case 1:
        turnLEDon(0, 1);
        break;
    // 2
    case 2:
        turnLEDon(0, 2);
        break;
    // 3
    case 3:
        turnLEDon(0, 3);
        break;
    // 4
    case 4:
        turnLEDon(1, 3);
        break;
    // 5
    case 5:
        turnLEDon(2, 3);
        break;
    }
}

void rainbow()
{
    uint16_t i, j;
    int WheelPos;
    ManagedBuffer b(3 * 3);

    for (j = 0; j < 250; j++)
    {
        for (i = 0; i < 3; i++)
        {
            WheelPos = i + j;
            WheelPos = 255 - WheelPos;
            if (WheelPos < 85)
            {
                b[0 + i * 3] = (255 - WheelPos * 3) / 10;
                b[1 + i * 3] = (0) / 10;
                b[2 + i * 3] = (WheelPos * 3) / 10;
            }
            if (WheelPos < 170)
            {
                WheelPos -= 85;
                b[0 + i * 3] = (0) / 10;
                b[1 + i * 3] = (WheelPos * 3) / 10;
                b[2 + i * 3] = (255 - WheelPos * 3) / 10;
            }
            if (WheelPos > 170 && WheelPos < 240)
            {
                WheelPos -= 170;
                b[0 + i * 3] = (WheelPos * 3) / 10;
                b[1 + i * 3] = (255 - WheelPos * 3) / 10;
                b[2 + i * 3] = (0) / 10;
            }
        }
        neopixel_send_buffer(uBit.io.RGB, b);
        uBit.sleep(10);
    }
}

void rainbow2()
{
    // Cycle a single RGB led using the sine fade technique
    // For common cathode led

    ManagedBuffer b(strip_length * 3);
    int delay = 1; // ms

    const uint8_t sins[360] = {
        0,   0,   0,   0,   0,   1,   1,   2,   2,   3,   4,   5,   6,   7,   8,   9,   11,  12,
        13,  15,  17,  18,  20,  22,  24,  26,  28,  30,  32,  35,  37,  39,  42,  44,  47,  49,
        52,  55,  58,  60,  63,  66,  69,  72,  75,  78,  81,  85,  88,  91,  94,  97,  101, 104,
        107, 111, 114, 117, 121, 124, 127, 131, 134, 137, 141, 144, 147, 150, 154, 157, 160, 163,
        167, 170, 173, 176, 179, 182, 185, 188, 191, 194, 197, 200, 202, 205, 208, 210, 213, 215,
        217, 220, 222, 224, 226, 229, 231, 232, 234, 236, 238, 239, 241, 242, 244, 245, 246, 248,
        249, 250, 251, 251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 254, 254,
        253, 253, 252, 251, 251, 250, 249, 248, 246, 245, 244, 242, 241, 239, 238, 236, 234, 232,
        231, 229, 226, 224, 222, 220, 217, 215, 213, 210, 208, 205, 202, 200, 197, 194, 191, 188,
        185, 182, 179, 176, 173, 170, 167, 163, 160, 157, 154, 150, 147, 144, 141, 137, 134, 131,
        127, 124, 121, 117, 114, 111, 107, 104, 101, 97,  94,  91,  88,  85,  81,  78,  75,  72,
        69,  66,  63,  60,  58,  55,  52,  49,  47,  44,  42,  39,  37,  35,  32,  30,  28,  26,
        24,  22,  20,  18,  17,  15,  13,  12,  11,  9,   8,   7,   6,   5,   4,   3,   2,   2,
        1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

    /* fade leds in */
    // for (int j = 0; j < 100; j++)
    // {
    //     for (size_t i = 0; i < 3; i++)
    //     {
    //         //
    //         b[0 + i * 3] = map(j, 0, 100, 0, map(sins[120 % 360], 0, 255, 0, brightness));
    //         b[1 + i * 3] = map(j, 0, 100, 0, map(sins[(120 + i* 120) % 360], 0, 255, 0,
    //         brightness)); b[2 + i * 3] = map(j, 0, 100, 0, map(sins[(240 + i * 120) % 360], 0,
    //         255, 0, brightness)); neopixel_send_buffer(uBit.io.RGB, b); uBit.sleep(1);
    //     }
    // }

    for (int k = 0; k < 360; k++)
    {

        for (int i = 0; i < strip_length; i++)
        {
            // green red blue
            b[0 + i * 3] = map(sins[(k + i * 120) % 360], 0, 255, 0, brightness);
            b[1 + i * 3] = map(sins[(k + 120 + i * 120) % 360], 0, 255, 0, brightness);
            b[2 + i * 3] = map(sins[(k + 240 + i * 120) % 360], 0, 255, 0, brightness);
            neopixel_send_buffer(uBit.io.RGB, b);
            uBit.sleep(delay);
        }
    }
}

bool mute_mic = false;

void playsound(int freq)
{
    if (mute_mic || freq == 0)
    {
        uBit.io.speaker.setAnalogValue(0);
        uBit.io.P0.setAnalogValue(0);
        return;
    }
    uBit.io.speaker.setHighDrive(true);
    uBit.io.speaker.setAnalogValue(512);
    uBit.io.P0.setHighDrive(true);
    uBit.io.P0.setAnalogValue(512);
    int period = 1000000.0 / (float)freq;

    uBit.io.speaker.setAnalogPeriodUs(period);
    uBit.io.P0.setAnalogPeriodUs(period);
    return;
}

// static void writeRgbLeds(const uint8_t led0r, const uint8_t led0g, const uint8_t led0b,
//                          const uint8_t led1r, const uint8_t led1g, const uint8_t led1b,
//                          const uint8_t led2r, const uint8_t led2g, const uint8_t led2b)
// {
//     // Create a buffer to hold pixel data of the appropriate size
//     ManagedBuffer b(strip_length * 3);
//     b.fill(0);
//     // b[0 + 3 * index] = ledg;
//     // b[1 + 3 * index] = ledr;
//     // b[2 + 3 * index] = ledb;

//     b[0] = led0g; // turns 1st LED green
//     b[1] = led0r;
//     b[2] = led0b;
//     b[3] = led1g;
//     b[4] = led1r; // turns 2nd LED red
//     b[5] = led1b;
//     b[6] = led2g;
//     b[7] = led2r;
//     b[8] = led2b; // turns 3rd LED blue
//     neopixel_send_buffer(uBit.io.RGB, b);
// }