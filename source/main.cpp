#include "MicroBit.h"
#include "samples/Tests.h"

MicroBit uBit;



// Colour counter variable (just used to remember what colour we last displayed)
int c = 0;

int main()
{
    uBit.init();

    // Length of neopixel strip
    int length = 3;
    // Brightness of pixels
    int brightness = 32;
    // Create a buffer to hold pixel data of the appropriate size
    ManagedBuffer b(length*3);
    b.fill(0);
    b[0]=brightness; //turns 1st LED green
    b[1]=0;
    b[2]=0;
    b[3]=0;
    b[4]=brightness; //turns 2nd LED red
    b[5]=0;
    b[6]=0;
    b[7]=0;
    b[8]=brightness; //turns 3rd LED blue
    neopixel_send_buffer(uBit.io.RGB, b);

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