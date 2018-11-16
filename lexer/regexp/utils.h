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
    return prev.type == "OPERAND" || prev.value == "*" || prev.value == "+" || prev.value == "?" || prev.value == ")" || prev.value == "{";
}
bool isBracket(char x)
{

    return (x == ')' || x == '(');
}
bool isOperator(char x)
{
    return (x == '*' || x == '|' || x == '?' || x == '+' || x == '{');
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
    case '{':
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
        << task << " Completed In: " << diff.count() << " s\n";
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

std::function<std::set<int>(char, int)> getTransition(std::vector<std::pair<Expression, int>> trans)
{

    return [trans](char alpha, int current = -1) {
        std::set<int> states = {};

        for (auto pair : trans)
        {
            auto exp = pair.first;
            auto state = pair.second;
            const auto str = exp.value;
            auto accepted = false;
            const int strLength = str.length();
            if (exp.sub == "ALL")
            {
                accepted = true;
            }
            else if (strLength == 1)
            {
                if (str.at(0) == alpha)
                {
                    accepted = true;
                }
            }
            else
            {
                std::set<char> group;
                std::vector<std::array<int, 2>> ranges;
                auto negated = false;
                for (int i = 0; i < strLength; ++i)
                {
                    if (str[i] == '^' && i == 0)
                    {
                        negated = true;
                        continue;
                    }
                    if (str[i] == '\\')
                    {
                        ++i;
                        group.insert(getEscapeChar(str[i]).at(0));
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
                if (negated)
                {
                    auto test = true;
                    if (group.find(alpha) != group.end())
                    {
                        test = false;
                    }
                    else
                    {
                        for (auto range : ranges)
                        {
                            if (alpha >= range[0] && alpha <= range[1])
                            {
                                test = false;
                                break;
                            }
                        }
                    }
                    if (test)
                        accepted = true;
                }
                else
                {
                    if (group.find(alpha) != group.end())
                    {
                        accepted = true;
                    }
                    else
                    {
                        for (auto range : ranges)
                        {
                            if (alpha >= range[0] && alpha <= range[1])
                            {
                                accepted = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (accepted)
            {
                if (exp.rangeType != "")
                {

                    if (exp.rangeType == "END" || exp.rangeType == "BOTH")
                    {
                        exp.range->counter++;
                        // std::cout << std::endl
                        //           << "COUNTER: " << exp.range->counter << " " << alpha << std::endl;
                        if (exp.range->counter < exp.range->lower)
                        {
                            states.insert(current);
                        }
                        else if (!(exp.range->counter > exp.range->upper))
                        {
                            states.insert(state);
                        }
                        else
                        {
                            exp.range->counter = 0;
                        }
                    }
                }
                else
                {

                    states.insert(state);
                }
            }
            else
            {
                if (exp.rangeType == "START" || exp.rangeType == "BOTH")
                {
                    // if (current != state)
                    exp.range->counter = 0;
                }
            }
        }

        return states;
    };
}

} // namespace utils

#endif // UTILS_H