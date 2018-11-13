
#if !defined(EXPRESSION_H)
#define EXPRESSION_H

struct Range
{
    Range()
    {
        lower = 0;
        upper = 0;
        counter = 0;
    }
    Range(int a, int b)
    {
        lower = a;
        upper = b;
        counter = 0;
    }
    int lower, upper, counter;
};
struct Expression
{
    explicit Expression()
    {
        this->type = "";
        this->value = "";
        this->sub = "";
        rangeType = "";
    }
    explicit Expression(std::string type,
                        std::string value, std::string sub = "")
    {
        this->type = type;
        this->value = value;
        this->sub = sub;
        rangeType = "";
    }
    explicit Expression(std::string type, char c, std::string sub = "")
    {
        this->type = type;
        this->value.push_back(c);
        this->sub = sub;
        rangeType = "";
    }
    std::string type;
    std::string value;
    std::string sub;
    std::string rangeType;
    std::shared_ptr<Range> range;
};

#endif // EXPRESSION_H
