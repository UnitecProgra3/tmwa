#include "read.hpp"
//    io/read_test.cpp - Testsuite for input from files
//
//    Copyright © 2013 Ben Longbons <b.r.longbons@gmail.com>
//
//    This file is part of The Mana World (Athena server)
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include "../strings/zstring.hpp"

#include "../poison.hpp"

static
io::FD string_pipe(ZString sz)
{
    io::FD rfd, wfd;
    if (-1 == io::FD::pipe(rfd, wfd))
        return io::FD();
    if (sz.size() != wfd.write(sz.c_str(), sz.size()))
    {
        rfd.close();
        wfd.close();
        return io::FD();
    }
    wfd.close();
    return rfd;
}

TEST(io, read1)
{
    io::ReadFile rf(string_pipe("Hello"));
    AString hi;
    EXPECT_TRUE(rf.getline(hi));
    EXPECT_EQ(hi, "Hello");
    EXPECT_FALSE(rf.getline(hi));
}
TEST(io, read2)
{
    io::ReadFile rf(string_pipe("Hello\n"));
    AString hi;
    EXPECT_TRUE(rf.getline(hi));
    EXPECT_EQ(hi, "Hello");
    EXPECT_FALSE(rf.getline(hi));
}
TEST(io, read3)
{
    io::ReadFile rf(string_pipe("Hello\r"));
    AString hi;
    EXPECT_TRUE(rf.getline(hi));
    EXPECT_EQ(hi, "Hello");
    EXPECT_FALSE(rf.getline(hi));
}
TEST(io, read4)
{
    io::ReadFile rf(string_pipe("Hello\r\n"));
    AString hi;
    EXPECT_TRUE(rf.getline(hi));
    EXPECT_EQ(hi, "Hello");
    EXPECT_FALSE(rf.getline(hi));
}
TEST(io, read5)
{
    io::ReadFile rf(string_pipe("Hello\n\r"));
    AString hi;
    EXPECT_TRUE(rf.getline(hi));
    EXPECT_EQ(hi, "Hello");
    EXPECT_TRUE(rf.getline(hi));
    EXPECT_FALSE(hi);
    EXPECT_FALSE(rf.getline(hi));
}
