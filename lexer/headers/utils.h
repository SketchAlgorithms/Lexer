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

std::string getEscapeChar(char c)
{
    switch (c)
    {
    case 'n':
        return "\n";
    case 't':
        return "\t";
    case 'r':
        return "\r";
    case 'v':
        return "\v";
    case 'f':
        return "\f";
    case '0':
        return "\n";
    case 's':
        return "\f\n\r\t\v ";
    case 'd':
        return "0-9";
    case 'w':
        return "A-Za-z0-9_";
    default:
        std::string s;
        s.push_back(c);
        return s;
    }
}
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

std::function<int(char)> getTransition(std::vector<std::pair<Expression, int>> trans)
{

    return [trans](char alpha) {
        for (auto it = trans.begin(); it != trans.end(); ++it)
        {
            auto trans = (*it);
            auto exp = trans.first;
            const auto state = trans.second;
            const auto str = exp.value;

            if (exp.sub == "ALL")
            {
                return state;
            }
            const int strLength = str.length();
            if (strLength == 1)
            {
                if (str.at(0) == alpha)
                    return state;
            }
            else
            {
                std::string group = "";
                std::vector<std::array<int, 2>> ranges;
                for (int i = 0; i < strLength; ++i)
                {
                    if (str[i] == '\\')
                    {
                        ++i;
                        group += str[i];
                        continue;
                    }
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

std::pair<std::set<char>, std::vector<std::array<int, 2>>> getRanges(std::string str)
{
    const int strLength = str.length();

    std::set<char> group;
    std::vector<std::array<int, 2>> ranges;
    for (int i = 0; i < strLength; ++i)
    {
        if (str[i] == '\\')
        {
            ++i;
            group.insert(str[i]);
            continue;
        }

        group.insert(str[i]);
        if (i + 2 < strLength && str[i + 1] == '-' && str[i + 2] != ']')
        {
            if (str[i] > str[i + 2])
                throw "Invalid Regular Expression Range";
            ranges.push_back({str[i], str[i + 2]});
            group.insert(str[i + 2]);
            i += 2;
        }
    }
    return std::make_pair(group, ranges);
}

std::function<bool(char)> getRangeCompare(std::pair<std::set<char>, std::vector<std::array<int, 2>>> ranges)
{
    return [ranges](char alpha) {
        if (ranges.first.find(alpha) != ranges.first.end())
            return true;
        for (auto range : ranges.second)
        {
            if (alpha >= range[0] && alpha <= range[1])
                return true;
        }
        return false;
    };
}
std::function<bool(char)> getComparison(Expression exp)
{
    if (exp.sub == "ALL")
    {
        return [](char) { return true; };
    }
    const auto str = exp.value;
    const int strLength = str.length();
    if (strLength == 1)
    {
        return [str](char alpha) { return str.at(0) == alpha; };
    }
    return getRangeCompare(getRanges(str));
}

bool rangeIntersection(std::string range1, std::string range2)
{
    auto pair1 = getRanges(range1);
    auto pair2 = getRanges(range2);
    auto compareChar = getRangeCompare(pair1);
    auto compareChar2 = getRangeCompare(pair2);
    for (auto character : pair2.first)
    {
        if (compareChar(character))
            return true;
    }
    for (auto character : pair1.first)
    {
        if (compareChar2(character))
            return true;
    }

    for (auto rn1 : pair1.second)
    {
        for (auto rn2 : pair2.second)
        {
            if (rn1[0] <= rn2[0] && rn1[1] >= rn2[0] || rn1[0] <= rn2[1] && rn1[1] >= rn2[1] || rn2[0] <= rn1[0] && rn2[1] >= rn1[0] || rn2[0] <= rn1[1] && rn2[1] >= rn1[1]

            )
                return true;
        }
    }
    return false;
}

} // namespace utils

#endif // UTILS_H