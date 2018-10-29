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

  utils::withTime([]() {
    auto pregex = reToPostfix("(a|b)*$");
    auto state = directMethod(pregex);
  },
                  "PostFIX");

  return 0;
}
