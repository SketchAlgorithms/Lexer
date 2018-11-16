
#if !defined(TOKEN_H)
#define TOKEN_H
#include <iostream>

namespace lex
{

struct Token
{
    explicit Token()
    {
        this->column = 0;
        this->value = "";
        this->line = 0;
        this->type = "";
        this->extra = "";
    }
    explicit Token(std::string type, std::string value = "", long line = 0, long column = 0, std::string subType = "", std::string extra = "")
    {
        this->column = column;
        this->value = value;
        this->line = line;
        this->type = type;
        this->extra = extra;
        this->subType = subType;
    }
    std::string value;
    std::string extra;
    std::string subType;
    long line;
    long column;
    std::string type;
};
} // namespace lex

#endif // TOKEN_H
