
#if !defined(TOKEN_H)
#define TOKEN_H
#include <iostream>

namespace lex
{

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    BOOLEAN,
    NUMBER,
    STRING,
    REGEXP,
    OPERATOR,
    DELIMETER,
    COMMENT,
    UNDEF,
    EOT
} TokenType;
struct Token
{
    explicit Token()
    {
        this->column = 0;
        this->value = "";
        this->line = 0;
        this->type = TokenType::EOT;
        this->extra = "";
    }
    explicit Token(TokenType type, std::string value = "", long line = 0, long column = 0, std::string extra = "")
    {
        this->column = column;
        this->value = value;
        this->line = line;
        this->type = type;
        this->extra = extra;
    }
    TokenType type;
    std::string value;
    std::string extra;
    long line;
    long column;
};

} // namespace lex

#endif // TOKEN_H
