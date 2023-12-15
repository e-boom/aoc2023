#pragma once
#include <fstream>
#include <Eigen/Core>
#include "config/config.hpp"

namespace AdventOfCode
{
    enum class Ground
    {
        unknown,
        ash,
        rocks,
    };

    struct Symmetry
    {
        int col{-1};
        bool smudgeFound{true};
    };

    inline void printMatrix(const Eigen::MatrixXi &matrix)
    {
        spdlog::info("Matrix: {}x{}", matrix.cols(), matrix.rows());
        std::string header;
        for (int i = 0; i < matrix.cols(); ++i)
            header += std::format("{}", (i + 1) % 10);
        spdlog::info("    {}", header);
        for (int i = 0; i < matrix.rows(); ++i)
        {
            std::string row;
            for (int j = 0; j < matrix.cols(); ++j)
            {
                char c = matrix(i, j) == to_underlying(Ground::ash) ? '.' : '#';
                row += std::format("{}", c);
            }
            spdlog::info("{:2} |{}|", i + 1, row);
        }
    }

    inline Eigen::MatrixXi createMatrix(std::vector<std::string> const &pattern)
    {
        auto const numRows = pattern.size();
        auto const numColumns = pattern.front().size();
        Eigen::MatrixXi matrix(numRows, numColumns);
        std::ranges::for_each(pattern, [&, row = 0](auto const &line) mutable {
            assert(line.size() == numColumns);
            std::ranges::for_each(line, [&, column = 0](auto const &c) mutable {
                auto ground = Ground::unknown;
                switch (c)
                {
                    case '.':
                        ground = Ground::ash;
                        break;
                    case '#':
                        ground = Ground::rocks;
                        break;
                    default:;
                }
                if (ground != Ground::unknown)
                    matrix(row, column) = to_underlying(ground);
                ++column;
            });
            ++row;
        });
        return matrix;
    }

    inline int getSymmetryValue(Eigen::MatrixXi const &matrix, bool const smudgeDefault)
    {
        /// check if diffCount is 1 and smudgeFound is not yet set
        /// otherwise check if diffCount is 0
        auto checkSmudge = [&](Eigen::Index const diffCount, bool &smudgeFound)
        {
            if (diffCount == 1 && !smudgeFound)
            {
                smudgeFound = true;
                return true;
            }
            if (diffCount == 0)
                return true;
            return false;
        };

        /// find the next symmetry line (two columns with no or one difference depending on smudgeFound)
        auto getSymmetryLine = [&](Symmetry symStart, Eigen::MatrixXi const &m) -> Symmetry
        {
            for (int i = symStart.col + 1; i < m.cols() - 1; ++i)
            {
                auto diffs = m.col(i).array() != m.col(i + 1).array();
                if (checkSmudge(diffs.count(), symStart.smudgeFound))
                    return {i, symStart.smudgeFound};
            }
            return {-1, smudgeDefault};
        };

        /// check by comparing the columns left and right of the symmetry line
        auto isMirror = [&](Symmetry &symmetry, Eigen::MatrixXi const &m)
        {
            for (int i = symmetry.col - 1, j = symmetry.col + 2; i >= 0 && j < m.cols(); --i, ++j)
            {
                auto diffs = m.col(i).array() != m.col(j).array();
                if (!checkSmudge(diffs.count(), symmetry.smudgeFound))
                    return false;
            }
            return symmetry.smudgeFound;
        };

        Symmetry symmetry{ -1, smudgeDefault };
        do
        {
            symmetry = getSymmetryLine(symmetry, matrix);
            if (symmetry.col >= 0 && isMirror(symmetry, matrix))
                return symmetry.col + 1;
            // no symmetry yet, reset smudge detection
            symmetry.smudgeFound = smudgeDefault;
        }
        while (symmetry.col >= 0 && symmetry.col < matrix.cols() - 1);

        return -1;
    }

    inline int getSymmetry(Eigen::MatrixXi& matrix, bool const smudgeDefault)
    {
        // check if columns has symmetry
        auto value = getSymmetryValue(matrix, smudgeDefault);
        if (value > 0)
            return value;

        // check if rows has symmetry (transpose matrix)
        matrix.transposeInPlace();
        value = getSymmetryValue(matrix, smudgeDefault);
        if (value > 0)
            return value * 100;

        throw std::runtime_error("No symmetry found");
    }

    inline std::pair<bool, std::vector<std::string>> getPattern(std::ifstream &input)
    {
        std::vector<std::string> pattern;
        for (std::string line; std::getline(input, line);)
        {
            if (!line.empty())
                pattern.push_back(line);
            else
                break;
        }
        return {!pattern.empty(), pattern};
    }

    inline int day13(Part const part)
    {
        spdlog::info(fmt::format("Point of Incidence"));
        constexpr auto fileName = "input_13.txt";
        auto const inputPath(Config::resources / fileName);
        auto input = std::ifstream(inputPath);

        int sum{};
        do
        {
            if (auto [hasPattern, pattern] = getPattern(input); hasPattern)
            {
                auto matrix = createMatrix(pattern);
                // check if pattern is symmetric, in part 2 also check for smudge
                sum += getSymmetry(matrix, part == Part::one);
            }
        }
        while (input.good());
        spdlog::critical("reflection index = {}", sum);

        return part == Part::one ? 
            (sum == 36015 ? EXIT_SUCCESS : EXIT_FAILURE):
            (sum == 35335 ? EXIT_SUCCESS : EXIT_FAILURE)
        ;
    }
} // namespace AdventOfCode
