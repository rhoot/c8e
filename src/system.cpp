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

    static void fetchOpCode(System* sys)
    {
        const uint16_t hi = sys->mem[sys->pc++];
        const uint16_t lo = sys->mem[sys->pc++];
        sys->op = (hi << 8) | lo;
    }

    static void execOpCode(System* sys)
    {
        switch (sys->op & 0xF000)
        {
            case 0x0000:
                switch (sys->op)
                {
                    case 0x00EE: // 0x00EE : Returns from a subroutine.
                        sys->pc = sys->stack[--(sys->sp)];
                        break;

                    default:
                        goto unknown_op;
                }

                break;

            case 0x1000: // 0x1NNN : Jumps to address NNN.
                sys->pc = sys->op & 0x0FFF;
                break;

            case 0x2000: // 0x2NNN : Calls subroutine at NNN.
                sys->stack[(sys->sp)++] = sys->pc;
                sys->pc = sys->op & 0x0FFF;
                break;

            case 0x3000: // 0x3XNN : Skips the next instruction if VX equals NN.
            {
                const uint8_t reg = (sys->op & 0x0F00) >> 8;
                const uint8_t val = (sys->op & 0x00FF);
                sys->pc += (sys->V[reg] == val ? 2 : 0);
                break;
            }

            case 0x4000: // 0x4XNN : Skips the next instruction if VX doesn't equal NN.
            {
                const uint8_t reg = (sys->op & 0x0F00) >> 8;
                const uint8_t val = (sys->op & 0x00FF);
                sys->pc += (sys->V[reg] == val ? 0 : 2);
                break;
            }

            case 0x6000: // 0x6XNN : Sets VX to NN.
            {
                const uint8_t reg = (sys->op & 0x0F00) >> 8;
                const uint8_t val = (sys->op & 0x00FF);
                sys->V[reg] = val;
                break;
            }

            case 0x7000: // 0x7XNN : Adds NN to VX. (Carry flag is not changed)
            {
                const uint8_t reg = (sys->op & 0x0F00) >> 8;
                const uint8_t val = (sys->op & 0x00FF);
                sys->V[reg] += val;
                break;
            }

            case 0x8000:
                switch (sys->op & 0x000F)
                {
                    case 0x0000: // 0x8XY0 : Sets VX to the value of VY.
                    {
                        const uint8_t regX = (sys->op & 0x0F00) >> 8;
                        const uint8_t regY = (sys->op & 0x00F0) >> 4;
                        sys->V[regX] = sys->V[regY];
                        break;
                    }

                    case 0x0002: // 0x8XY2 : Sets VX to VX and VY. (Bitwise AND operation)
                    {
                        const uint8_t regX = (sys->op & 0x0F00) >> 8;
                        const uint8_t regY = (sys->op & 0x00F0) >> 4;
                        sys->V[regX] &= sys->V[regY];
                        break;
                    }

                    case 0x0004: // 0x8XY4 : Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                    {
                        const uint8_t regX = (sys->op & 0x0F00) >> 8;
                        const uint8_t regY = (sys->op & 0x00F0) >> 4;
                        const uint16_t res = sys->V[regX] + sys->V[regY];
                        sys->V[regX] = res;
                        sys->VF      = (res >> 8 ? 1 : 0);
                        break;
                    }

                    case 0x0005: // 0x8XY5 : VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    {
                        const uint8_t regX = (sys->op & 0x0F00) >> 8;
                        const uint8_t regY = (sys->op & 0x00F0) >> 4;
                        const uint16_t res = sys->V[regX] - sys->V[regY];
                        sys->V[regX] = res;
                        sys->VF      = (res >> 8 ? 1 : 0);
                        break;
                    }

                    default:
                        goto unknown_op;
                }

                break;

            case 0xA000: // 0xANNN : Sets I to the address NNN.
                sys->I = sys->op & 0x0FFF;
                break;

            case 0xC000: // 0xCXNN : Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
            {
                const uint8_t reg = (sys->op & 0x0F00) >> 8;
                const uint8_t val = (sys->op & 0x00FF);
                sys->V[reg] = uint8_t(rand() & val);
                break;
            }

            case 0xD000: // 0xDXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
                printf("IMPLEMENT: 0xDXYN\n");
                break;

            case 0xE000:
                switch (sys->op & 0x00FF)
                {
                    case 0x00A1: // Skips the next instruction if the key stored in VX isn't pressed.
                    {
                        printf("IMPLEMENT: 0xEXA1\n");
                        sys->pc += 2;
                        break;
                    }

                    default:
                        goto unknown_op;
                }

                break;

            case 0xF000:
                switch (sys->op & 0x00FF)
                {
                    case 0x0007: // 0xFX07 : Sets VX to the value of the delay timer.
                    {
                        const uint8_t reg = (sys->op & 0x0F00) >> 8;
                        sys->V[reg] = sys->delayTimer;
                        break;
                    }

                    case 0x0015: // 0xFX15 : Sets the delay timer to VX.
                    {
                        const uint8_t reg = (sys->op & 0x0F00) >> 8;
                        sys->delayTimer = sys->V[reg];
                        break;
                    }

                    case 0x0018: // 0xFX18 : Sets the sound timer to VX.
                    {
                        const uint8_t reg = (sys->op & 0x0F00) >> 8;
                        sys->soundTimer = sys->V[reg];
                        break;
                    }

                    case 0x0029: // 0xFX29 : Sets I to the location of the sprite for the character in VX.
                    {
                        const uint8_t reg = (sys->op & 0x0F00) >> 8;
                        sys->I = sys->V[reg] * 5;
                        break;
                    }

                    case 0x0033: // 0xFX33 : Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
                    {
                        const uint8_t reg = (sys->op & 0x0F00) >> 8;
                        sys->mem[sys->I + 0] = sys->V[reg] / 100;
                        sys->mem[sys->I + 1] = (sys->V[reg] / 10) % 10;
                        sys->mem[sys->I + 2] = sys->V[reg] % 10;
                        break;
                    }

                    case 0x0065: // 0xFX65 : Fills V0 to VX (including VX) with values from memory starting at address I.
                    {
                        const uint8_t bytes = (sys->op & 0x0F00) >> 8;
                        memcpy(sys->V, sys->mem + sys->I, bytes);
                        break;
                    }

                    default:
                        goto unknown_op;
                }

                break;

            default:
                goto unknown_op;
        }

        return;

    unknown_op:
        printf("unknown op code: 0x%4X\n", sys->op);
        assert(!"unknown op code");
    }

    void systemInit(System* sys)
    {
        sys->op = 0;
        sys->pc = 0x200;
        sys->I  = 0;
        sys->sp = 0;

        memcpy(sys->mem, CHIP8_FONT, sizeof(CHIP8_FONT));
    }

    void systemLoad(System* sys, const void* program, uint16_t programSize)
    {
        assert(sizeof(sys->mem) >= (programSize + 0x200));
        memcpy(sys->mem + 0x200, program, programSize);
    }

    void systemCycle(System* sys)
    {
        fetchOpCode(sys);
        execOpCode(sys);

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
