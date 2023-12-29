#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>
#include "common.hpp"
#include "config/config.hpp"
#include "days/day1.hpp"
#include "days/day13.hpp"
#include "days/day2.hpp"
#include "days/day3.hpp"
#include "days/day4.hpp"
#include "days/day5.hpp"
#include "days/day6.hpp"
#include "days/day7.hpp"
#include "days/day8.hpp"

int main(int const argc, char **argv)
{
    auto returnCode = EXIT_FAILURE;
    spdlog::info("Welcome to AdventOfCode 2023");
    try
    {
        std::unordered_map<unsigned, std::function<int(AdventOfCode::Part)>> const dayFunctions = {
            {  1, AdventOfCode::day1},
            {  2, AdventOfCode::day2},
            {  3, AdventOfCode::day3},
            {  4, AdventOfCode::day4},
            {  5, AdventOfCode::day5},
            {  6, AdventOfCode::day6},
            {  7, AdventOfCode::day7},
            {  8, AdventOfCode::day8},
            { 13, AdventOfCode::day13},
        };
        
        constexpr std::array<std::string_view, 17> choices{
            "1a", "1b",
            "2a", "2b",
            "13a", "13b",
            "3a", "3b",
            "4a", "4b",
            "5a", "5b",
            "6a", "6b",
            "7a" , "7b",
            "8a" //, "8b"
        };
        constexpr auto defaultChoice = choices.back();

        argparse::ArgumentParser program(std::string(AdventOfCode::Config::project_name),
                                         fmt::format("version {}", AdventOfCode::Config::project_version),
                                         argparse::default_arguments::all);
        program.add_argument("-d", "--day")
            .help("number of the day of advent")
            .default_value(std::string{defaultChoice})
            .action(
                [](const std::string &value)
                {
                    if (std::ranges::find(choices, value) != choices.end())
                        return value;
                    return std::string{defaultChoice};
                });
        program.add_argument("-l", "--list")
            .help("list all implemented days of advent")
            .default_value(false)
            .implicit_value(true);
        program.add_argument("-v", "--verbose")
            .help("display extra debug info")
            .default_value(false)
            .implicit_value(true);

        program.parse_args(argc, argv);
        if (program.get<bool>("list"))
        {
            spdlog::info("Scratched days of advent:");
            std::ranges::for_each(choices, [](auto const &choice) { spdlog::info(" {}", choice); });
            return EXIT_SUCCESS;
        }

        if (program.get<bool>("verbose"))
            spdlog::set_level(spdlog::level::debug);
        
        auto const day = program.get<std::string>("day");
        spdlog::info("Day {}", day);
        if (auto const dayNr = std::stoi(day); dayFunctions.contains(dayNr))
        {
            auto const part = day.back() == 'a' ? AdventOfCode::Part::one : AdventOfCode::Part::two;
            returnCode = dayFunctions.at(dayNr)(part);
        }
        else
            throw std::runtime_error("We ran out of days!");
    }
    catch (const std::exception &e)
    {
        spdlog::error("Unfortunate exception: {}", e.what());
    }
    if (returnCode == EXIT_SUCCESS)
        spdlog::info("Ho ho ho!!! Merry Xmas everybody!");
    return returnCode;
}
