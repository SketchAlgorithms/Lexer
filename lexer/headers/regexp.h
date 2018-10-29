
#if !defined(REGEXP_H)
#define REGEXP_H
#include "fa_state.h"
#include "dfa.h"
#include "re_to_postfix.h"
#include "directMethod.h"
namespace lex
{
DFA regexp(std::string regexp)
{
    return directMethod(reToPostFix("(" + regexp + ")$"));
}
} // namespace lex

#endif // REGEXP_H
