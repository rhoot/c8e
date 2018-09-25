#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <time.h>

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
        bool step{false};
        bool help{false};
        bool log{false};
        const char* path{nullptr};
    };


    struct EventOpts
    {
        bool step{false};
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
                        if (event.key.code == sf::Keyboard::Key::Space)
                        {
                            opts->step = true;
                        }
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
        printf("Usage: %s <c8_path>\n", findBasename(prg));
        printf("\n");
        printf("Yet another CHIP-8 emulator.\n");
        printf("\n");
        printf("    c8_path:\tPath to the CHIP-8 ROM to run.\n");
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
    srand(time(nullptr));

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

        // Reset event options.
        opts = c8e::EventOpts{};
    }
}
