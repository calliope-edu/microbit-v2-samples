#include <cmath>
#include "RunTests.h"
#include "MicroBit.h"
//#include "MicroBitSerial.h"
#include "Images.h"
#include "Utils.h"
#include "GPIOTest.h"

extern MicroBit uBit;

static void showAllLEDs()
{
    uint16_t pixel_length = 25;

    uBit.display.clear();
    for (uint32_t i = 0; i < pixel_length; i++)
    {
        uBit.display.image.setPixelValue(i % 5, i / 5, 255);
    }
}

void tests_run()
{
    // uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.serial.send("begin test");
    uBit.sleep(500);
    // initializes accelerometer. It's automatically initialized by creating an object. If
    // requestUpdate could not save data in the buffer it will throw an 050 error
    uBit.accelerometer.requestUpdate();
    uBit.serial.send("accelerometer found");

    showAllLEDs();
    uBit.serial.send("show all LEDs on display\r\n");
    turnLEDon(0, 3);
    uBit.serial.send("show all neopixel LEDs\r\n");

    while (true)
    {
        // button A
        if (uBit.buttonA.isPressed())
        {

            uBit.serial.send("button A pressed\r\n");
            playsound(300);
            uBit.sleep(500);
            uBit.serial.send("play sound\r\n");
            playsound(0);
            break;
        }
        if (uBit.buttonB.isPressed())
        {
            uBit.serial.send("button B pressed\r\n");
            edge_connector_test();
        }
    }

    if (uBit.accelerometer.getZ() != 0)
    {
        uBit.serial.send("accelerometer z-axis not 0\r\n");
        uBit.display.print(check);
    }
    else
    {
        uBit.display.clear();
    }
    uBit.serial.send("end test\r\n");
    disableLEDs();
}