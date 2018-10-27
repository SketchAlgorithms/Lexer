#include <iostream>
#include "headers/FATuples.h"
#include "headers/dfa.h"
#include "headers/re_to_postfix.h"


int main(int argc, char const *argv[])
{

    reToPostFix(R"(123\.)");
    reToPostFix(R"(123\\)");
    reToPostFix(R"(\+\.\*)");
    reToPostFix(R"((1|2|3)*)");

    return 0;
}
