
#if !defined(FATUPLES_H)
#define FATUPLES_H
#include <memory>
#include <functional>
struct FATuples
{
    static std::shared_ptr<FATuples> rejected()
    {
        return std::make_shared<FATuples>(false, true);
    }
    FATuples()
    {
        isFinal = false;
        isRejected = false;
        transition = [](char a) {
            return -1;
        };
    }

    FATuples(bool isFinal, bool isRejected, int (*transition)(char))
    {
        this->isFinal = isFinal;
        this->isRejected = isRejected;
        this->transition = transition;
    }

    FATuples(bool isFinal, bool isRejected = false)
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

#endif // FATUPLES_H
