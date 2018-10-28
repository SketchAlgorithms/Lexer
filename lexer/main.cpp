#include <iostream>
#include "headers/fa_state.h"
#include "headers/dfa.h"
#include "headers/re_to_postfix.h"
#include "headers/directMethod.h"

int main(int argc, char const *argv[])
{

    reToPostFix(R"(123\.)");
    reToPostFix(R"(123\\)");
    reToPostFix(R"(\+\.\*)");

    utils::withTime([]() {
        auto dfa = directMethod(reToPostFix(R"(a(a*|b*)*bba$)"));
        std::cout << dfa.etf("");
    },

                    "REPOSTFIX");
    // directMethod( reToPostFix(R"((a|#)bc*$)"));
    // auto dfa = directMethod( );
    

    return 0;
}
