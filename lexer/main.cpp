#include <iostream>
#include "headers/FATuples.h"
#include "headers/dfa.h"

auto getTransition(char a)
{
    return [a](char ab) {
        return ab == a;
    };
}

int main(int argc, char const *argv[])
{

    auto q0 = std::make_shared<FATuples>();
    auto q1 = std::make_shared<FATuples>();

    DFA a{{q0, q1}, q0};
    q1->isFinal = true;
    q0->transition = [](char a) {
        if (a == '0')
            return 1;
        return -1;
    };
    q1->transition = [](char a) {
        if (a == '1')
            return 0;
        return -1;
    };

    auto n = a.next('0');
    a.resetCurrent();
    auto k = a.next('0');

    return 0;
}
