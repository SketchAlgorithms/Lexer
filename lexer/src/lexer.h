
#if !defined(LEXER_H)
#define LEXER_H
#include <iostream>
#include <vector>
#include "token.h"
#include "lex-utils.h"
#include "tokenizer.h"
#include <sstream>
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
  explicit Lexer(Tokenizer tokenizer, std::string input = "")
  {
    this->tokenCount = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    this->position = 0;
    this->line = 1;
    this->column = 1;
    this->input = input;
    this->tokenizer = tokenizer;
    this->stringOpen = '\0';
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
    if (stringOpen != '\0' || tokenizer.isStringOrChar(character))
    {
      return processStringChar();
    }
    else if (tokenizer.isAlphabet(character))
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
    else if (character == '/')
    {
      if ((position + 1) < input.length())
      {
        const auto nextChar = input.at(position + 1);
        if (nextChar == '/')
          return processSingleLineComment();
        if (nextChar == '*')
          return processMultiLineComment();
      }
      // if (position > 0 && !lex::isAlphabet(input.at(position)) && !lex::isDigit(input.at(position)))
      // {
      //   return processRegExp();
      // }
      return processOperator();
    }
    else if (tokenizer.isOperator(character))
    {
      return processOperator();
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
    return Token(DELIMETER, bracket, line, column++, "BRACKET", std::to_string(pair));
  }

  /**
   * summary - returns Operators token
   * @method processOperator
   * @return {Token}
  */
  Token processOperator()
  {
    std::string op = "";
    auto lookAhead = this->position;
    auto column = this->column;
    auto position = this->position;
    auto dfa = tokenizer.operatorMatch;
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
      op += character;
      ++lookAhead;

    } while (!currentState->isRejected);
    std::string acceptedNumber = op.substr(0, position - this->position + 1);
    this->position = position + 1;
    this->column += acceptedNumber.length();
    tokenCount[OPERATOR]++;
    return Token(OPERATOR, acceptedNumber, line, column);
  }

  /**
   * summary - returns String Token 
   * @method processStringChar
   * @return {Token}
  */
  Token processStringChar()
  {
    std::string literal = "";
    auto character = input.at(position);

    if (tokenizer.isStringOrChar(character))
    {
      position++;
      literal.push_back('\\');
      if (stringOpen == '\0')
      {
        stringOpen = character;
        literal.push_back(character);
        return Token(DELIMETER, literal, line, column++, character == '"' ? "STRING" : "CHAR", "OPEN");
      }
      stringOpen = '\0';
      literal.push_back(character);
      return Token(DELIMETER, literal, line, column++, character == '"' ? "STRING" : "CHAR", "CLOSE");
    }
    auto lookAhead = this->position;
    auto column = this->column;

    do
    {

      if (character == '\n' || ++lookAhead >= input.length())
      {
        position = lookAhead;
        stringOpen = '\0';
        this->column += literal.length();
        return Token(UNDEF, literal, line, column + 1);
      }
      literal += character;

      character = input.at(lookAhead);
    } while (character != stringOpen);
    position = lookAhead;
    this->column += literal.length();
    return Token(STRING, literal, line, column);
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
   * summary - returns single line comment
   * @method
   * @return {std::shared_ptr<Token>} - A Token
  */
  Token processSingleLineComment()
  {
    std::string comment = "";
    auto startColumn = column;
    char character = input.at(position);
    while (character != '\n')
    {
      comment += character;

      if (++position >= input.length())
        break;
      character = input.at(position);
    }
    position++;
    column = 1;

    tokenCount[COMMENT]++;
    return Token(COMMENT, comment, line++, startColumn);
  }
  /**
   * summary - returns multi line comment
   * @method
   * @return {std::shared_ptr<Token>} - A Token
  */
  Token processMultiLineComment()
  {
    std::string comment = "";
    auto startColumn = column;
    auto startLine = line;
    char character;
    auto dfa = tokenizer.multiLineCommentMatch;
    auto currentState = dfa.getStart();
    do
    {
      column++;
      character = input.at(position);
      // std::cout << character;
      if (character == '\n')
      {
        line++;
        column = 1;
        comment += "\\n";
      }
      else if (character == '"')
      {
        comment += "\\\"";
      }
      else
        comment += character;

      if (++position >= input.length())
        break;

      currentState = dfa.next(character);

    } while (!currentState->isFinal);

    tokenCount[COMMENT]++;
    return Token(COMMENT, comment, startLine, startColumn);
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

public:
  std::string toString()
  {
    std::stringstream file;
    file << " <div class=\"snippet\"> <style scoped> * { margin: 0; padding: 0; box-sizing: border-box; font-family: monospace; } .KEYWORD { color: #45a1ce; } .IDENTIFIER { color: black; } .NUMBER { color: purple; } .OPERATOR { color: #eaaf00; } .DELIMETER { color: #0004b7; } .STRING { color: green; } .COMMENT { color: #5050ff; font-style: italic; } .UNDEF { text-decoration: underline; color: red; } .snippet { background: white; padding: 10px; display: flex; align-items: stretch; overflow: auto; } .snippet .line div::after { content: \".\" } .snippet .line { padding-right: 10px; height: 100%; border-right: 2px solid red; user-select: none; } pre { padding-left: 1em; } </style><div class=\"line\"> <div>1</div> <div>2</div> <div>3</div> <div>4</div> <div>5</div> <div>6</div> <div>7</div> <div>8</div> <div>9</div> <div>10</div> <div>11</div> <div>12</div> </div><pre>";
    auto token = nextToken();
    int line = 1;
    int column = 1;

    auto addLine = [&](int l) mutable {
      const auto diff = l - line;
      for (int i = 0; i < diff; i++)
      {
        file << "\n";
      }
      line = l;
      column = 1;
    };
    auto addColumn = [&](int col) mutable {
      const auto diff = col - column;
      for (int i = 0; i < diff; i++)
      {
        file << " ";
      }
    };
    while (token.type != EOT)
    {

      if (line != token.line)
        addLine(token.line);
      if (column != token.column)
        addColumn(token.column);
      file << "<span class=\"" << tokenString[token.type] << "\">" << token.value << "</span>";
      column = token.column + token.value.length();

      token = nextToken();
    }
    file << "</pre></div>";
    std::string data = file.str();
    return data;
  }

  void setInput(std::string s)
  {
    input = s;
  }

private:
  int position;
  int line;
  int column;
  std::string input;
  std::vector<long> tokenCount;
  lex::Token prevToken;
  Tokenizer tokenizer;
  char stringOpen;
};
} // namespace lex

#endif // LEXER_H
