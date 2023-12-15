#pragma once
#include <fstream>
#include "config/config.hpp"

// testcases
// 12 red cubes, 13 green cubes, and 14 blue cubes?
// games 1, 2, and 5 would have been possible if the bag had been loaded with that configuration. However,
// game 3 would have been impossible because at one point the Elf showed you 20 red cubes at once; similarly,
// game 4 would also have been impossible because the Elf showed you 15 blue cubes at once.
// If you add up the IDs of the games that would have been possible, you get 8.

// solve:
// 12 red cubes, 13 green cubes, and 14 blue cubes. What is the sum of the IDs of those games?
namespace AdventOfCode
{
    inline int day2()
    {
        spdlog::info(fmt::format("Cube Conundrum"));

        constexpr auto fileName = "demo_2.txt";
        //constexpr auto fileName = "input_2.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        for (std::string line; std::getline(input, line);)
        {
            spdlog::info(line);
        }
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode