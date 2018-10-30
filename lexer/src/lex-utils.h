

#if !defined(LEXER_UTILS_H)
#define LEXER_UTILS_H
#include <set>
#include <algorithm>
#include <string>
#include "../headers/regexp.h"
#include "token.h"
namespace lex
{
std::string tokenString[11] =
    {
        "KEYWORD",
        "IDENTIFIER",
        "BOOLEAN",
        "NUMBER",
        "STRING",
        "REGEXP",
        "OPERATOR",
        "DELIMETER",
        "COMMENT",
        "UNDEF",
        "EOT"};

bool isEscapeSequence(char a)
{
    return a == '\\';
}
bool isNumber(char a)
{
    return (a >= '0' && a <= '9');
}

bool isAlphabet(char a)
{
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_' || a == '$';
}

bool isWhiteSpace(char a)
{
    return a == ' ' || a == '\t' || a == '\n' || a == '\b' || a == '\r' || a == '\v';
}

bool isDelimeter(char a)
{
    return a == '{' || a == '}' || a == '[' || a == ']' || a == '(' || a == ')' || a == ',' || a == ';' || a == ':' || a == '#' || a == '.';
}
bool isBracket(char a)
{
    return a == '{' || a == '}' || a == '[' || a == ']' || a == '(' || a == ')';
}
bool isClosingBracket(char a)
{
    return a == '}' || a == ']' || a == ')';
}
bool isOperator(char a)
{
    return a == '+' || a == '-' || a == '*' || a == '/' || a == '^' || a == '%' || a == '!' || a == '&' || a == '?' || a == '|' || a == '~' || a == '>' || a == '=' || a == '<' || a == ':';
}
bool isStringOrChar(char a)
{
    return a == '\'' || a == '"';
}
bool isKeyWord(std::string keyWord)
{
    static const std::set<std::string> keyWords = {"abstract", "else", "instanceof", "super", "boolean", "enum", "int", "switch", "break", "export", "interface", "synchronized", "byte", "extends", "let", "this", "case", "false", "long", "throw", "catch", "final", "native", "throws", "char", "finally", "new", "transient", "class", "float", "null", "true", "const", "for", "package", "try", "continue", "function", "private", "typeof", "debugger", "goto", "protected", "var", "default", "if", "public", "void", "delete", "implements", "return", "volatile", "do", "import", "short", "while", "double", "in", "static", "with"};
    return keyWords.find(keyWord) != keyWords.end();
}

DFA numberMatch = lex::regexp(R"((([1-9][0-9]*)|0)((\.[0-9][0-9]*)|#)(([Ee]([+-]|#)[0-9][0-9]*)|#))");
DFA operatorMatch = lex::regexp(R"(([-+*/%=&^|><!](=|#))|([?:~])|(>>=)|(<<=)|(&&)|(\|\|)|(++)|(--))");
// DFA operatorMatch = lex::regexp(R"([-+*/%=&^|><!])");
DFA getMultiLine()
{
    auto q0 = std::make_shared<FAState>();
    auto q1 = std::make_shared<FAState>();
    auto q2 = std::make_shared<FAState>();
    auto q3 = std::make_shared<FAState>();
    auto q4 = std::make_shared<FAState>();
    auto q5 = std::make_shared<FAState>();
    q5->isFinal = true;
    DFA a({q0, q1, q2, q3, q4, q5}, q0);
    q0->transition = [](char a) {
        if (a == '/')
            return 1;
        return -1;
    };
    q1->transition = [](char a) {
        if (a == '*')
            return 2;
        return -1;
    };
    q2->transition = [](char a) {
        return 3;
    };
    q3->transition = [](char a) {
        if (a == '*')
            return 4;
        return 3;
    };
    q4->transition = [](char a) {
        if (a == '/')
            return 5;
        return -1;
    };

    return a;
}
template <typename T>
void tokenPrinter(Token token, T &logFile)
{
    logFile << "Token: " << tokenString[token.type] << std::endl;
    if (token.subType != "")
        logFile << "Sub Type: " << token.subType << std::endl;
    if (token.type == STRING)
        logFile << "Value: \"" << token.value << "\"" << std::endl;
    else
        logFile << "Value: " << token.value << std::endl;
    logFile << "Column: " << token.column << std::endl;
    logFile << "Line: " << token.line << std::endl;
    if (token.extra != "")
        logFile << "Extra: " << token.extra << std::endl;
    logFile << "=====================================\n\n";
}

} // namespace lex

#endif // LEXER_UTILS_H
