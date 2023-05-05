#include "MicroBit.h"
#include "samples/Tests.h"

MicroBit uBit;

int main()
{
    uBit.init();
    uBit.display.scroll('W');
    while(1){
    uBit.display.scroll('w');
    cap_touch_test();
    uBit.sleep(3000);
    // uBit.io.P3.setServoValue(uBit.io.P16.getAnalogValue()/6);
    // uBit.io.P0.setServoValue(uBit.io.P16.getAnalogValue()/6);
    }
    // out_of_box_experience();
}