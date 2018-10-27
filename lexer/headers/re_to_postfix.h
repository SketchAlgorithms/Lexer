
#if !defined(RE_TO_POSTFIX_H)
#define RE_TO_POSTFIX_H
#include <iostream>
#include <vector>
#include "./utils.h"
struct Expression
{
    Expression() {}
    explicit Expression(std::string type,
                        std::string value)
    {
        this->type = type;
        this->value = value;
    }
    explicit Expression(std::string type, char c)
    {
        this->type = type;
        this->value.push_back(c);
    }
    std::string type;
    std::string value;
};

std::vector<Expression> reToPostFix(std::string re)
{
    std::vector<Expression> postfix;
    std::vector<Expression> stack;
    Expression prev;
    for (auto i = re.begin(); i != re.end(); ++i)
    {

        if (utils::isOperand(*i) || utils::isEscapeChar(*i))
        {
            if (utils::isEscapeChar(*i))
            {
                ++i;
            }

            if (prev.type == "OPERAND" || prev.value == "*" || prev.value == ")")
            {
                char c = '.';
                while (!stack.empty() && utils::precedence(c) <= utils::precedence(stack.back().value.at(0)))
                {
                    postfix.push_back(stack.back());
                    stack.pop_back();
                }

                stack.push_back(Expression("OPERATOR", c));
            }
            prev = Expression("OPERAND", *i);
            postfix.push_back(prev);
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
            if (prev.type == "OPERAND" || prev.value == "*" || prev.value == ")")
            {
                char c = '.';
                while (!stack.empty() && utils::precedence(c) <= utils::precedence(stack.back().value.at(0)))
                {
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
    return postfix;
}
#endif // RE_TO_POSTFIX_H