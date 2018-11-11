#include <iostream>
#include "regexp/fa_state.h"
#include "regexp/nfa.h"
#include "regexp/re_to_postfix.h"
#include "regexp/directMethod.h"
#include "regexp/regexp.h"
#include <fstream>
#include <sstream>

void testNumbers()
{

    // NFA numberMatch = lex::regexp(R"((([1-9][0-9]*)|0)((\.[0-9][0-9]*)|#)(([Ee]([+-]|#)[0-9][0-9]*)|#))");
    // NFA numberMatch = lex::regexp(R"((([1-9][0-9]*)|0)(\.[0-9]+)?([Ee][+-]?[0-9]+)?)");
    NFA numberMatch = lex::regexp(R"((([1-9]\d*)|0)(\.\d+)?([Ee][+-]?\d+)?)");

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
    auto id = lex::regexp(R"([a-zA-Z_]\w+)");
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

void testMultiLineComment()
{
    auto nfa = lex::regexp(R"(/\*.*1?\*/)");
    std::cout
        << "TEST: "
        << ((

                nfa.etf("/**/") &&
                !nfa.etf("/*/") &&
                nfa.etf("/*123123*/"))
                ? "Accepted"
                : "Rejected");
}

void testOperator()
{

    NFA nfa = lex::regexp(R"([-+*/%=&^|><!]=?|[?:~]|>>=|<<=|&&|\|\||\+\+|--)");

    std::cout
        << "TEST: "
        << ((nfa.etf("+") &&
             nfa.etf("-") &&
             nfa.etf("/") &&
             nfa.etf("%") &&
             nfa.etf("&") &&
             nfa.etf("^") &&
             nfa.etf("+=") &&
             nfa.etf("-=") &&
             nfa.etf("++") &&
             nfa.etf("--") &&
             !nfa.etf("a"))
                ? "Accepted"
                : "Rejected");
}
void Test()
{
    utils::withTime([]() {
        utils::withTime(testNumbers, "NUMBER");
        utils::withTime(testID, "IDENTIFIER");
        utils::withTime(testMultiLineComment, "MULTILINE COMMENT");
        utils::withTime(testOperator, "OPERATOR");
    },
                    "RUNNING TESTS");
}

int main(int argc, char const *argv[])
{
    Test();
    NFA dfa = lex::regexp(R"(#( |\t).*\n)", 1);
    std::cout << dfa.etf("# \n");
    std::cout << dfa.etf("# 1\n");
    std::cout << dfa.etf("# 1n");
    std::cout << dfa.etf("# 12");

    return 0;
}
