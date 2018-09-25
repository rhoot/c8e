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


    static void renderCtxInit(RenderCtx* ctx)
    {
        ctx->window.create(sf::VideoMode{640, 320}, "c8e", sf::Style::Titlebar | sf::Style::Close);
        ctx->texture.create(64, 32);
        ctx->sprite.setScale(10.0f, 10.0f);
        ctx->sprite.setTexture(ctx->texture);
    }


    static bool processEvents(sf::Window* window)
    {
        bool run = window->isOpen();

        if (run)
        {
            sf::Event event;

            while (window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window->close();
                    run = false;
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

} // namespace c8e

int main(int argc, char** argv)
{
    srand(time(nullptr));

    if (argc == 1)
    {
        c8e::showUsage(argv[0]);
        return 1;
    }

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

    c8e::RenderCtx render;
    c8e::renderCtxInit(&render);

    while (c8e::processEvents(&render.window))
    {
        c8e::systemCycle(&sys);

        if (sys.drawFlag)
        {
            c8e::drawFb(&render, sys.fb);
            sys.drawFlag = false;
        }

        constexpr int32_t CYCLE_HZ = 540;

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1000000000 / CYCLE_HZ;
        nanosleep(&ts, nullptr);
    }
}
