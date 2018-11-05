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

    std::ifstream file("test.js");
    std::ofstream logFile("test.log");
    std::string input;
    if (!file)
        throw "Can't Open File";
    std::stringstream buffer;
    buffer << file.rdbuf();
    input = buffer.str();

    lex::Tokenizer t;
    lex::Lexer lexer(t, input);
    // logFile << lexer.toString();
    auto next = lexer.nextToken();
    logFile << "[\n";
    while (next.type != lex::TokenType::EOT)
    {
        logFile << lex::tokenJSON(next);
        next = lexer.nextToken();
        logFile << ",\n";
    }
    logFile << "{}]";

    return 0;
}
