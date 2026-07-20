#ifndef CAMLEAF_COPROC
#define CAMLEAF_COPROC

#include <cstdint>






namespace mechmania {
    struct buttons {

        static const int AXIS_X = 0;
        static const int AXIS_Y = 1;
        static const int AXIS_RX = 2;
        static const int AXIS_RY = 3;
        static const int AXIS_L2 = 4;
        static const int AXIS_R2 = 5;

        static const int X = 0;
        static const int O = 1;
        static const int SQUARE = 2;
        static const int TRIANGLE = 3;
        static const int SHARE = 4;
        static const int HOME = 5;
        static const int OPTION = 6;
        static const int L3 = 7;
        static const int R3 = 8;
        static const int L1 = 9;
        static const int R1 = 10;
        static const int UP = 11;
        static const int DOWN = 12;
        static const int LEFT = 13;
        static const int RIGHT = 14;
    } ;


    struct  [[gnu::packed]] ControllerState{
        bool buttons[15] = {0};
        uint16_t axii[6] = {0};
    };
}

extern mechmania::ControllerState* cState;
#endif
