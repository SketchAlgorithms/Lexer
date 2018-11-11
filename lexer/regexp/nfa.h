
#if !defined(NFA_H)
#define NFA_H
#include <vector>
#include <memory>
#include <set>
#include "fa_state.h"
class NFA
{
  private:
    std::vector<std::shared_ptr<FAState>> states;
    std::shared_ptr<FAState> start;
    std::set<int> current;
    bool isAccepted;
    bool isRejected;

  public:
    explicit NFA()
    {
    }
    explicit NFA(std::vector<std::shared_ptr<FAState>> states)
    {
        setStates(states);
    }

    void setStates(std::vector<std::shared_ptr<FAState>> states)
    {
        if (states.size())
        {
            this->states = states;
        }
        else
        {
            throw "Error";
        }
    }

    void resetCurrent()
    {
        this->current = {0};
    }

    auto getStates()
    {
        return states;
    }

    auto getStart()
    {
        return states.at(0);
    }
    auto getCurrent()
    {
        return current;
    }

    bool next(char c)
    {
        auto current = this->current;
        isRejected = true;
        isAccepted = false;
        std::set<int> next = {};
        for (auto pos : current)
        {
            auto trans = states.at(pos)->transition(c);
            next.insert(trans.begin(), trans.end());
        }
        current = next;

        for (auto pos : next)
        {
            if (states.at(pos)->isFinal)
            {
                isAccepted = true;
                break;
            }
            if (!states.at(pos)->isRejected)
            {
                isRejected = false;
            }
        }

        return isRejected;
    }

    bool etf(std::string input)
    {

        std::set<int> current = {0};
        for (auto character : input)
        {
            std::set<int> next = {};
            for (auto pos : current)
            {
                auto trans = states.at(pos)->transition(character);
                next.insert(trans.begin(), trans.end());
            }
            if (next.empty())
                return false;
            current = next;
        }
        for (auto pos : current)
        {
            if (states.at(pos)->isFinal)
            {
                return true;
            }
        }

        return false;
    }
};

#endif // NFA_H
