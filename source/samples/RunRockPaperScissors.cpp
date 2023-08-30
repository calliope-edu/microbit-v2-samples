#include "RunRockPaperScissors.h"
#include "Images.h"
#include "MicroBit.h"
#include "Utils.h"

extern MicroBit uBit;

void rockpaperscissors_run()
{
    const uint16_t pause = 200;

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.display.print(rock, 0, 0, 0, pause * 2);
    uBit.display.print(paper, 0, 0, 0, pause * 2);
    uBit.display.print(scissor, 0, 0, 0, pause * 2);
    // uBit.display.print(*images(ImageWell), 0, 0, 0, pause * 2);
    uBit.display.clear();
    uBit.sleep(pause);

    leave = false;
    while (!leave) {

    moveImageUntilEvent(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, doublerow, 3, 1, 40);

        if (leave) {
            break;
        }

        int r = uBit.random(300);
        if (r < 100) {
            uBit.display.print(rock);
        } else if (r < 200) {
            uBit.display.print(paper);
        }  else  {
            uBit.display.print(scissor);
        // } else {
        //     uBit.display.print(*images(ImageWell));
        }

        uBit.sleep(3000);
        uBit.display.clear();
    }

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
}