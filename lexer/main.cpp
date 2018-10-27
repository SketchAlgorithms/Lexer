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
    
    directMethod( reToPostFix(R"((a|#)bc*$)"));
    directMethod( reToPostFix(R"((a|b)*a$)"));

    return 0;
}
