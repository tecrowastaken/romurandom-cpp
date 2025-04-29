/* Romu Random Implemented is partially unlicensed under the Apache-2.0 & UNLICENSE LICENSE
 * C++ Classes are unlicensed so feel free to copy and paste any parts of the code that your
 * looking to implement.
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 *
 * */

// Romu Pseudorandom Number Generators
//
// Copyright 2020 Mark A. Overton
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ------------------------------------------------------------------------------------------------
//
// Website: romu-random.org
// Paper:   http://arxiv.org/abs/2002.11331

#ifndef __ROMU_RANDOM_HPP__
#define __ROMU_RANDOM_HPP__

#define ROTL(d, lrot) ((d << (lrot)) | (d >> (8 * sizeof(d) - (lrot))))

#include <stdint.h>
#include <ctime>
#include <array>

namespace Romu
{

class SplitMix32
{
private:
    uint32_t m_state;

public:
    constexpr uint32_t next()
    {
        m_state = 1664525u * (m_state + 314159265u);
        uint32_t z = m_state;
        z = (z ^ (z >> 15)) * 0x5ce4e5b9u;
        z = (z ^ (z >> 13)) * 0x1331c1ebu;
        return z ^ (z >> 15);
    }

    SplitMix32()
    {
        m_state = (time(nullptr) >> 32);
    }

