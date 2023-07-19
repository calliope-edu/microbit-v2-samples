#include "MicroBit.h"
#include "Storage.h"
#include "Images.h"
// #include "RunTests.h"
// #include "RunDemo.h"
#include "Menu.h"
#include "RunOracle.h"
#include "RunRockPaperScissors.h"
#include "RunMultiplication.h"
#include "RunVolumeMeter.h"
#include "Utils.h"
#include "Tests.h"
// #include "neopixel.h"
// #include "Interpreter.h"
// #include "nrf.h"

MicroBit uBit;

// static void showNameHistogram(MicroBitDisplay &display)
// {
//     NRF_FICR_Type *ficr = NRF_FICR;
//     uint32_t n = ficr->DEVICEID[1];
//     uint32_t ld = 1;
//     uint32_t d = MICROBIT_DFU_HISTOGRAM_HEIGHT;
//     uint32_t h;

//     display.clear();
//     for (uint32_t i = 0; i < MICROBIT_DFU_HISTOGRAM_WIDTH; i++)
//     {
//         h = (n % d) / ld;

//         n -= h;
//         d *= MICROBIT_DFU_HISTOGRAM_HEIGHT;
//         ld *= MICROBIT_DFU_HISTOGRAM_HEIGHT;

//         for (uint32_t j = 0; j < h + 1; j++)
//             display.image.setPixelValue(
//                 static_cast<int16_t>(MICROBIT_DFU_HISTOGRAM_WIDTH - i - 1),
//                 static_cast<int16_t>(MICROBIT_DFU_HISTOGRAM_HEIGHT - j - 1),
//                 255);
//     }
// }

static bool mute = false;
// Length of neopixel strip
const int strip_length = 3;

// ---------------------------
static void playsound(int freq)
{
    if (mute || freq == 0)
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

static int readMic()
{
    int v;
    uBit.audio.activateMic();
    uBit.sleep(100);                     // seems to be necessary
    v = uBit.audio.levelSPL->getValue(); // what is SPL?
    uBit.serial.printf("%d \n", v);
    uBit.sleep(100);
    return v;
}

static inline void waitForever()
{
    while (true)
    {
        uBit.sleep(1000);
    }
}

static void beep()
{
    playsound(784);
    uBit.sleep(125);
    playsound(0);
}

static void startSound()
{
    playsound(262);
    uBit.sleep(125);
    playsound(0);
    uBit.sleep(63);
    playsound(784);
    uBit.sleep(500);
    playsound(0);
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

void neopixel_write()
{
    // Brightness of pixels
    int brightness = 50;

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
        b.fill(0);
        b[0 + i] = brightness;
        b[3 + i] = brightness;
        b[6 + i] = brightness;
        neopixel_send_buffer(uBit.io.RGB, b);
        // c = (c + 1) % 3;
        uBit.sleep(300);
        // Update the neopixel strip on pin P0.
        blinkImage(herz, 0, 300);
    }

    // Increment our colour counter so that we show a different colour in the next frame.

    // Wait for a second.
}

static void disableRgbLeds()
{
    // Create a buffer to hold pixel data of the appropriate size
    ManagedBuffer b(strip_length * 3);
    b.fill(0);
    neopixel_send_buffer(uBit.io.RGB, b);
}

static void menuAnimateEnter()
{
    uBit.display.print(dot);
    uBit.sleep(200);
    uBit.display.print(rect_s);
    uBit.sleep(200);
    uBit.display.print(rect_l);
    uBit.sleep(200);
    uBit.display.clear();
}

static void menuAnimateLeave()
{
    uBit.display.print(rect_l);
    uBit.sleep(200);
    uBit.display.print(rect_s);
    uBit.sleep(200);
    uBit.display.print(dot);
    uBit.sleep(200);
    uBit.display.clear();
}

const uint16_t pause = 300;


// fire a accelerometer shake event with a 3G event
static void shakeEvent(MicroBitEvent)
{
    MicroBitEvent evt(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE);
    uBit.messageBus.send(evt);
}

int main()
{

    // uBit.accelerometer.updateSample();
    uBit.init();
    // fire a accelerometer shake event with a 3G event
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_3G, shakeEvent);

    uBit.display.setBrightness(100);

    uBit.serial.send("Calliope Demo v3.0\r\n");

    //check if DEMO has already been played
    // if (!hasStorageKey(KEY_DEMO))
    // {
    //     setStorageKey(KEY_DEMO);

        // begin of welcome DEMO program
        startSound();
        // mems_mic_test();
        // start state

        uBit.display.scroll("Hi");

        // press A
        uBit.display.print("A");
        uBit.sleep(pause);
        blinkImageUntilEvent(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, arrow_left);
        uBit.display.print(check);
        uBit.sleep(pause);
        uBit.display.clear();

        // press B
        uBit.display.print("B");
        uBit.sleep(pause);
        blinkImageUntilEvent(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, arrow_right);
        uBit.display.print(check);
        uBit.sleep(pause);
        uBit.display.clear();

        // press A+B
        uBit.display.scroll("A+B");
        uBit.sleep(pause);
        blinkImageUntilEvent(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, arrow_left_right);
        beep();
        uBit.display.print(check);
        uBit.sleep(pause);
        uBit.display.clear();

        // shake
        // uBit.display.scroll(LS_DEMO_SHAKE);
        blinkImageUntilEvent(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, doublerow, -1,
                             2, 100);
        uBit.display.print(check);
        uBit.sleep(pause);
        uBit.display.clear();

        uBit.display.scroll("OK!");

        // RGB LEDs
        neopixel_write();
        disableRgbLeds();
    // } // end storage key DEMO

    menustate_t state = MenuStateOracle;

    while (true)
    {
        // for(int i = 0; i<100; i++) {
        //     uBit.sleep(10);
        //     threeLEDs(0,0,0,i,i,i,0,0,0);
        // }
        // const int v = uBit.io.microphone.getAnalogValue();
        // uBit.serial.send();
        // uBit.serial.printf("%d \n",readMic());

        state = menuWaitForChoice(state);
        switch (state)
        {
        // 1
        case MenuStateOracle:
            menuAnimateEnter();
            oracle_run();
            menuAnimateLeave();
            break;
        // 2
        case MenuStateRockPaperScissors:
            menuAnimateEnter();
            rockpaperscissors_run();
            menuAnimateLeave();
            break;
        // 3
        case MenuStateMultiplication:
            menuAnimateEnter();
            multiplication_run();
            menuAnimateLeave();
            break;
        // 4
        case MenuStateVolumeMeter:
            menuAnimateEnter();
            volumemeter_run();
            menuAnimateLeave();
            break;
        // 5
        case MenuStateInterpreter:
            setStorageKey(KEY_INTERPRETER);
            uBit.reset();
            break;
        }
    }
}

