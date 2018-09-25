//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

namespace c8e
{

    struct System
    {
        using Fb = uint64_t[32];

        uint16_t op;
        uint8_t  mem[4096];
        Fb       fb;
        uint16_t buttons;

        uint8_t  delayTimer;
        uint8_t  soundTimer;
        struct timespec nextTick;

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


    struct CycleOpts
    {
        bool fbUpdated{false};
    };

    using DisasmStr = char[16];

    void systemInit(System* sys);
    void systemProgramMem(System* sys, void** o_buf, uint16_t* o_maxSize);
    void systemCycle(System *sys, CycleOpts* o_opts);
    void systemDisasm(uint16_t opcode, DisasmStr& o_str);

} // namespace c8e
