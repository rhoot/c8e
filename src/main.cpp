//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "compat/time.hpp"

#include <SFML/Graphics.hpp>

#include "system.hpp"


namespace c8e
{

    struct RenderCtx
    {
        sf::RenderWindow window;
        sf::Texture      texture;
        sf::Sprite       sprite;
    };


    struct Args
    {
        bool        step{false};
        bool        help{false};
        bool        log{false};
        const char* path{nullptr};
    };


    struct EventOpts
    {
        bool     step{false};
        uint16_t keys{0};
    };


    static void renderCtxInit(RenderCtx* ctx)
    {
        ctx->window.create(sf::VideoMode{640, 320}, "c8e", sf::Style::Titlebar | sf::Style::Close);
        ctx->texture.create(64, 32);
        ctx->sprite.setScale(10.0f, 10.0f);
        ctx->sprite.setTexture(ctx->texture);
    }


    static bool processEvents(sf::Window* window, EventOpts* opts)
    {
        bool run = window->isOpen();

        opts->step = false;

        if (run)
        {
            sf::Event event;

            while (window->pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window->close();
                        run = false;
                        break;

                    case sf::Event::KeyPressed:
                        switch (event.key.code)
                        {
                            case sf::Keyboard::Key::Space: opts->step = true; break;

                            case sf::Keyboard::Key::Num1:  opts->keys |= System::KEY_1; break;
                            case sf::Keyboard::Key::Num2:  opts->keys |= System::KEY_2; break;
                            case sf::Keyboard::Key::Num3:  opts->keys |= System::KEY_3; break;
                            case sf::Keyboard::Key::Num4:  opts->keys |= System::KEY_C; break;
                            case sf::Keyboard::Key::Q:     opts->keys |= System::KEY_4; break;
                            case sf::Keyboard::Key::W:     opts->keys |= System::KEY_5; break;
                            case sf::Keyboard::Key::E:     opts->keys |= System::KEY_6; break;
                            case sf::Keyboard::Key::R:     opts->keys |= System::KEY_D; break;
                            case sf::Keyboard::Key::A:     opts->keys |= System::KEY_7; break;
                            case sf::Keyboard::Key::S:     opts->keys |= System::KEY_8; break;
                            case sf::Keyboard::Key::D:     opts->keys |= System::KEY_9; break;
                            case sf::Keyboard::Key::F:     opts->keys |= System::KEY_E; break;
                            case sf::Keyboard::Key::Z:     opts->keys |= System::KEY_A; break;
                            case sf::Keyboard::Key::X:     opts->keys |= System::KEY_0; break;
                            case sf::Keyboard::Key::C:     opts->keys |= System::KEY_B; break;
                            case sf::Keyboard::Key::V:     opts->keys |= System::KEY_F; break;

                            default: break;
                        }

                        break;

                    case sf::Event::KeyReleased:
                        switch (event.key.code)
                        {
                            case sf::Keyboard::Key::Num1:  opts->keys &= ~System::KEY_1; break;
                            case sf::Keyboard::Key::Num2:  opts->keys &= ~System::KEY_2; break;
                            case sf::Keyboard::Key::Num3:  opts->keys &= ~System::KEY_3; break;
                            case sf::Keyboard::Key::Num4:  opts->keys &= ~System::KEY_C; break;
                            case sf::Keyboard::Key::Q:     opts->keys &= ~System::KEY_4; break;
                            case sf::Keyboard::Key::W:     opts->keys &= ~System::KEY_5; break;
                            case sf::Keyboard::Key::E:     opts->keys &= ~System::KEY_6; break;
                            case sf::Keyboard::Key::R:     opts->keys &= ~System::KEY_D; break;
                            case sf::Keyboard::Key::A:     opts->keys &= ~System::KEY_7; break;
                            case sf::Keyboard::Key::S:     opts->keys &= ~System::KEY_8; break;
                            case sf::Keyboard::Key::D:     opts->keys &= ~System::KEY_9; break;
                            case sf::Keyboard::Key::F:     opts->keys &= ~System::KEY_E; break;
                            case sf::Keyboard::Key::Z:     opts->keys &= ~System::KEY_A; break;
                            case sf::Keyboard::Key::X:     opts->keys &= ~System::KEY_0; break;
                            case sf::Keyboard::Key::C:     opts->keys &= ~System::KEY_B; break;
                            case sf::Keyboard::Key::V:     opts->keys &= ~System::KEY_F; break;

                            default: break;
                        };

                        break;

                    default:
                        break;
                }
            }
        }

