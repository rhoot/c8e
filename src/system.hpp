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

        enum Keys : uint16_t
        {
            KEY_0 = (1 <<  0),
            KEY_1 = (1 <<  1),
            KEY_2 = (1 <<  2),
            KEY_3 = (1 <<  3),
            KEY_4 = (1 <<  4),
            KEY_5 = (1 <<  5),
            KEY_6 = (1 <<  6),
            KEY_7 = (1 <<  7),
            KEY_8 = (1 <<  8),
            KEY_9 = (1 <<  9),
            KEY_A = (1 << 10),
            KEY_B = (1 << 11),
            KEY_C = (1 << 12),
            KEY_D = (1 << 13),
            KEY_E = (1 << 14),
            KEY_F = (1 << 15),
        };

        uint16_t op;
        uint8_t  mem[4096];
        Fb       fb;
        uint16_t keys;

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
