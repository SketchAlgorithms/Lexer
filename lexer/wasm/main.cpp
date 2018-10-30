#include <iostream>
#include "../headers/fa_state.h"
#include "../headers/dfa.h"
#include "../headers/re_to_postfix.h"
#include "../headers/directMethod.h"
#include "../headers/regexp.h"
#include "../src/lex-utils.h"
#include "../src/lexer.h"
#include "../src/tokenizer.h"
#include <fstream>
#include <sstream>

lex::Tokenizer t;
#include <emscripten/emscripten.h>
extern "C"
{

    char *fromStdString(std::string data)
    {
        char *str = new char[data.length() + 1];
        std::strcpy(str, data.c_str());
        return str;
    }
    lex::Lexer EMSCRIPTEN_KEEPALIVE lexer(t);
    char *EMSCRIPTEN_KEEPALIVE highlight()
    {
        const std::string data = lexer.toString();
        return fromStdString(data);
    }
    void EMSCRIPTEN_KEEPALIVE setInput(const char *input)
    {
        lexer = lex::Lexer(t, input);
    }
    char *EMSCRIPTEN_KEEPALIVE getNextToken()
    {
        return fromStdString(lex::tokenJSON(lexer.nextToken()));
    }
}

int main(int argc, char const *argv[])
{

    return 0;
}
