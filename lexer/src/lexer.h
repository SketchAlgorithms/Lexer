
#if !defined(LEXER_H)
#define LEXER_H
#include <iostream>
#include <vector>
#include "token.h"
#include "lex-utils.h"
#include "tokenizer.h"
namespace lex
{

/** Class Lexer
 * summary - Receives a file name as an input and generates nextToken on method call.
 * **/
class Lexer
{
public:
  /**
    * @constructor
    *  Lexer()
    */
  Lexer(Tokenizer tokenizer, std::string input = "")
  {
    this->tokenCount = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    this->position = 0;
    this->line = 1;
    this->column = 1;
    this->input = input;
    this->tokenizer = tokenizer;
  }
  /**
   * summary - returns next token in sequence pointed by line  number and column number
   * @method nextToken
   * @private
   * @return {Token} - A Token
  */
  Token nextToken()
  {
    skipWhiteSpace();
    // Checking if the scanning is complete
    if (position >= input.length())
    {
      //End
      return Token(EOT);
    }
    // Reading Character
    char character = input.at(position);
    if (tokenizer.isAlphabet(character))
    {
      return processAlphabet();
    }
    else if (tokenizer.isNumber(character))
    {
      return processNumber();
    }
    else if (tokenizer.isBracket(character))
    {
      return processBracket();
    }
    else if (tokenizer.isDelimeter(character))
    {
      return processDelimeter();
    }
    position++;
    std::string s;
    s.push_back(character);
    return Token(UNDEF, s, column++, line);
  }

private:
  /**
   * summary - returns Delimeter tokens
   * @method processDelimeter
   * @return {Token}
  */
  Token processDelimeter()
  {
    std::string delim = "";
    delim += input.at(position++);
    tokenCount[DELIMETER]++;
    return Token(DELIMETER, delim, line, column++);
  }
  /**
   * summary - returns Bracket Token (Delimeter)
   * @method processBracket
   * @return {Token}
  */
  Token processBracket()
  {
    static std::vector<int> stack = {};
    static int count = 0;
    std::string bracket = "";
    bracket += input.at(position++);
    int pair;
    if (lex::isClosingBracket(bracket.at(0)))
    {
      pair = stack.back();
      stack.pop_back();
    }
    else
    {
      pair = count++;
      stack.push_back(pair);
    }
    tokenCount[DELIMETER]++;
    return Token(DELIMETER, bracket, line, column++, std::to_string(pair));
  }

  /**
   * summary - returns an identifier or a keyword using lookahead
   * @method
   * @return {Token}
  */
  Token processAlphabet()
  {

    //look ahead buffer
    long lookAhead = position;
    //the column pointing identifier start
    long startColNum = column;

    //Containing the word
    std::string word = "";

    char nextChar = input.at(lookAhead);

    // Looking ahead until valid characters
    while (tokenizer.isAlphabet(nextChar) || tokenizer.isNumber(nextChar))
    {
      word += nextChar;
      if (++lookAhead >= input.length())
        break;
      nextChar = input.at(lookAhead);
    }
    //updating new values
    position = lookAhead;
    column += word.length();

    //checking for keyword or identifier
    if (tokenizer.isKeyWord(word))
    {
      tokenCount[KEYWORD]++;
      return Token(KEYWORD, word, line, startColNum);
    }
    tokenCount[IDENTIFIER]++;
    return Token(IDENTIFIER, word, line, startColNum);
  }

  /**
   * summary - returns Number Token 
   * @method processNumber
   * @private
   * @return {Token}
  */
  Token processNumber()
  {
    std::string number = "";
    auto lookAhead = this->position;
    auto column = this->column;
    auto position = this->position;
    auto dfa = tokenizer.numberMatch;
    auto currentState = dfa.getStart();
    do
    {
      if (lookAhead >= input.length())
        break;
      auto character = input.at(lookAhead);
      currentState = dfa.next(character);

      if (currentState->isFinal)
      {
        position = lookAhead;
      }
      number += character;
      ++lookAhead;

    } while (!currentState->isRejected);

    std::string acceptedNumber = number.substr(0, position - this->position + 1);
    this->position = position + 1;
    this->column += acceptedNumber.length();
    tokenCount[NUMBER]++;
    return Token(NUMBER, acceptedNumber, line, column);
  }

  /**
   * summary - Skips all white space incrementing column and line count
   * @method
  */
  void skipWhiteSpace()
  {
    if (position >= input.length())
      return;
    char currentChar = input.at(position);

    while (tokenizer.isWhiteSpace(currentChar))
    {
      if (currentChar == '\n')
      {
        column = 1;
        line += 1;
      }
      else
      {
        column += 1;
      }
      if (++position >= input.length())
        break;
      currentChar = input.at(position);
    }
  }

private:
  int position;
  int line;
  int column;
  std::string input;
  std::vector<long> tokenCount;
  lex::Token prevToken;
  Tokenizer tokenizer;
};
} // namespace lex

#endif // LEXER_H
