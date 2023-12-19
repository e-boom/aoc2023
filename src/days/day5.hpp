#pragma once
#include <boost/regex.hpp>
#include <fstream>
#include <ranges>
#include <algorithm>
#include "config/config.hpp"

namespace AdventOfCode
{
    inline int day5(Part const part)
    {
        spdlog::info(fmt::format("If You Give A Seed A Fertilizer"));

        //constexpr auto fileName = "demo_5.txt";
        constexpr auto fileName = "input_5.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        struct AlmanacItem
        {
            unsigned long destination{};
            unsigned long source{};
            unsigned long range{};

            [[nodiscard]] bool inRange(UNumber const value) const
            {
                return source <= value && 
                    source + range > value;
            }
            [[nodiscard]] UNumber getValue(UNumber const value) const
            {
                assert(inRange(value));
                return destination + (value - source);
            }
            [[nodiscard]] static AlmanacItem read(std::string const &line)
            {
                auto const numbers = getNumbers(line);
                if (numbers.size() != 3)
                    throw std::logic_error("Almanac item is damaged");
                return {numbers[0], numbers[1], numbers[2]};
            }
        };

        struct AlmanacMap
        {
            std::string name;
            [[nodiscard]] UNumber getValue(UNumber sourceIn)
            {
                // find destination in items within range
                auto const destinationOut = std::ranges::find_if(items, [sourceIn](auto const &item) {
                    return item.inRange(sourceIn);
                });
                if (destinationOut != end(items))
                    return destinationOut->getValue(sourceIn);
                return sourceIn;
            }

            std::vector<AlmanacItem> items;
            void read(std::string const &line)
            {
                if (!line.empty())
                    items.push_back(AlmanacItem::read(line));
            }
        };

        struct Almanac
        {
            AlmanacMap seed2soil;
            AlmanacMap soil2fertilizer;
            AlmanacMap fertilizer2water;
            AlmanacMap water2light;
            AlmanacMap light2temperature;
            AlmanacMap temperature2humidity;
            AlmanacMap humidity2location;

            [[nodiscard]] UNumber getSeedLocation(UNumber const seedNr)
            {
                return humidity2location.getValue(
                    temperature2humidity.getValue(
                        light2temperature.getValue(
                            water2light.getValue(
                                fertilizer2water.getValue(
                                    soil2fertilizer.getValue(
                                        seed2soil.getValue(seedNr)
                                    )
                                )
                            )
                        )
                    )
                );
            }
        };

        std::vector<unsigned long> seeds;
        Almanac almanac;
        AlmanacMap *currentMap{};
        for (std::string line; std::getline(input, line);)
        {
            boost::regex pattern(R"(\s*(.+?)\s*:\s*(.*)\s*)"); // label, ':' en numbers (if any)
            boost::smatch match;
            if (boost::regex_search(line, match, pattern))
            {
                auto const &label = match[1];
                std::string labelStr = label.str();
                if (label == "seeds")
                {
                    if (part == Part::one)
                        seeds = getNumbers(match[2]);
                    else
                    {
                        // seeds are given as ranges (number and range)
                        auto seedRange = getNumbers(match[2]);
                        for (auto i = seedRange.begin(); i != seedRange.end(); i += 2)
                        {
                            for (UNumber j = 0; j < *(i + 1); ++j)
                                seeds.push_back(*i + j);
                        }
                    }
                }
                else if (label == "seed-to-soil map")
                    currentMap = &almanac.seed2soil;
                else if (label == "soil-to-fertilizer map")
                    currentMap = &almanac.soil2fertilizer;
                else if (label == "fertilizer-to-water map")
                    currentMap = &almanac.fertilizer2water;
                else if (label == "water-to-light map")
                    currentMap = &almanac.water2light;
                else if (label == "light-to-temperature map")
                    currentMap = &almanac.light2temperature;
                else if (label == "temperature-to-humidity map")
                    currentMap = &almanac.temperature2humidity;
                else if (label == "humidity-to-location map")
                    currentMap = &almanac.humidity2location;
                if (currentMap)
                    currentMap->name = label;
            }
            else if (currentMap && !line.empty())
                currentMap->items.push_back(AlmanacItem::read(line));
        }

        // get minimum location of the seeds
        std::vector<UNumber> locations;
        std::ranges::transform(seeds, std::back_inserter(locations), [&almanac](auto seedNr) {
            return almanac.getSeedLocation(seedNr);
        });
        auto minimumLocation = *std::ranges::min_element(locations);
        spdlog::critical("Closest seeding location = {}", minimumLocation);
        if (part == Part::one && minimumLocation == 157211394)
            return EXIT_SUCCESS;
        if (part == Part::two && minimumLocation == 50855035)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
} // namespace AdventOfCode