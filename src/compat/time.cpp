//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#if defined(_WIN32)

#include "time.hpp"

#include <chrono>
#include <thread>

//
// clock_gettime polyfill
//

int32_t clock_gettime(int32_t, struct timespec* ts)
{
    static auto s_epoch = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    auto ns  = std::chrono::duration_cast<std::chrono::nanoseconds>(now - s_epoch).count();
    ts->tv_sec  = ns / 1000000000;
    ts->tv_nsec = ns % 1000000000;
    return 0;
}

//
// nanosleep polyfill
//

int32_t nanosleep(const struct timespec* req, struct timespec* rem)
{
    const uint64_t ns = uint64_t(req->tv_sec) * 1000000000 + req->tv_nsec;
    std::this_thread::sleep_for(std::chrono::nanoseconds{ns});

    if (rem)
    {
        rem->tv_sec  = 0;
        rem->tv_nsec = 0;
    }

    return 0;
}

#endif // defined(_WIN32)
