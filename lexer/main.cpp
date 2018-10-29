#include <iostream>
#include "headers/fa_state.h"
#include "headers/dfa.h"
#include "headers/re_to_postfix.h"
#include "headers/directMethod.h"
#include "headers/regexp.h"
#include "src/lex-utils.h"
#include "src/lexer.h"
#include "src/tokenizer.h"
#include <fstream>
#include <sstream>
int main(int argc, char const *argv[])
{
    std::string arr[11] =
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

    std::ifstream file("test.cpp");
    std::ofstream logFile("test.log");
    std::string input;
    if (!file)
        throw "Can't Open File";
    std::stringstream buffer;
    buffer << file.rdbuf();
    input = buffer.str();

    lex::Tokenizer t;
    lex::Lexer lexer(t, input);
    auto next = lexer.nextToken();
    while (next.type != lex::TokenType::EOT)
    {
        logFile << "Token: " << arr[next.type] << std::endl;
        logFile << "Value: " << next.value << std::endl;
        logFile << "Column: " << next.column << std::endl;
        logFile << "Line: " << next.line << std::endl;
        logFile << "Extra: " << next.extra << std::endl;
        logFile << "=====================================\n\n";
        next = lexer.nextToken();
    }

    return 0;
}
