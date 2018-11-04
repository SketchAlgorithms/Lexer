#include <iostream>
#include "headers/fa_state.h"
#include "headers/dfa.h"
#include "headers/re_to_postfix.h"
#include "headers/directMethod.h"
#include "headers/regexp.h"
#include <fstream>
#include <sstream>

void testNumbers()
{

    // DFA numberMatch = lex::regexp(R"((([1-9][0-9]*)|0)((\.[0-9][0-9]*)|#)(([Ee]([+-]|#)[0-9][0-9]*)|#))");
    DFA numberMatch = lex::regexp(R"((([1-9][0-9]*)|0)(\.[0-9]+)?([Ee][+-]?[0-9]+)?)");

    std::cout << "TEST: "
              << ((numberMatch.etf("1000") &&
                   numberMatch.etf("123456789") &&
                   numberMatch.etf("123456789.0123456789") &&
                   numberMatch.etf("123456789.0123456789e+0123456789") &&
                   numberMatch.etf("123456789.0123456789E-0123456789") &&
                   numberMatch.etf("0.0123456789E-0123456789") &&
                   !numberMatch.etf("0123456789.0123456789E-0123456789") &&
                   !numberMatch.etf("0123456789.0123456789E-0123e456789") &&
                   !numberMatch.etf(".0123456789E-0123e456789") &&
                   numberMatch.etf("0.1e+1"))
                      ? "Accepted"
                      : "Rejected");
}
void testID()
{
    auto id = lex::regexp(R"([a-zA-Z_][a-zA-Z_0-9]+)");
    std::cout
        << "TEST: "
        << ((

                id.etf("abc") &&
                id.etf("abc123") &&
                id.etf("_abc123") &&
                id.etf("___abc123") &&
                id.etf("___123") &&
                id.etf("___abab") &&
                !id.etf("1___abab") &&
                !id.etf("1abs"))
                ? "Accepted"
                : "Rejected");
}
void Test()
{
    utils::withTime([]() {
        utils::withTime(testNumbers, "NUMBER");
        utils::withTime(testID, "IDENTIFIER"); }, "RUNNING TESTS");
}
int main(int argc, char const *argv[])
{

    Test();

    auto a = lex::regexp(R"([0-9]+)");
    std::cout << a.etf("11");
    std::cout << a.etf("1000000000000");
    std::cout << a.etf("2");
    std::cout << a.etf("+");
    std::cout << a.etf("1-");
    return 0;
}
