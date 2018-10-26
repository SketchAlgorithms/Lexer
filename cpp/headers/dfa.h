#ifndef DFA_H
#define DFA_H
#include "fastate.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <functional>
#include <algorithm>

class DFA
{
  public:
    DFA(FAState *start, std::set<int> alphabet = {})
    {
        this->start = start;
        this->alphabet = alphabet;
        this->current = start;
    }
    int getAlphabet(char c)
    {
        int count = 0;
        for (auto it = this->alphabet.begin(); it != this->alphabet.end(); ++it)
        {
            if (*it == c)
                return count;
            count++;
        }
        return -1;
    }
    bool transition(char character)
    {
        auto alphabet = this->getAlphabet(character);
        if (alphabet == -1)
        {
            auto reject = new FAState();
            reject->isRejected = true;
            return reject;
        }

        this->current = this->current->transitions.at(alphabet).at(0);
        return this->current->isFinal;
    }

    bool test(std::string str, int (*getAlphabet)(int))
    {
        auto current = this->start;
        auto length = str.length();
        for (unsigned int i = 0; i < length; i++)
        {
            auto alphabet = getAlphabet(str[i]);
            if (alphabet == -1 || current->isRejected)
                return false;
            current = current->transitions[alphabet][0];
        }

        return current->isFinal;
    }

    bool etf(std::string str)
    {
        auto current = this->start;
        auto length = str.length();
        for (unsigned int i = 0; i < length; i++)
        {
            auto alphabet = this->getAlphabet(str[i]);
            if (alphabet == -1 || current->isRejected)
                return false;
            current = current->transitions[alphabet][0];
        }

        return current->isFinal;
    }

    static FAState *next(FAState *state, int (*getAlphabet)(int), char character)
    {
        auto alphabet = getAlphabet(character);
        if (alphabet == -1)
        {
            auto reject = new FAState();
            reject->isRejected = true;
            return reject;
        }

        return state->transitions.at(alphabet).at(0);
    }
    static bool isRejected(FAState *s)
    {
        return s->isFinal ? false : std::all_of(s->transitions.begin(), s->transitions.end(), [s](std::vector<FAState *> trans) {
            return trans[0] == s;
        });
    }

  private:
    FAState *start;
    FAState *current;
    std::set<int> alphabet;
};
#endif