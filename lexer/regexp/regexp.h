
#if !defined(REGEXP_H)
#define REGEXP_H
#include "fa_state.h"
#include "nfa.h"
#include "re_to_postfix.h"
#include "directMethod.h"
namespace lex
{
NFA regexp(std::string regexp, int print = 0)
{
    return directMethod(reToPostFix("(" + regexp + ")$", print));
}
} // namespace lex

#endif // REGEXP_H
