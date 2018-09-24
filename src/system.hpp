#pragma once

namespace c8e
{

    struct System
    {
        uint16_t op;
        uint8_t  mem[4096];
        uint8_t  fb[256];
        uint16_t buttons;

        uint8_t  delayTimer;
        uint8_t  soundTimer;

        uint16_t stack[16];
        int8_t   sp;

        uint16_t I;
        uint16_t pc;

        union
        {
            struct
            {
                uint8_t V0;
                uint8_t V1;
                uint8_t V2;
                uint8_t V3;
                uint8_t V4;
                uint8_t V5;
                uint8_t V6;
                uint8_t V7;
                uint8_t V8;
                uint8_t V9;
                uint8_t VA;
                uint8_t VB;
                uint8_t VC;
                uint8_t VD;
                uint8_t VE;
                uint8_t VF;
            };

            uint8_t V[16];
        };

    }; // struct System

    void systemInit(System* sys);
    void systemLoad(System* sys, const void* program, uint16_t programSize);
    void systemCycle(System *sys);

} // namespace c8e
