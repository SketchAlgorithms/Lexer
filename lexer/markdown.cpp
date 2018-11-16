#include <iostream>
#include "lexer/markdown/markdown-lexer.h"
#include "lexer/lexer-utils.h"
#include <fstream>
#include <sstream>
int main(int argc, char const *argv[])
{

    std::ifstream file("test.md");
    std::ofstream logFile("test.log");
    std::string input;
    if (!file)
        throw "Can't Open File";
    std::stringstream buffer;
    buffer << file.rdbuf();
    input = buffer.str();

    lex::Markdown lexer(input);
    // logFile << lexer.toString();
    auto next = lexer.nextToken();
    while (next.type != "EOT")
    {
        lex::tokenPrinter(next,logFile);
        next = lexer.nextToken();
    }

    return 0;
}
