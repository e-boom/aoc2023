#pragma once
#include <boost/regex.hpp>
#include <fstream>
#include <spdlog/spdlog.h>
#include "config/config.hpp"
#include "common.hpp"

namespace AdventOfCode
{
    using LRMap = std::unordered_map<std::string, std::pair<std::string, std::string>>;
    inline UNumber findPath(std::string const& instructions, LRMap const& lrMap)
    {
        auto const noInstructions = instructions.size();
        UNumber steps{};
        std::string youAreHere = "AAA";
        while (true)
        {
            char const& instruction = instructions[steps++ % noInstructions];
            if (instruction == 'L')
                youAreHere = lrMap.at(youAreHere).first;
            else
                youAreHere = lrMap.at(youAreHere).second;
            spdlog::debug("{:3>} Instruction = {}, location = {}", steps, instruction, youAreHere);
             
            if (youAreHere == "ZZZ")
                break;
            if (steps == std::numeric_limits<UNumber>::max())
                throw std::runtime_error("We are lost in the wasteland forever!");
        }
        return steps;
    }

    inline int day8(Part const part)
    {
        spdlog::info("Haunted Wasteland (part {})", part == Part::one ? 1 : 2);

        //constexpr auto fileName = "demo_8.txt";
        constexpr auto fileName = "input_8.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        std::string instructions;
        LRMap lrMap;
        for (std::string line; std::getline(input, line);)
        {
            if (line.empty())
                continue;
            if (instructions.empty())
                instructions = line;

            boost::regex const pattern{R"(([A-Z]{3})\s*=\s*\(\s*([A-Z]{3})\s*,\s*([A-Z]{3})\s*\))"}; // match map instruction, name and left + right names
            if (boost::smatch match; regex_search(line, match, pattern))
                lrMap.emplace(match[1], std::make_pair(match[2], match[3]));
        }

        UNumber totalSteps = findPath(instructions, lrMap);
        spdlog::critical("Number of steps = {}", totalSteps);
        if (part == Part::one && totalSteps == 13939)
            return EXIT_SUCCESS;
        if (part == Part::two && false)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode