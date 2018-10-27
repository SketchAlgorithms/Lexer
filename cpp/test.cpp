#include <iostream>
#include "headers/utils.h"
#include "headers/direct_method.h"
#include "headers/re_to_postfix.h"
#include "headers/direct_method.h"
#include "headers/dfa.h"
#include "headers/fastate.h"
#include "headers/nfa.h"
#include "headers/enfa.h"
#include "headers/enfatodfa.h"
#include <set>
#include "headers/re_to_postfix.h"
#include "headers/postfix_re_to_enfa.h"
#include <cstring>
#include <string>
#include "headers/dfa.h"

int main(int argc, char const *argv[])
{

  auto dec = std::string("(1|2|3|4|5|6|7|8|9)");
  auto decZ = std::string("(0|1|2|3|4|5|6|7|8|9)*");

  auto reg = reToPostfix("a(b|(cc*))*de");
  auto preg = postfixToEnfa(reg);
  auto start = EnfaToDfa(preg.first).toDfa(preg.second.size());
  DFA a(start, preg.second);
  std::cout << a.etf("abcccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccde");

  return 0;
}
