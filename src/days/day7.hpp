#pragma once
#include <fstream>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>
#include "config/config.hpp"

namespace AdventOfCode
{
    inline int day7(Part const part)
    {
        spdlog::info("Camel Cards (part {})", part == Part::one ? 1 : 2);
        static std::unordered_map<char, unsigned> card2Value = {
            {'A', 13},
            {'K', 12},
            {'Q', 11},
            {'J', 10},
            {'T', 9},
            {'9', 8},
            {'8', 7},
            {'7', 6},
            {'6', 5},
            {'5', 4},
            {'4', 3},
            {'3', 2},
            {'2', 1}
        };
        
        if (part == Part::two)
            card2Value['J'] = 0; // jack is joker, but lowest value

        struct Score
        {
            bool fiveOfKind{};
            bool fourOfKind{};
            bool fullHouse{};
            bool threeOfKind{};
            bool twoPairs{};
            bool onePair{};
            bool highCard{};
            std::vector<unsigned> cardValues{};

            bool operator<(Score const &other) const
            {
                if (fiveOfKind != other.fiveOfKind)
                    return fiveOfKind < other.fiveOfKind;
                if (fourOfKind != other.fourOfKind)
                    return fourOfKind < other.fourOfKind;
                if (fullHouse != other.fullHouse)
                    return fullHouse < other.fullHouse;
                if (threeOfKind != other.threeOfKind)
                    return threeOfKind < other.threeOfKind;
                if (twoPairs != other.twoPairs)
                    return twoPairs < other.twoPairs;
                if (onePair != other.onePair)
                    return onePair < other.onePair;
                if (highCard != other.highCard)
                    return highCard < other.highCard;

                // compare card values by index
                return std::ranges::lexicographical_compare(cardValues, other.cardValues);
            }

            void calculate(Part const part)
            {
                std::unordered_map<unsigned, size_t> count;
                std::ranges::for_each(cardValues, [&](auto const value) { ++count[value]; });

                if (part == Part::two)
                {
                    auto const jackValue = card2Value.at('J');
                    auto const jackCount = count.contains(jackValue) ? count.at(jackValue) : 0;
                    // get max element of any card except jack
                    auto const maxCount = std::ranges::max_element(count, [jackValue](auto const &lh, auto const &rh) {
                        if (lh.first == jackValue)
                            return true;
                        if (rh.first == jackValue)
                            return false;
                        return lh.second < rh.second;
                    });
                    // add jack count to max element, and remove jack
                    if (maxCount != count.end() && maxCount->first != jackValue)
                    {
                        maxCount->second += jackCount;
                        count.erase(jackValue);
                    }
                }

                auto anyWithCount = [&count](size_t const number) {
                    return std::ranges::any_of(count, [number](auto const& card) {
                        return card.second == number;
                    });
                };
                
                if ((fiveOfKind = anyWithCount(5)))
                    return;
                if ((fourOfKind = anyWithCount(4)))
                    return;
                if ((fullHouse = anyWithCount(3) && anyWithCount(2)))
                    return;
                if ((threeOfKind = anyWithCount(3)))
                    return;
                if ((twoPairs = std::ranges::count_if(count, [](const auto &card) { return card.second == 2; }) == 2))
                    return;
                if ((onePair = std::ranges::count_if(count, [](const auto &card) { return card.second == 2; }) == 1))
                    return;
                highCard = count.size() == 5;
            }

            [[nodiscard]] std::string what() const
            {
                if (fiveOfKind)
                    return "five of a kind";
                if (fourOfKind)
                    return "four of a kind";
                if (fullHouse)
                    return "full house";
                if (threeOfKind)
                    return "three of a kind";
                if (twoPairs)
                    return "two pairs";
                if (onePair)
                    return "one pair";
                if (highCard)
                    return "high card";
                return "rubbish";
            }
        };

        struct Hand
        {
            std::string cards{};
            Score score{};
            UNumber bid{};

            static Score getScore(std::string const &hand, Part part)
            {
                Score value;
                // add hand to card values
                std::ranges::transform(hand, std::back_inserter(value.cardValues), [](char const card) {
                    return card2Value.at(card);
                });
                value.calculate(part);

                return value;
            }
        };

        //constexpr auto fileName = "demo_7.txt";
        constexpr auto fileName = "input_7.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        std::vector<Hand> hands;
        for (std::string line; std::getline(input, line);)
        {
            boost::regex const pattern{R"(([AKQJT98765432]{5})\s+(\d+))"}; // match 5 cards and a bid
            if (boost::smatch match; boost::regex_search(line, match, pattern))
                hands.emplace_back(match[1], Hand::getScore(match[1], part), toUNumber(match[2]));
        }

        // sort hands on score
        std::ranges::sort(hands, [](auto const &lh, auto const &rh) { return lh.score < rh.score; });

        UNumber totalWinnings{};
        UNumber multiplier = 1;
        for (auto const &[cards, score, bid] : hands)
        {
            spdlog::debug("{:>4} hand: {}, bid: {:>4} : {}", multiplier, cards, bid, score.what());
            totalWinnings += bid * multiplier++;
        }

        spdlog::critical("Total winnings = {}", totalWinnings);
        if (part == Part::one && totalWinnings == 248179786)
            return EXIT_SUCCESS;
        if (part == Part::two && totalWinnings == 247885995)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode