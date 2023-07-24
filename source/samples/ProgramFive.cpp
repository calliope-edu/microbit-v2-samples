    #include "ProgramFive.h"
    #include "MicroBit.h"
    #include "Utils.h"
    
    extern MicroBit uBit;

    // visual ID code constants

static void showNameHistogram(MicroBitDisplay &display)
{
    uint16_t pixel_length = 5;
    NRF_FICR_Type *ficr = NRF_FICR;
    uint32_t n = ficr->DEVICEID[1];
    uint32_t ld = 1;
    uint32_t d = pixel_length;
    uint32_t h;

    display.clear();
    for (uint32_t i = 0; i < pixel_length; i++)
    {
        h = (n % d) / ld;

        n -= h;
        d *= pixel_length;
        ld *= pixel_length;

        for (uint32_t j = 0; j < h + 1; j++)
            display.image.setPixelValue(static_cast<int16_t>(pixel_length - i - 1),
                                        static_cast<int16_t>(pixel_length - j - 1), 255);
    }
}

void programfive_run()
{
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveHandler);

    leave = false;
    while (!leave) {

        if (leave) {
            break;
        }

        showNameHistogram(uBit.display);

        uBit.sleep(100);
        uBit.display.clear();
    }

        uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveHandler);
}