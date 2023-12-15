#pragma once
#include <fstream>
#include <regex>

#include <boost/regex.hpp>
#include <boost/algorithm/string/join.hpp>

#include "../common.hpp"

namespace AdventOfCode
{
    inline std::unordered_map<std::string, std::string> words = {
        {"one", "1"},   {"two", "2"},   {"three", "3"},
        {"four", "4"},  {"five", "5"},  {"six", "6"},
        {"seven", "7"}, {"eight", "8"}, {"nine", "9"}
    };

    enum class Directions
    {
        forward,
        reverse
    };
    
    inline std::string getDigit(std::string const &line, boost::regex const &findDigitRegex,
                                Directions const direction = Directions::forward)
    {
        std::string digit;
        if (boost::smatch what; regex_search(line, what, findDigitRegex))
            digit = what[1];
        if (digit.size() > 1)
        {
            if (direction == Directions::reverse)
                std::ranges::reverse(digit);
            digit = words[digit];
        }
        return digit;
    }

    inline int day1(Part part)
    {
        spdlog::info(fmt::format("Trebuchet?! part {}", part == Part::one ? 1 : 2));

        constexpr auto fileName = "input_1.txt";
        auto const inputPath(Config::resources / fileName);

        // add all keys of words to vector
        auto keys = std::vector<std::string>{};
        std::ranges::transform(words, std::back_inserter(keys), [](auto const &pair) { return pair.first; });
        auto wordsPattern = boost::algorithm::join(keys, "|");
        boost::regex findDigitRegex;
        boost::regex findReverseDigitRegex;
        if (part == Part::one)
        {
            findDigitRegex = boost::regex("(\\d)");
            findReverseDigitRegex = findDigitRegex;
        }
        else
        {
            findDigitRegex = boost::regex(fmt::format("({}|\\d)", wordsPattern));
            std::ranges::reverse(wordsPattern);
            findReverseDigitRegex = boost::regex(fmt::format("({}|\\d)", wordsPattern));
        }

        auto input = std::ifstream(inputPath);
        int sum{};
        for (std::string line; std::getline(input, line);)
        {
            auto firstDigit = getDigit(line, findDigitRegex);
            // reverse line and search for last digit
            std::ranges::reverse(line);
            auto lastDigit = getDigit(line, findReverseDigitRegex, Directions::reverse);
            sum += std::stoi(firstDigit + lastDigit);
        }
        spdlog::critical("calibration result = {}", sum);
        if (part == Part::one && sum == 55607 || part == Part::two && sum == 55291)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode