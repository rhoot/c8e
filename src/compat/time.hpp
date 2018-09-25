//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>
#include <time.h>

#if defined(_WIN32)

//
// clock_gettime polyfill
//


constexpr int32_t CLOCK_MONOTONIC = 0;
int32_t clock_gettime(int32_t, struct timespec* ts);

//
// nanosleep polyfill
//

int32_t nanosleep(const struct timespec* req, struct timespec* rem);

#endif // defined(_WIN32)
