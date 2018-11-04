
#if !defined(EXPRESSION_H)
#define EXPRESSION_H
struct Expression
{
    Expression()
    {
        this->type = "";
        this->value = "";
        this->sub = "";
    }
    explicit Expression(std::string type,
                        std::string value, std::string sub = "")
    {
        this->type = type;
        this->value = value;
        this->sub = sub;
    }
    explicit Expression(std::string type, char c, std::string sub = "")
    {
        this->type = type;
        this->value.push_back(c);
        this->sub = sub;
    }
    std::string type;
    std::string value;
    std::string sub;
};

#endif // EXPRESSION_H
