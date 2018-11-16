
#if !defined(MARKDOWN_LEXER_H)
#define MARKDOWN_LEXER_H
#include <iostream>
#include <vector>
#include "../../regexp/regexp.h"
#include "../token.h"
namespace lex
{

auto isControlChar(char c)
{
  return c;
}

auto getHeader()
{
  return lex::regexp(R"(#{1,6}(( |\t).*)?\n|.*\n *(-+|=+) *\n)");
}
auto getParagraph()
{
  return lex::regexp(R"(.*[^\n][^\n]|[^\n]*\n?)");
}

class Markdown
{
private:
  int position;
  int line;
  int column;
  std::string input;
  // std::vector<long> tokenCount;

public:
  Markdown(std::string input = "")
  {
    this->position = 0;
    this->line = 1;
    this->column = 1;
    this->input = input;
  }
  Token nextToken()
  {

    auto header = getParagraph();
    std::string value = "";
    while (position < input.length())
    {
      char alpha = input.at(position);
      std::cout << alpha;
      position++;
      header.next(alpha);
      if (!header.isAccepted)
      {
        return Token("Header", value);
      }
      value += alpha;
    }
    return Token("EOT");
  }

  int getControl()
  {
    auto lookAhead = position;
    int spaceCount = 0;
    auto currentChar = input.at(lookAhead);
    while (lookAhead < input.length())
    {
      if (currentChar == '#')
      {
        return 1;
      }
      if (currentChar == '-')
      {
        return 1;
      }
      if(currentChar == )
    }
  }
};
} // namespace lex

#endif // MARKDOWN_LEXER_H
