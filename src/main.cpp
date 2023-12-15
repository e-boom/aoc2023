#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>
#include "common.hpp"
#include "config/config.hpp"
#include "days/day1.hpp"
#include "days/day13.hpp"
#include "days/day2.hpp"

int main(int const argc, char **argv)
{
    auto returnCode = EXIT_FAILURE;
    spdlog::info("Welcome to AdventOfCode 2023");
    try
    {
        constexpr std::array<std::string_view, 5> choices{
            "1a", "1b", "2", "13a", "13b",
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

        program.parse_args(argc, argv);
        if (program.get<bool>("list"))
        {
            spdlog::info("Scratched days of advent:");
            std::ranges::for_each(choices, [](auto const &choice) { spdlog::info(" {}", choice); });
            return EXIT_SUCCESS;
        }

        auto const day = program.get<std::string>("day");
        spdlog::info("Day {}", day);
        if (day == choices[0])
            returnCode = day1(AdventOfCode::Part::one);
        else if (day == choices[1])
            returnCode = day1(AdventOfCode::Part::two);
        else if (day == choices[2])
            returnCode = AdventOfCode::day2();
        else if (day == choices[3])
            returnCode = day13(AdventOfCode::Part::one);
        else if (day == choices[4])
            returnCode = day13(AdventOfCode::Part::two);
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