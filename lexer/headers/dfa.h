
#if !defined(DFA_H)
#define DFA_H
#include <vector>
#include <memory>
#include "./FATuples.h"
class DFA
{
  private:
    std::vector<std::shared_ptr<FATuples>> states;
    std::shared_ptr<FATuples> start;
    std::shared_ptr<FATuples> current;

  public:
    DFA()
    {
    }
    DFA(std::vector<std::shared_ptr<FATuples>> states,
        std::shared_ptr<FATuples> start)
    {
        setStates(states);
        setStart(start);
    }

    void setStates(std::vector<std::shared_ptr<FATuples>> states)
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

    void setStart(std::shared_ptr<FATuples> start)
    {
        if (start != nullptr)
        {
            this->start = start;
            resetCurrent();
        }
        else
        {
            throw "Error";
        }
    }
    void resetCurrent()
    {
        this->current = this->start;
    }

    auto getStates()
    {
        return states;
    }

    auto getStart()
    {
        return start;
    }
    auto getCurrent()
    {
        return current;
    }

    std::shared_ptr<FATuples> next(char c)
    {
        int state = this->current->transition(c);
        this->current = state != -1 ? states.at(state) : FATuples::rejected();
        return this->current;
    }

    bool etf(std::string input)
    {
        auto current = this->start;
        for (auto character : input)
        {
            if (current->isRejected)
                return false;
            int state = current->transition(character);
            current = state != -1 ? states.at(state) : FATuples::rejected();
        }

        if (current->isFinal)
            return true;
        return false;
    }
};

#endif // DFA_H
