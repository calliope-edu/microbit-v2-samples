#ifndef IMAGES_H
#define IMAGES_H

#include "MicroBitImage.h"

extern MicroBitImage smiley;
extern MicroBitImage sad_smiley;
extern MicroBitImage rect_l;
extern MicroBitImage rect_s;
extern MicroBitImage dot;
extern MicroBitImage arrow_left;
extern MicroBitImage arrow_right;
extern MicroBitImage arrow_left_right;
extern MicroBitImage check;
extern MicroBitImage doublerow;
extern MicroBitImage herz;
extern MicroBitImage rock;
extern MicroBitImage paper;
extern MicroBitImage scissor;
extern MicroBitImage multiply;

enum Image_t{
    ImageSmiley,
    ImageSadly,
    ImageHeart,
    ImageArrowLeft,
    ImageArrowRight,
    ImageArrowLeftRight,
    ImageFull,
    ImageDot,
    ImageSmallRect,
    ImageLargeRect,
    ImageDoubleRow,
    ImageTick,
    ImageRock,
    ImageScissors,
    ImageWell,
    ImageFlash,
    ImageWave,
    ImageMultiplier,
    ImageCOUNTER
};

MicroBitImage *images(Image_t index);

#endif // IMAGES_H