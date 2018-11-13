
#if !defined(FASTATE_H)
#define FASTATE_H
#include <memory>
#include <functional>
#include <set>
struct FAState
{
    static std::shared_ptr<FAState> rejected()
    {
        return std::make_shared<FAState>(false, true);
    }
    FAState()
    {
        isFinal = false;
        isRejected = false;
        transition = [](char a,int) {
            return std::set<int>();
        };
    }

    FAState(bool isFinal, bool isRejected, std::set<int> (*transition)(char,int))
    {
        this->isFinal = isFinal;
        this->isRejected = isRejected;
        this->transition = transition;
    }

    FAState(bool isFinal, bool isRejected = false)
    {
        this->isFinal = isFinal;
        this->isRejected = isRejected;
        transition = [](char a,int) {
            return std::set<int>();
        };
    }
    bool isFinal;
    bool isRejected;
    std::function<std::set<int>(char,int)> transition;
};

#endif // FASTATE_H
