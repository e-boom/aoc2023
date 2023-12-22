#pragma once
#include <boost/regex.hpp>
#include <fstream>

#include "config/config.hpp"

namespace AdventOfCode
{
    inline int day4(Part const part)
    {
        spdlog::info(fmt::format("Scratchcards"));

        //constexpr auto fileName = "demo_4.txt";
        constexpr auto fileName = "input_4.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        struct Card
        {
            UNumber nr;
            size_t numberOfWinningNumbers;
            size_t numberOfCards{1};
        };

        std::vector<Card> cards;
        for (std::string line; std::getline(input, line);)
        {
            boost::regex pattern(R"(Card\s*(\d+):(.*)\|(.*))"); // match card number, winning numbers and actual card numbers
            boost::smatch match;
            if (boost::regex_search(line, match, pattern))
            {
                auto winning = getNumbers(match[2]);
                auto numbers = getNumbers(match[3]);

                std::vector<UNumber> intersection;
                std::ranges::sort(winning);
                std::ranges::sort(numbers);
                std::ranges::set_intersection(winning, numbers, std::back_inserter(intersection));
                cards.emplace_back(std::stoull(match[1]), intersection.size());
            }
        }

        auto sum = std::accumulate(cards.begin(), cards.end(), 0ull, [](auto const &acc, auto const &card) {
            auto score = card.numberOfWinningNumbers > 0 ? 1 << (card.numberOfWinningNumbers - 1) : 0;
            return acc + score;
        });
        spdlog::critical("Cards winning sum = {}", sum);
        if (part == Part::one && sum == 22488)
            return EXIT_SUCCESS;

        // part 2: play the game
        for (size_t i = 0; i < cards.size(); ++i) {
            for (size_t j = 1; j <= cards[i].numberOfWinningNumbers; ++j) {
                assert(i + j < cards.size());
                if (i + j < cards.size()) {
                    cards[i + j].numberOfCards += cards[i].numberOfCards;
                }
            }
        }

        auto totalNoCards = std::accumulate(cards.begin(), cards.end(), 0ull, [](auto const &acc, auto const &card) {
            return acc + card.numberOfCards;
        });
        spdlog::critical("Total number of cards = {}", totalNoCards);
        if (part == Part::two && totalNoCards == 7013204)
            return EXIT_SUCCESS;

        return EXIT_FAILURE;
    }
} // namespace AdventOfCode