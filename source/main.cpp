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
#include "ProgramFive.h"
#include "Utils.h"
#include "Tests.h"
// #include "neopixel.h"
// #include "Interpreter.h"
// #include "nrf.h"

MicroBit uBit;

static bool mute = false;
// Length of neopixel strip
const uint16_t pause = 300;

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

static void menuAnimateEnter()
{
    disableLEDs();
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
    disableLEDs();
}

int main()
{

    // uBit.accelerometer.updateSample();
    uBit.init();

    // while (1)
    // {
    //        uBit.serial.printf("%d \n", soundLevel());
    //        uBit.sleep(100);
    // }

    // fire a accelerometer shake event with a 3G event
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_3G, shakeEvent);

    uBit.display.setBrightness(100);
    disableLEDs();

    uBit.serial.send("Calliope Demo v3.0\r\n");

    // check if DEMO has already been played
    // if (!hasStorageKey(KEY_DEMO))
    // {
    //     setStorageKey(KEY_DEMO);

    // begin of welcome DEMO program
    startSound();

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
    blinkImageUntilEvent(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, doublerow, -1, 2,
                         100);
    uBit.display.print(check);
    uBit.sleep(pause);
    uBit.display.clear();

    uBit.display.scroll("OK!");

    // RGB LEDs
    startLEDs();
    disableLEDs();
    uBit.display.clear();

    uBit.display.print(smiley);
    uBit.sleep(1000);
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
            menuAnimateEnter();
            programfive_run();
            menuAnimateLeave();
            break;
        }
        LEDcounter(state);
    }
}

