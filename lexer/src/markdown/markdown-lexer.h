
#if !defined(MARKDOWN_LEXER_H)
#define MARKDOWN_LEXER_H
#include "../lex-utils.h";
#include <iostream>
#include <vector>
#include "../token.h"
#include "../tokenizer.h"
#include <sstream>

namespace lex
{

typedef enum
{
  HEADER,
  PARAGRAPH,
  
} MDToken
class MDLexer
{
private:
  int position;
  int line;
  int column;
  std::string input;
  std::vector<long> tokenCount;
  std::string prevToken;
  char stringOpen;

public:
  explicit MDLexer(std::string input = "")
  {
    this->tokenCount = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    this->position = 0;
    this->line = 1;
    this->column = 1;
    this->input = input;
    this->stringOpen = '\0';
  }

  MDToken getNextToken()
  {
  }
};
} // namespace lex

#endif // MARKDOWN_LEXER_H
