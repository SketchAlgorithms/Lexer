#include <iostream>
#include "headers/FATuples.h"
#include "headers/dfa.h"

bool id(std::string input)
{
    auto start = std::make_shared<FATuples>();
    auto alpha = std::make_shared<FATuples>(true);
    auto num = std::make_shared<FATuples>(true);

    DFA a({start, alpha, num}, start);
    start->transition = [](char a) {
        if (a >= 'a' && a <= 'z')
            return 1;
        return -1;
    };
    auto trans = [](char a) {
        if (a >= 'a' && a <= 'z')
            return 1;
        if (a >= '0' && a <= '9')
            return 2;
        return -1;
    };
    alpha->transition = trans;
    num->transition = trans;
    
    return a.etf(input);
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

    std::cout << (n->isFinal) ? "true" : "false";
    std::cout << (k->isRejected) ? "true" : "false";
    std::cout << a.etf("1");
    std::cout << a.etf("01010101010") << std::endl;
    std::cout << (id("int") ? "true" : "false");
    std::cout << (id("int1001asd10asd") ? "true" : "false");
    std::cout << (id("1int1001asd10asd") ? "true" : "false");

    return 0;
}
