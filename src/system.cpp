#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "system.hpp"

namespace c8e
{

    static const uint8_t CHIP8_FONT[] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void systemInit(System* sys)
    {
        sys->op        = 0;
        sys->reg.pc    = 0x200;
        sys->reg.I     = 0;
        sys->stack.sp  = 0;

        memcpy(sys->mem, CHIP8_FONT, sizeof(CHIP8_FONT));
    }

    void systemLoad(System* sys, const void* program, uint16_t programSize)
    {
        assert(sizeof(sys->mem) >= (programSize + 0x200));
        memcpy(sys->mem + 0x200, program, programSize);
    }

    void systemCycle(System* sys)
    {
        const uint16_t hi = sys->mem[sys->reg.pc++];
        const uint16_t lo = sys->mem[sys->reg.pc++];
        sys->op = (hi << 8) | lo;

        switch (sys->op & 0xF000)
        {
            case 0xA000: // 0xANNN : Sets I to the address NNN.
                sys->reg.I = sys->op & 0x0FFF;
                break;

            default:
                printf("unknown op code: %#6x\n", sys->op);
                assert(!"unknown op code");
                break;
        }

        if (sys->delayTimer > 0)
        {
            --sys->delayTimer;
        }

        if (sys->soundTimer > 0)
        {
            if (--sys->soundTimer == 0)
            {
                printf("Beep!\n");
            }
        }

    }

} // namespace c8e
