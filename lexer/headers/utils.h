#if !defined(UTILS_H)
#define UTILS_H
#include <set>
#include <iostream>
#include <chrono>
#include <string>
#include <array>
#include <numeric>
#include <algorithm>
#include "expression.h"
#include <functional>

namespace utils
{

bool isEscapeChar(char x)
{
    return x == '\\';
}
bool shouldConcat(Expression prev)
{
    return prev.type == "OPERAND" || prev.value == "*" || prev.value == "+" || prev.value == "?" || prev.value == ")";
}
bool isBracket(char x)
{

    return (x == ')' || x == '(');
}
bool isOperator(char x)
{
    return (x == '*' || x == '|' || x == '?' || x == '+');
}

bool isOperand(char x)
{
    return !isOperator(x) && !isBracket(x) && x != '\0';
}

int precedence(char op)
{
    switch (op)
    {
    case '*':
    case '?':
    case '+':
        return 4;
    case '.':
        return 3;
    case '|':
        return 2;
    default:
        return 1;
    }
}

template <typename T>
void withTime(T callback, std::string task = "Task")
{
    std::cout << std::endl
              << "\n-----" << task << "-----" << std::endl;
    auto start = std::chrono::system_clock::now();
    callback();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout
        << "\n-----" << task << "-----" << std::endl
        << task << " Completed In: " << diff.count() << " s\n\n";
}

std::string nodeSetToString(std::set<int> set)
{
    if (!set.size())
        return "###";
    std::vector<int> v(set.begin(), set.end());
    return std::accumulate(std::next(v.begin()), v.end(), std::to_string(v[0]), [](std::string acc, int name) {
        return acc + "-" + std::to_string(name);
    });
}

std::function<int(char)> getTransition(std::vector<std::pair<std::string, int>> trans)
{

    return [trans](char alpha) {
        for (auto it = trans.begin(); it != trans.end(); ++it)
        {
            const auto str = (*it).first;
            const int strLength = str.length();
            const auto state = (*it).second;
            if (strLength == 1)
            {
                if (str.at(0) == alpha)
                    return (*it).second;
            }
            else
            {
                std::string group = "";
                std::vector<std::array<int, 2>> ranges;
                for (int i = 0; i < strLength; ++i)
                {
                    if (i + 2 < strLength && str[i + 1] == '-')
                    {
                        if (str[i] > str[i + 2])
                            throw "Invalid Regular Expression Range";
                        ranges.push_back({str[i], str[i + 2]});
                    }
                    else
                    {
                        group += str[i];
                    }
                }

                if (group.find(alpha) != std::string::npos)
                    return state;
                for (auto it = ranges.begin(); it != ranges.end(); ++it)
                {
                    auto range = (*it);
                    if (alpha >= range[0] && alpha <= range[1])
                        return state;
                }
            }
        }
        return -1;
    };
}

} // namespace utils

#endif // UTILS_H