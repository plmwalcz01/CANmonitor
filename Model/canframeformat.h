#pragma once
#include <QWidget>

namespace CANMonitor{

    //for MCP-2515DM-BM demo board;
    struct canBaseFrameFormat
    {
        u_int8_t DLC;
        u_int8_t RTR;
        u_int8_t IDE;
        u_int32_t canID;
        u_int8_t data[8];
    };
}