    SplitMix32(uint32_t state)
    {
        m_state = state;
    }
};

class SplitMix
{
private:
    uint64_t m_state; /* The state can be seeded with any value. */

public:
    constexpr uint64_t next()
    {
        uint64_t z = (m_state += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

    SplitMix()
    {
        m_state = static_cast<uint64_t>(time(nullptr));
    }

    SplitMix(uint64_t state)
    {
        m_state = state;
    }
};

class Quad
{

private:
    std::array<uint64_t, 4> m_state;

public:
    Quad(uint64_t w, uint64_t x, uint64_t y, uint64_t z)
    {
        m_state[0] = w;
        m_state[1] = x;
        m_state[2] = y;
        m_state[3] = z;
    }

    Quad(uint64_t state[4])
    {
        m_state[0] = state[0];
        m_state[1] = state[1];
        m_state[2] = state[2];
        m_state[3] = state[3];
    }

    Quad(std::array<uint64_t, 4> state) : m_state(state) {}

    Quad()
    {
        SplitMix smix = SplitMix();
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    void seed(uint64_t s)
    {
        SplitMix smix = SplitMix(s);
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    constexpr std::array<uint64_t, 4> state()
    {
        return m_state;
    }

    uint64_t next()
    {
        uint64_t wp = m_state[0], xp = m_state[1], yp = m_state[2], zp = m_state[3];
        m_state[0] = 15241094284759029579u * zp; // a-mult
        m_state[1] = zp + ROTL(wp, 52);          // b-rotl, c-add
        m_state[2] = yp - xp;                    // d-sub
        m_state[3] = yp + wp;                    // e-add
        m_state[3] = ROTL(m_state[3], 19);       // f-rotl
        return xp;
    }
};

class Trio
{
private:
    std::array<uint64_t, 3> m_state;

public:
    Trio(uint64_t w, uint64_t x, uint64_t y)
    {
        m_state[0] = w;
        m_state[1] = x;
        m_state[2] = y;
    }

    Trio(uint64_t state[3])
    {
        m_state[0] = state[0];
        m_state[1] = state[1];
        m_state[2] = state[2];
    }

    Trio(std::array<uint64_t, 3> state) : m_state(state) {}

    Trio()
    {
        SplitMix smix = SplitMix();
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    void seed(uint64_t s)
    {
        SplitMix smix = SplitMix(s);
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    constexpr std::array<uint64_t, 3> state()
    {
        return m_state;
    }

    uint64_t next()
    {
        uint64_t xp = m_state[0], yp = m_state[1], zp = m_state[2];
        m_state[0] = 15241094284759029579u * zp;
        m_state[1] = yp - xp;
        m_state[1] = ROTL(m_state[1], 12);
        m_state[2] = zp - yp;
        m_state[2] = ROTL(m_state[2], 44);
        return xp;
    }
};

class Duo
{
private:
    std::array<uint64_t, 2> m_state;

public:
    Duo(uint64_t w, uint64_t x)
    {
        m_state[0] = w;
        m_state[1] = x;
    }

    Duo(uint64_t state[2])
    {
        m_state[0] = state[0];
        m_state[1] = state[1];
    }

    Duo(std::array<uint64_t, 2> state) : m_state(state) {}

    Duo()
    {
        SplitMix smix = SplitMix();
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    void seed(uint64_t s)
    {
        SplitMix smix = SplitMix(s);
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    constexpr std::array<uint64_t, 2> state()
    {
        return m_state;
    }

    uint64_t random()
    {
        uint64_t xp = m_state[0];
        m_state[0] = 15241094284759029579u * m_state[1];
        m_state[1] = ROTL(m_state[1], 36) + ROTL(m_state[1], 15) - xp;
        return xp;
    }
};

class DuoJr
{
private:
    std::array<uint64_t, 2> m_state;

public:
    DuoJr(uint64_t w, uint64_t x)
    {
        m_state[0] = w;
        m_state[1] = x;
    }

    DuoJr(uint64_t state[2])
    {
        m_state[0] = state[0];
        m_state[1] = state[1];
    }

    DuoJr(std::array<uint64_t, 2> state) : m_state(state) {}

    DuoJr()
    {
        SplitMix smix = SplitMix();
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    void seed(uint64_t s)
    {
        SplitMix smix = SplitMix(s);
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    constexpr std::array<uint64_t, 2> state()
    {
        return m_state;
    }

    uint64_t next()
    {
        uint64_t xp = m_state[0];
        m_state[0] = 15241094284759029579u * m_state[1];
        m_state[1] = m_state[1] - xp;
        m_state[1] = ROTL(m_state[1], 27);
        return xp;
    }
};

class Quad32
{

private:
    std::array<uint32_t, 4> m_state;

public:
    Quad32(uint32_t w, uint32_t x, uint32_t y, uint32_t z)
    {
        m_state[0] = w;
        m_state[1] = x;
        m_state[2] = y;
        m_state[3] = z;
    }

    Quad32(uint32_t state[4])
    {
        m_state[0] = state[0];
        m_state[1] = state[1];
        m_state[2] = state[2];
        m_state[3] = state[3];
    }

    Quad32(std::array<uint32_t, 4> state) : m_state(state) {}

    Quad32()
    {
        SplitMix32 smix = SplitMix32();
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    void seed(uint32_t s)
    {
        SplitMix32 smix = SplitMix32(s);
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    constexpr std::array<uint32_t, 4> state()
    {
        return m_state;
    }

    uint32_t next()
    {
        uint32_t wp = m_state[0], xp = m_state[1], yp = m_state[2], zp = m_state[3];
        m_state[0] = 3323815723u * zp;    // a-mult
        m_state[1] = zp + ROTL(wp, 26);   // b-rotl, c-add
        m_state[2] = yp - xp;             // d-sub
        m_state[3] = yp + wp;             // e-add
        m_state[3] = ROTL(m_state[3], 9); // f-rotl
        return xp;
    }
};

class Trio32
{
private:
    std::array<uint32_t, 3> m_state;

public:
    Trio32(uint32_t w, uint32_t x, uint32_t y)
    {
        m_state[0] = w;
        m_state[1] = x;
        m_state[2] = y;
    }

    Trio32(uint32_t state[3])
    {
        m_state[0] = state[0];
        m_state[1] = state[1];
        m_state[2] = state[2];
    }

    Trio32(std::array<uint32_t, 3> state) : m_state(state) {}

    Trio32()
    {
        SplitMix32 smix = SplitMix32();
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    void seed(uint32_t s)
    {
        SplitMix32 smix = SplitMix32(s);
        m_state[0] = smix.next();
        m_state[1] = smix.next();
    }

    constexpr std::array<uint32_t, 3> state()
    {
        return m_state;
    }

    uint32_t next()
    {
        uint32_t xp = m_state[0], yp = m_state[1], zp = m_state[2];
        m_state[0] = 15241094284759029579u * zp;
        m_state[1] = yp - xp;
        m_state[1] = ROTL(m_state[1], 12);
        m_state[2] = zp - yp;
        m_state[2] = ROTL(m_state[2], 44);
        return xp;
    }
};

class Mono32
{
private:
    uint32_t m_state;

public:
    Mono32(uint32_t s)
    {
        m_state = (s & 0x1fffffffu) + 1156979152u;
    }
    Mono32()
    {
        Mono32(static_cast<uint32_t>(time(nullptr) >> 32));
    }

    void seed(uint32_t s)
    {
        m_state = (s & 0x1fffffffu) + 1156979152u;
    }

    constexpr uint32_t state()
    {
        return m_state;
    }

    uint16_t next()
    {
        uint16_t result = m_state >> 16;
        m_state *= 3611795771u;
        m_state = ROTL(m_state, 12);
        return result;
    }
};

} // namespace Romu

#endif // __ROMU_RANDOM_HPP__