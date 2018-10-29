#include <iostream>
#include "headers/fa_state.h"
#include "headers/dfa.h"
#include "headers/re_to_postfix.h"
#include "headers/directMethod.h"
#include "headers/regexp.h"
int main(int argc, char const *argv[])
{

    utils::withTime([]() {
        auto dfa = lex::regexp(R"([1-9][0-9]*((\.[0-9]*)|#))");
        std::cout << dfa.etf("100000");
        std::cout << dfa.etf("01");
        std::cout << dfa.etf("1000101");
        std::cout << dfa.etf("1000101.123123123");
        std::cout << std::endl;
        auto dfa1 = lex::regexp(R"([a-zA-Z_][a-zA-Z_0-9]*)");
        std::cout << dfa1.etf("a100000");
        std::cout << dfa1.etf("_01");
        std::cout << dfa1.etf("a1000101");
        std::cout << dfa1.etf("a1000101.123123123");
    },

                    "REPOSTFIX");
    // directMethod( reToPostFix(R"((a|#)bc*$)"));
    // auto dfa = directMethod( );

    return 0;
}
