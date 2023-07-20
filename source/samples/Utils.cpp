#include "Utils.h"
#include "MicroBit.h"
#include "MicroBitImage.h"
#include "Images.h"

extern MicroBit uBit;

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
        turnLEDon(0,1);
        break;
    // 2
    case 2:
        turnLEDon(0,2);
        break;
    // 3
    case 3:
        turnLEDon(0,3);
        break;
    // 4
    case 4:
        turnLEDon(1,3);
        break;
    // 5        
    case 5:
        turnLEDon(2,3);
        break;
    }
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