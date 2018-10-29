
#if !defined(TOKENIZER_H)
#define TOKENIZER_H
#include "../headers/regexp.h"
#include "../headers/dfa.h"
#include "lex-utils.h"
namespace lex
{
struct Tokenizer
{
    explicit Tokenizer()
    {
        isNumber = lex::isNumber;
        isAlphabet = lex::isAlphabet;
        isWhiteSpace = lex::isWhiteSpace;
        isDelimeter = lex::isDelimeter;
        isOperator = lex::isOperator;
        isKeyWord = lex::isKeyWord;
        isBracket = lex::isBracket;
        numberMatch = lex::numberMatch;
        multiLineCommentMatch = lex::getMultiLine();
        isStringOrChar = lex::isStringOrChar;
    }

    bool (*isNumber)(char);
    bool (*isAlphabet)(char);
    bool (*isWhiteSpace)(char);
    bool (*isDelimeter)(char);
    bool (*isOperator)(char);
    bool (*isBracket)(char);
    bool (*isStringOrChar)(char);
    bool (*isKeyWord)(std::string);

    DFA numberMatch;
    DFA multiLineCommentMatch;
};
} // namespace lex

#endif // TOKENIZER_H
