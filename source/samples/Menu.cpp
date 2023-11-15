#include "Menu.h"
#include "MicroBit.h"
#include "Utils.h"

extern MicroBit uBit;

menustate_t menuWaitForChoice(menustate_t start)
{
    menustate_t state = start;
    while (true) {

        // if (state == MenuStateInterpreter) {
        //     showNameHistogram(uBit.display);
        //     // static const uint8_t pixels[25] = {
        //     //     0, 0, 1, 1, 1,
        //     //     0, 0, 0, 1, 1,
        //     //     0, 0, 1, 0, 1,
        //     //     0, 1, 0, 0, 0,
        //     //     1, 0, 0, 0, 0
        //     // };
        //     // const MicroBitImage Image(5, 5, pixels);
        //     // uBit.display.print(Image);
        // } else {
        //     uBit.display.print(ManagedString(state));
        // }
        uBit.display.print(ManagedString(state));
        LEDcounter(state);

        // event loop
        while (true) {
            // button A
            if (uBit.buttonA.isPressed()) {
                while (uBit.buttonA.isPressed()) {
                    uBit.sleep(10);
                }

                if (state > MenuStateMin) {
                    state = menustate_t(state - 1);
                    break;
                }
            }

            // button B
            if (uBit.buttonB.isPressed()) {
                while (uBit.buttonB.isPressed()) {
                    uBit.sleep(10);
                }

                if (state < MenuStateMax) {
                    state = menustate_t(state + 1);
                    break;
                }
            }

            // gesture
            if (uBit.accelerometer.getGesture() == MICROBIT_ACCELEROMETER_EVT_SHAKE) {
                while (uBit.accelerometer.getGesture() == MICROBIT_ACCELEROMETER_EVT_SHAKE) {
                    uBit.sleep(10);
                }
                return state;
            }
            // reset to test mode by touching all 4 edges
            if (uBit.io.P0.isTouched() && uBit.io.P1.isTouched() && uBit.io.P2.isTouched() && uBit.io.P3.isTouched()){
            uBit.storage.remove("counter");   
            uBit.display.print("TestMode");
            uBit.reset(); 
            }
            uBit.sleep(10);
        }
    }
}