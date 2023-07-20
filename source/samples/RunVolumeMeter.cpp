#include <cmath>
#include "RunVolumeMeter.h"
#include "MicroBit.h"
#include "Utils.h"
#include "LevelDetector.h"
#include "LevelDetectorSPL.h"

#define MICROPHONE_MIN 52.0f
#define MICROPHONE_MAX 120.0f

extern MicroBit uBit;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int soundLevel()
{
    LevelDetectorSPL *level = uBit.audio.levelSPL;
    if (NULL == level)
        return 0;
    const int micValue = level->getValue();
    const int scaled = max(MICROPHONE_MIN, min(micValue, MICROPHONE_MAX)) - MICROPHONE_MIN;
    return min(0xff, scaled * 0xff / (MICROPHONE_MAX - MICROPHONE_MIN));
}

// static int read()
// {
//     // int mic = 0;
//     // const int n = 30;

//     uBit.audio.activateMic();

//     // for (uint8_t i = 0; i < n; i++)
//     // {
//     //     mic = soundLevel();
//     // }

//     // mapping values in range of LEDs
//     const int gauge = map(soundLevel(), 0, 100, 0, 5);

//     if (gauge < 0)
//     {
//         return 0;
//     }

//     if (gauge > 5)
//     {
//         return 5;
//     }

//     return gauge;
// }

void volumemeter_run()
{
    uBit.audio.activateMic();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveHandler);

    int gauges[5] = {};

    leave = false;
    while (!leave)
    {

        // shift values
        for (uint8_t i = 0; i < (5 - 1); i++)
        {
            gauges[i] = gauges[i + 1];
        }

        gauges[4] = map(soundLevel(), 0, 100, 0, 5);

        for (uint16_t x = 0; x < 5; x++)
        {
            const int gauge = gauges[x];
            for (uint16_t y = 0; y < 5; y++)
            {
                const uint8_t t = static_cast<uint8_t>(gauge > y ? 255 : 0);
                uBit.display.image.setPixelValue(x, 4 - y, t);
            }
        }

        int sum = 0;
        for (uint8_t i = 0; i < 5; i++)
        {
            sum += gauges[i];
        }

        if (sum > 5 * 3)
        {
            LEDTrafficLight(2);
        }
        else if (sum > 5 * 1.5)
        {
            LEDTrafficLight(1);
        }
        else
        {
            LEDTrafficLight(0);
        }

        uBit.sleep(100);
    }

    // uBit.rgb.off();

    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveHandler);
}