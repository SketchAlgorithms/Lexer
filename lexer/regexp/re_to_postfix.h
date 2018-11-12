
#if !defined(RE_TO_POSTFIX_H)
#define RE_TO_POSTFIX_H
#include <iostream>
#include <vector>
#include "./utils.h"
#include "expression.h"

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
    if (print)
        std::cout << re << std::endl;
    Expression prev;
    for (auto i = re.begin(); i != re.end(); ++i)
    {

        if (utils::isOperand(*i) || *i == '\\' || *i == '[')
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
                prev = Expression("OPERAND", *i, "ALL");
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
            else if (*i == '\\')
            {
                ++i;
                prev = Expression("OPERAND", utils::getEscapeChar(*i));
                postfix.push_back(prev);
            }
            else
            {
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