        return run;
    }


    static void drawFb(RenderCtx* ctx, const System::Fb& fb)
    {
        // Convert from bit-array to RGBA.
        uint32_t pixels[64 * 32];
        uint32_t* px = pixels;

        for (int32_t y = 0; y < 32; ++y)
        {
            const uint64_t row = fb[y];

            for (int64_t x = 0; x < 64; ++x)
            {
                const uint64_t mask  = uint64_t(1) << (63 - x);
                const uint64_t bit   = row & mask;
                const uint32_t color = bit ? UINT32_MAX : 0;
                *(px++) = color;
            }
        }

        ctx->texture.update((const sf::Uint8*)pixels);

        ctx->window.clear(sf::Color::Black);
        ctx->window.draw(ctx->sprite);
        ctx->window.display();
    }

    static const char* findBasename(const char* path)
    {
        const char* basename = path;

        for (const char* ch = path; *ch; ++ch)
        {
            if (*ch == '/')
            {
                basename = ch + 1;
            }
        }

        return basename;
    }

    static void showUsage(const char* prg)
    {
        const char* basename = findBasename(prg);

        printf("%s: Yet another CHIP-8 emulator.\n", basename);
        printf("\n");
        printf("Usage:\n");
        printf("\n");
        printf("  %s --help\n", basename);
        printf("  %s [--log] [--step] <c8_path>\n", basename);
        printf("\n");
        printf("Arguments:\n");
        printf("\n");
        printf("  --help\tShow this help and exit.\n");
        printf("  --log \tPrint every instruction executed to stdout.\n");
        printf("  --step\tOnly cycle the CPU when space is pressed.\n");
        printf("  c8_path\tPath to the CHIP-8 ROM to run.\n");
        printf("\n");
    }

    static bool loadFileInto(const char* path, void* buffer, uint16_t maxSize)
    {
        bool success = false;

        if (FILE* file = fopen(path, "rb"))
        {
            fseek(file, 0, SEEK_END);
            const size_t size = ftell(file);
            fseek(file, 0, SEEK_SET);

            if (size && size <= maxSize)
            {
                const size_t read = fread(buffer, 1, size, file);
                success = (read == size);
            }

            fclose(file);
        }

        return success;
    }

    static void parseArgs(Args* args, int32_t argc, char** argv)
    {
        for (int32_t i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "--help") == 0)
            {
                args->help = true;
                continue;
            }

            if (strcmp(argv[i], "--step") == 0)
            {
                args->step = true;
                continue;
            }

            if (strcmp(argv[i], "--log") == 0)
            {
                args->log = true;
                continue;
            }

            if (!args->path)
            {
                args->path = argv[i];
            }
        }
    }

} // namespace c8e

int main(int argc, char** argv)
{
    srand(unsigned(time(nullptr)));

    // Parse arguments.
    c8e::Args args;
    c8e::parseArgs(&args, argc, argv);

    if (args.help || !args.path)
    {
        c8e::showUsage(argv[0]);
        return args.help ? 0 : 1;
    }

    // Init the system, and load the program.
    void* programBuf;
    uint16_t programMaxSize;

    c8e::System sys;
    c8e::systemInit(&sys);
    c8e::systemProgramMem(&sys, &programBuf, &programMaxSize);

    if (!c8e::loadFileInto(argv[1], programBuf, programMaxSize))
    {
        fprintf(stderr, "ERROR: Failed to load ROM %s.\n", argv[1]);
        return 1;
    }

    // Init rendering.
    c8e::RenderCtx render;
    c8e::renderCtxInit(&render);

    // Pump events.
    c8e::EventOpts opts;
    while (c8e::processEvents(&render.window, &opts))
    {
        // Only step the CPU if manual stepping is turned off, or if the step
        // button was pressed.
        if (!args.step || opts.step)
        {
            sys.keys = opts.keys;
            c8e::CycleOpts cycle;
            c8e::systemCycle(&sys, &cycle);

            if (cycle.fbUpdated)
            {
                c8e::drawFb(&render, sys.fb);
            }

            if (args.log)
            {
                char dasm[16];
                c8e::systemDisasm(sys.op, dasm);
                printf("%s\n", dasm);
            }
        }

        // Rate limit.
        constexpr int32_t CYCLE_HZ = 540;

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1000000000 / CYCLE_HZ;
        nanosleep(&ts, nullptr);
    }
}
