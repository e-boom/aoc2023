#pragma once
#include <boost/regex.hpp>
#include <fstream>

namespace AdventOfCode
{
    using LRMap = std::unordered_map<std::string, std::pair<std::string, std::string>>;

    inline std::string const& doInstruction(std::string const& instructions, LRMap const& lrMap, UNumber& steps, std::string const& youAreHere) {
        char const& instruction = instructions[steps++ % instructions.size()];
        if (steps == std::numeric_limits<UNumber>::max())
            throw std::runtime_error("We are lost in the wasteland forever!");

        if (instruction == 'L') 
            return lrMap.at(youAreHere).first;
        return lrMap.at(youAreHere).second;
    }

    inline UNumber findPath(std::string const& instructions, LRMap const& lrMap)
    {
        UNumber noSteps{};
        std::string youAreHere = "AAA";
        while (true)
        {
            youAreHere = doInstruction(instructions, lrMap, noSteps, youAreHere);
            // are we there yet?
            if (youAreHere == "ZZZ")
                break;
        }
        return noSteps;
    }

    inline UNumber findGhostPaths(std::string const& instructions, LRMap const& lrMap)
    {
        // find all starting paths ending in A
        std::vector<std::string> startingPaths;
        std::vector<UNumber> steps;
        for (auto const& key : lrMap | std::views::keys)
        {
            if (key.back() == 'A')
                startingPaths.push_back(key);
        }

        // find the path for each starting path
        std::ranges::for_each(startingPaths, [&](auto& youAreHere) {
            UNumber noSteps{};
            while (true)
            {
                youAreHere = doInstruction(instructions, lrMap, noSteps, youAreHere);
                // are we there yet?
                if (youAreHere.back() == 'Z')
                    break;
            }
            steps.push_back(noSteps);
        });

        // find the least common multiple of all steps to get the total number of steps
        UNumber noSteps = 1;
        std::ranges::for_each(steps, [&](auto const& step) {
            noSteps = std::lcm(noSteps, step);
        });
        return noSteps;
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

            boost::regex const pattern{R"((\S+)\s*=\s*\(\s*(\S+)\s*,\s*(\S+)\s*\))"}; // match map instruction, name and left + right names
            if (boost::smatch match; regex_search(line, match, pattern))
                lrMap.emplace(match[1], std::make_pair(match[2], match[3]));
        }

        UNumber totalSteps{};
        if (part == Part::one)
            totalSteps = findPath(instructions, lrMap);
        else
            totalSteps = findGhostPaths(instructions, lrMap);
        
        spdlog::critical("Number of steps = {}", totalSteps);
        if (part == Part::one && totalSteps == 13939)
            return EXIT_SUCCESS;
        if (part == Part::two && totalSteps == 8906539031197)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode