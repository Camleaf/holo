#ifndef CAMLEAF_COPROC
#define CAMLEAF_COPROC

#include <cstdint>

const int MM_AXIS_X = 0;
const int MM_AXIS_Y = 1;
const int MM_AXIS_RX = 2;
const int MM_AXIS_RY = 3;

namespace mechmania {

    struct  [[gnu::packed]] RobotState{
        uint32_t buttons = 0;
        uint8_t dpad = 0;
        uint8_t misc_buttons;
        uint16_t axii[4] = {0,0,0,0}; // axisX, axisY, axisRX, axisRY
    };
}

extern mechmania::RobotState* rState;
#endif
