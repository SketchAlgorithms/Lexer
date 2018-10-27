
#if !defined(RE_TO_POSTFIX_H)
#define RE_TO_POSTFIX_H
#include <iostream>
#include <vector>
struct Expression
{
    explicit Expression(std::string type,
                        std::string value)
    {
        this->type = type;
        this->value = value;
    }
    std::string type;
    std::string value;
};

std::string reToPostFix(std::string re)
{
    std::vector<Expression> postfix;
    std::vector<Expression> stack;
    for (auto i = re.begin(); i != re.end(); ++i)
    {
        
    }
}
#endif // RE_TO_POSTFIX_H