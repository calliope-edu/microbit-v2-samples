#include "MicroBit.h"
#include "samples/Tests.h"

MicroBit uBit;

int main()
{
    uBit.init();
    
    const char * const happy_emoji ="\
    000,255,000,255,000\n\
    000,000,000,000,000\n\
    255,000,000,000,255\n\
    000,255,255,255,000\n\
    000,000,000,000,000\n";
    MicroBitImage smile(happy_emoji);

    ManagedBuffer b(3*3);
    b.fill(0);
    
    
    b[0]=32; //turns 1st LED green
    b[4]=32; //turns 2nd LED red
    b[8]=32; //turns 3rd LED blue
    neopixel_send_buffer(uBit.io.RGB, b);

    uBit.display.scroll("hi");
    uBit.display.print(smile);
    uBit.sleep(1000);
    version_test();
    // uBit.power.deepSleepAll();
    while(1){

    }
    // out_of_box_experience();

    // microbit_panic( 999 );
}

