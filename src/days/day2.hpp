#pragma once
#include <boost/regex.hpp>
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
    inline int day2(Part part)
    {
        spdlog::info(fmt::format("Cube Conundrum"));

        // constexpr auto fileName = "demo_2.txt";
        constexpr auto fileName = "input_2.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        constexpr unsigned maxBlue = 14;
        constexpr unsigned maxGreen = 13;
        constexpr unsigned maxRed = 12;

        struct Game
        {
            unsigned long nr;
            bool isPossible{true};
            unsigned long maxRed{};
            unsigned long maxGreen{};
            unsigned long maxBlue{};
        };

        std::vector<Game> games;
        for (std::string line; std::getline(input, line);)
        {
            boost::regex pattern(R"(Game\s*(\d+)\s*:(.*))"); // match game number and color amounts
            boost::smatch match;
            if (boost::regex_search(line, match, pattern))
            {
                Game game{std::stoul(match[1])};

                // match amount, color and separator
                std::string colorAmounts = match[2];
                boost::regex colorPattern(R"(\s*(\d+)\s*(green|blue|red)\s*(,|;*))");
                boost::sregex_iterator it(colorAmounts.begin(), colorAmounts.end(), colorPattern);
                boost::sregex_iterator end;
                while (it != end)
                {
                    bool isHandPossible{true};
                    while (it != end)
                    {
                        auto amount = std::stoul(it->str(1));
                        if (it->str(2) == "red")
                        {
                            isHandPossible &= amount <= maxRed;
                            game.maxRed = std::max(game.maxRed, amount);
                        }
                        else if (it->str(2) == "green")
                        {
                            isHandPossible &= amount <= maxGreen;
                            game.maxGreen = std::max(game.maxGreen, amount);
                        }
                        else if (it->str(2) == "blue")
                        {
                            isHandPossible &= amount <= maxBlue;
                            game.maxBlue = std::max(game.maxBlue, amount);
                        }
                        if (it->str(3) != ",")
                            break;
                        ++it;
                    }
                    game.isPossible &= isHandPossible;
                    ++it;
                }
                games.push_back(game);
            }
        }
        auto sum = std::accumulate(games.begin(), games.end(), 0, [](auto const &acc, auto const &game) {
            return acc + (game.isPossible ? game.nr : 0);
        });
        auto power = std::accumulate(games.begin(), games.end(), 0, [](auto const &acc, auto const &game) {
            return acc + (game.maxRed * game.maxGreen * game.maxBlue);
        });
        spdlog::critical("Game possibility sum = {}", sum);
        spdlog::critical("Game power = {}", power);
        if (part == Part::one && sum == 2439 || part == Part::two && power == 2286)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode