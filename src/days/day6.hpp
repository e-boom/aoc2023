#pragma once
#include <boost/regex.hpp>
#include <fstream>
#include "config/config.hpp"

namespace AdventOfCode
{
    inline UNumber race(UNumber const time, UNumber const distance)
    {
        UNumber recordBreakers = 0;
        for (size_t t = 1; t < time; ++t)
        {
            if (distance < (time - t) * t)
                ++recordBreakers;
        }
        return recordBreakers;
    }

    inline int day6(Part const part)
    {
        spdlog::info(fmt::format("Wait For It"));

        //constexpr auto fileName = "demo_6.txt";
        constexpr auto fileName = "input_6.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        std::vector<UNumber> times;
        std::vector<UNumber> distances;
        for (std::string line; std::getline(input, line);)
        {
            boost::regex pattern(R"(\s*(.+?)\s*:\s*(.*)\s*)"); // label, ':' and numbers
            boost::smatch match;
            if (boost::regex_search(line, match, pattern))
            {
                auto const& label = match[1];
                std::string numbers = match[2];
                if (part == Part::two)
                    std::erase(numbers, ' ');
                if (label == "Time")
                    times = getNumbers(numbers);
                else if (label == "Distance")
                    distances = getNumbers(numbers);
            } 
        }

        assert(times.size() == distances.size());
        UNumber multiplier = 1;
        for (size_t i = 0; i < times.size(); ++i)
            multiplier *= race(times[i], distances[i]);

        spdlog::critical("Multiplied record breakers = {}", multiplier);
        if (part == Part::one && multiplier == 170000)
            return EXIT_SUCCESS;
        if (part == Part::two && multiplier == 20537782)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode