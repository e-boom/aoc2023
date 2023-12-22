#pragma once
#include <boost/regex.hpp>

template <typename E>
constexpr std::underlying_type_t<E> to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

namespace AdventOfCode
{
    using UNumber = unsigned long long;
    
    enum class Part
    {
        one,
        two
    };

    /// Convert the string with numbers as a vector of unsigned longs
    inline auto getNumbers(std::string const &numberString)
    {
        std::vector<UNumber> numbers;
        boost::regex const numberPattern(R"((\d+))");
        boost::sregex_iterator it(numberString.begin(), numberString.end(), numberPattern);
        boost::sregex_iterator const end;
        while (it != end)
        {
            numbers.push_back(std::stoull(it->str(1)));
            ++it;
        }
        return numbers;
    }
}