#pragma once
#include <boost/regex.hpp>
#include <fstream>

#include "config/config.hpp"

// The engine schematic (your puzzle input) consists of a visual 
// representation of the engine. There are lots of numbers and symbols you 
// don't really understand, but apparently any number adjacent to a symbol, 
// even diagonally, is a "part number" and should be included in your sum. 
// (Periods (.) do not count as a symbol.)

namespace AdventOfCode
{
    struct Box // bounding box
    {
        int64_t left{}, top{}, right{}, bottom{};

        bool overlaps(Box const& other) const
        {
            return !(left > other.right || right < other.left || top > other.bottom || bottom < other.top);
        }
    };
    
    inline int day3(Part const part)
    {
        spdlog::info(fmt::format("Gear Ratios"));

        //constexpr auto fileName = "demo_3.txt";
        constexpr auto fileName = "input_3.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        std::vector<std::pair<unsigned long, Box>> parts; // part number and bounding box
        std::vector<std::pair<bool, Box>> symbols; // is ratio gear and bounding box

        unsigned long row = 0;
        for (std::string line; std::getline(input, line);)
        {
            // matching groups: part number (digits), symbol (not '.' nor digits) or empty spaces ('.')
            boost::regex pattern(R"((\d+)|([^\.\d])|(\.+))");
            boost::sregex_iterator it(line.begin(), line.end(), pattern);
            boost::sregex_iterator end;
            while (it != end) {
                const auto& match = *it;
                auto const& matchStr = match.str();
                if (std::isdigit(matchStr[0]))
                {
                    auto partNr = std::stoul(matchStr);
                    parts.emplace_back(
                        partNr, 
                        Box{
                            match.position(), row,
                            match.position() + match.length() - 1, row
                        }
                    );
                }
                else if (matchStr[0] != '.')
                    assert(match.length() == 1);
                    symbols.emplace_back(
                        matchStr[0] == '*',
                        Box{
                            match.position() - 1, row - 1,
                            match.position() + 1 + match.length() - 1, row + 1
                        }
                    );
                ++it;
            }
            ++row;
        }

        // sum all part numbers that are adjacent to a symbol
        auto sum = std::accumulate(parts.begin(), parts.end(), 0UL, [&symbols](auto sum, auto const& part) {
            auto const& [partNr, box] = part;
            auto const isAdjacent = std::any_of(symbols.begin(), symbols.end(), [&box](auto const& symbol) {
                auto const& [hasRatio, gearBox] = symbol;
                return gearBox.overlaps(box);
            });
            return sum + (isAdjacent ? partNr : 0);
        });

        // sum all gear ratios that have at least two adjacent part numbers
        auto ratioSum = std::accumulate(symbols.begin(), symbols.end(), 0UL, [&parts](auto sum, auto const& symbol) {
            auto const& [hasRatio, gearBox] = symbol;
            if (hasRatio)
            {
                // find all part numbers that overlap with the gear box
                std::vector<unsigned long> gearParts;
                std::for_each(parts.begin(), parts.end(), [&gearBox, &gearParts](auto const& part) {
                    auto const& [partNr, box] = part;
                    if (box.overlaps(gearBox))
                        gearParts.push_back(partNr);
                });
                if (gearParts.size() >= 2)
                {
                    unsigned long ratio = 1;
                    std::for_each(gearParts.begin(), gearParts.end(), [&ratio](auto const& partNr) {
                        ratio *= partNr;
                    });
                    return sum + ratio;
                }
            }
            return sum;
        });
        spdlog::critical("Partnumber sum = {}", sum);
        spdlog::critical("Gear ratio sum = {}", ratioSum);
        if (part == Part::one && sum == 546312 || 
            part == Part::two && ratioSum == 87449461)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode