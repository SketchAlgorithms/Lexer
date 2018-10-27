
#if !defined(FASTATE_H)
#define FASTATE_H
#include <memory>
#include <functional>
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
        transition = [](char a) {
            return -1;
        };
    }

    FAState(bool isFinal, bool isRejected, int (*transition)(char))
    {
        this->isFinal = isFinal;
        this->isRejected = isRejected;
        this->transition = transition;
    }

    FAState(bool isFinal, bool isRejected = false)
    {
        this->isFinal = isFinal;
        this->isRejected = isRejected;
        transition = [](char a) {
            return -1;
        };
    }
    bool isFinal;
    bool isRejected;
    std::function<int(char)> transition;
};

#endif // FASTATE_H
