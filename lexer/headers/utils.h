#if !defined(UTILS_H)
#define UTILS_H
#include <set>
#include <iostream>
#include <chrono>
#include <string>
#include <numeric>
#include <algorithm>
namespace utils
{

bool isEscapeChar(char x)
{
    return x == '\\';
}

bool isBracket(char x)
{

    return (x == ')' || x == '(');
}
bool isOperator(char x)
{
    return (x == '.' || x == '*' || x == '|');
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
    std::cout << "\n-----" << task << "-----" << std::endl;
    auto start = std::chrono::system_clock::now();
    callback();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << task << " Completed In: " << diff.count() << " s\n";
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

// std::string intSetToNodeSet


} // namespace utils

#endif // UTILS_H