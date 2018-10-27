
#if !defined(DIRECT_METHOD_H)
#define DIRECT_METHOD_H
#include <algorithm>
#include "./fa_state.h"
#include "utils.h"
#include <iostream>
#include <set>
#include <iterator>
#include <deque>
#include <unordered_map>
#include "dfa.h"
#include "./re_to_postfix.h"
#include <utility>
struct Node
{
    bool nullable;
    std::set<int> firstPos;
    std::set<int> lastPos;
    explicit Node(bool nullable = true, std::set<int> firstPos = {}, std::set<int> lastPos = {})
    {
        this->nullable = nullable;
        this->firstPos = firstPos;
        this->lastPos = lastPos;
    }
};

void nodesToSate(std::set<int> startSet, std::vector<std::pair<std::set<int>, Expression>> nodes)
{

    std::set<std::set<int>> foundSets = {startSet};
    std::deque<std::pair<std::set<int>, int>> remaining = {std::make_pair(startSet, 0)};
    std::vector<std::pair<std::set<int>, int>> vecSets = {std::make_pair(startSet, 0)};

    std::vector<std::vector<std::pair<std::string, int>>> stateTransition;
    while (!remaining.empty())
    {
        auto mark = remaining.front().first;
        auto markIndex = remaining.front().second;
        remaining.pop_front();
        stateTransition.push_back({});
        std::set<int> foundTransition = {};
        for (auto it = mark.begin(); it != mark.end(); ++it)
        {
            if (foundTransition.find(*it) != foundTransition.end())
                continue;
            std::set<int> transState = nodes[*it].first;
            std::string alphabet = nodes[*it].second.value;
            auto jt = mark.begin();

            for (auto jt = std::next(it); jt != mark.end(); ++jt)
            {
                if (nodes[*jt].second.value == alphabet)
                {
                    transState.insert(nodes[*jt].first.begin(), nodes[*jt].first.end());
                    foundTransition.insert(*jt);
                }
            }
            if (transState.size())
            {
                auto iter = std::find_if(vecSets.begin(), vecSets.end(), [transState](auto a) {
                    return a.first == transState;
                });
                int index;
                if (iter == vecSets.end())
                {

                    index = vecSets.size();
                    vecSets.push_back(std::make_pair(transState, index));
                    remaining.push_back(std::make_pair(transState, index));
                }
                else
                    index = (*iter).second;

                stateTransition.at(markIndex).push_back(std::make_pair(alphabet, index));
            }
        }
    }

    std::vector<std::shared_ptr<FAState>> dStates;
    const int dfaLength = stateTransition.size();

    for (int i = 0; i < dfaLength; ++i)
        dStates.push_back(std::make_shared<FAState>());
    DFA dfa(dStates, dStates[0]);
}
Node kleeneClosure(Node op1)
{
    auto node = Node();
    node.firstPos = op1.firstPos;
    node.lastPos = op1.lastPos;
    // delete op1;
    return node;
}
Node unionPlus(Node op1, Node op2)
{

    auto node = Node(op1.nullable || op2.nullable);
    std::set_union(op1.firstPos.begin(), op1.firstPos.end(), op2.firstPos.begin(), op2.firstPos.end(), std::inserter(node.firstPos, node.firstPos.begin()));
    std::set_union(op1.lastPos.begin(), op1.lastPos.end(), op2.lastPos.begin(), op2.lastPos.end(), std::inserter(node.lastPos, node.lastPos.begin()));
    return node;
}
Node concat(Node op1, Node op2)
{
    auto node = Node(op1.nullable && op2.nullable);
    if (op1.nullable)
        std::set_union(op1.firstPos.begin(), op1.firstPos.end(), op2.firstPos.begin(), op2.firstPos.end(), std::inserter(node.firstPos, node.firstPos.begin()));
    else
        node.firstPos = op1.firstPos;
    if (op2.nullable)
        std::set_union(op1.lastPos.begin(), op1.lastPos.end(), op2.lastPos.begin(), op2.lastPos.end(), std::inserter(node.lastPos, node.lastPos.begin()));
    else
        node.lastPos = op2.lastPos;
    return node;
}

DFA directMethod(std::vector<Expression> postfix)
{
    std::vector<std::pair<std::set<int>, Expression>> nodes;
    std::deque<Node> stack;
    auto counter = 0;
    for (auto i = postfix.begin(); i != postfix.end(); ++i)
    {
        auto exp = (*i);
        if (exp.type == "OPERAND")
        {
            if (exp.value == "#")
            {
                stack.push_back(Node());
            }
            else
            {
                int pos = counter++;
                auto node = Node(false, {pos}, {pos});
                std::set<int> set = {};
                nodes.push_back(std::make_pair(set, exp));
                stack.push_back(node);
            }
        }
        else if (exp.value == "*")
        {
            auto op = stack.back();
            stack.pop_back();

            stack.push_back(kleeneClosure(op));

            // Follow Pos
            for (auto pos : op.lastPos)
            {
                nodes[pos].first.insert(op.firstPos.begin(), op.firstPos.end());
            }
        }
        else if (exp.value == ".")
        {
            auto op1 = stack.back();
            stack.pop_back();
            auto op2 = stack.back();
            stack.pop_back();

            for (auto pos : op2.lastPos)
            {
                nodes[pos].first.insert(op1.firstPos.begin(), op1.firstPos.end());
            }
            stack.push_back(concat(op2, op1));
        }
        else if (exp.value == "|")
        {
            auto op1 = stack.back();
            stack.pop_back();
            auto op2 = stack.back();
            stack.pop_back();
            stack.push_back(unionPlus(op2, op1));
        }
    }

    nodesToSate(stack.front().firstPos, nodes);
    return DFA();
}

#endif // DIRECT_METHOD_H
