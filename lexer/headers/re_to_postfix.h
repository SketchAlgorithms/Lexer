
#if !defined(RE_TO_POSTFIX_H)
#define RE_TO_POSTFIX_H
#include <iostream>
#include <vector>
#include "./utils.h"
#include "expression.h"

std::string normalize(std::string str)
{
    const int strLength = str.length();
    std::string normalizedString = "";
    for (int i = 0; i < strLength; ++i)
    {
        std::string temp;
        if (str[i] == '[')
        {
            ++i;
            std::string value = "(";
            while (str[i] != ']' && i < strLength)
            {
                if (i + 2 < strLength && str[i + 1] == '-' && str[i + 2] != ']')
                {
                    if (str[i] > str[i + 2])
                        throw "Invalid Regular Expression Range";
                    value += "(\\";
                    value.push_back(str[i]);
                    value.push_back('|');
                    for (char j = str[i] + 1; j <= str[i + 2]; ++j)
                    {
                        value.push_back('\\');
                        value.push_back(j);
                        if (j != str[i + 2])
                            value += "|";
                    }
                    value += ")";
                    if (i + 3 < strLength && str[i + 3] != ']')
                        value.push_back('|');
                    i += 2;
                }
                else
                {

                    value.push_back('\\');
                    value.push_back(str[i]);
                    if (i + 1 < strLength && str[i + 1] != ']')
                        value += "|";
                }
                i++;
            }
            if (i >= strLength)
                throw "Invalid RE";
            temp += value + ")";
        }

        else
        {
            temp += str[i];
        }

        normalizedString += temp;
    }

    return normalizedString;
}

void prefixPrinter(std::vector<Expression> pre)
{
    std::cout << std::endl
              << "============" << std::endl;
    for (auto v : pre)
    {
        if (v.sub == "NULL")
            std::cout << " EPS ";
        else
            std::cout << " " << v.value << " ";
    }
    std::cout << std::endl
              << "============" << std::endl;
}

std::vector<Expression> reToPostFix(std::string re, int print = 0)
{
    std::vector<Expression> postfix;
    std::vector<Expression> stack;
    re = normalize(re);
    if (print)
        std::cout << re << std::endl;
    Expression prev;
    for (auto i = re.begin(); i != re.end(); ++i)
    {

        if (utils::isOperand(*i) || utils::isEscapeChar(*i) || *i == '[')
        {
            if (utils::shouldConcat(prev))
            {
                char c = '.';
                while (!stack.empty() && utils::precedence(c) <= utils::precedence(stack.back().value.at(0)))
                {
                    postfix.push_back(stack.back());
                    stack.pop_back();
                }

                stack.push_back(Expression("OPERATOR", c));
            }
            if (*i == '.')
            {
                prev = Expression("OPERAND", *i, "SPECIAL");
                postfix.push_back(prev);
            }
            else if (*i == '[')
            {
                std::string value = "";
                i++;
                while (*i != ']' && i != re.end())
                {
                    value += *i;
                    i++;
                }
                if (i == re.end())
                    throw "Invalid RE";
                prev = Expression("OPERAND", value);
                postfix.push_back(prev);
            }
            else
            {
                if (utils::isEscapeChar(*i))
                {
                    ++i;
                }
                prev = Expression("OPERAND", *i);
                postfix.push_back(prev);
            }
        }
        else if (utils::isOperator(*i))
        {
            while (!stack.empty() && utils::precedence(*i) <= utils::precedence(stack.back().value.at(0)))
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            prev = Expression("OPERATOR", *i);
            stack.push_back(prev);
        }
        else if (*i == '(')
        {
            if (utils::shouldConcat(prev))
            {
                char c = '.';
                while (!stack.empty() && utils::precedence(c) <= utils::precedence(stack.back().value.at(0)))
                {
                    postfix.push_back(stack.back());
                    stack.pop_back();
                }

                stack.push_back(Expression("OPERATOR", c));
            }
            prev = Expression("BRACKET", *i);
            stack.push_back(prev);
        }
        else
        {

            while (!stack.empty() && stack.back().value.at(0) != '(')
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            if (stack.back().value.at(0) == '(')
                stack.pop_back();
            prev = Expression("BRACKET", *i);
        }
    }
    while (!stack.empty())
    {
        postfix.push_back(stack.back());
        stack.pop_back();
    }
    if (print)
        prefixPrinter(postfix);

    return postfix;
}
#endif // RE_TO_POSTFIX_H