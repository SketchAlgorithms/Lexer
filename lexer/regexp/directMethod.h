
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
#include "nfa.h"
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

NFA nodesToSate(std::set<int> startSet, std::vector<std::pair<std::set<int>, Expression>> nodes)
{
    // TO Determine Final Node
    const int finalNode = nodes.size() - 1;
    // Keeping Track of found States and there positions
    std::unordered_map<std::string, int> uSets;
    // Adding the first State
    auto startKey = utils::nodeSetToString(startSet);
    auto stateCount = 0;
    uSets[startKey] = stateCount++;

    // remaining sets to be marked
    std::deque<std::set<int>> remaining = {startSet};
    // Contains Sets whether they are final or not
    std::vector<bool> finalState = {};
    // Counts how many times the loop enter the inner most loop
    long count = 0;
    // Keeping track of nodes comming out as marked
    int nodeCount = 0;
    // Keeps track of transition
    std::vector<std::vector<std::pair<Expression, int>>> stateTransition;

    // Looping While all the found sets are not marked
    while (!remaining.empty())
    {
        // Checking if the set is final
        auto isFinal = false;
        // the marked set
        auto mark = remaining.front();
        // Its Index
        auto markIndex = nodeCount++;
        // Removing from the queue
        remaining.pop_front();
        // Initializing transition
        stateTransition.push_back({});
        // Making a set so that un necessary comparisions are not mafe
        std::set<int> foundTransition = {};
        std::set<std::string> foundAlphabet = {};
        // Iterating the nodes in set
        for (auto it = mark.begin(); it != mark.end(); ++it)
        {

            // If the node contains final node
            if ((*it) == finalNode)
                isFinal = true;
            // if already marked continue
            if (foundTransition.find(*it) != foundTransition.end())
                continue;
            //initializing next state
            std::set<int> transState = nodes[*it].first;
            // Transition Alphabet
            auto alphabet = nodes[*it].second;
            // Checking if others states have transition
            for (auto jt = std::next(it); jt != mark.end(); ++jt)
            {
                auto compareAlphabet = nodes[*jt].second;

                // if the node has same alphabet
                if (compareAlphabet.value == alphabet.value && compareAlphabet.rangeType == alphabet.rangeType && compareAlphabet.range == alphabet.range)
                {
                    // merge the set
                    transState.insert(nodes[*jt].first.begin(), nodes[*jt].first.end());
                    foundTransition.insert(*jt);
                }
            }
            // If the set is not null
            if (transState.size())
            {
                // Check if node found else add it to uSets
                auto key = utils::nodeSetToString(transState);
                auto iter = uSets.find(key);
                int index;
                if (iter == uSets.end())
                {
                    index = stateCount;
                    uSets[key] = stateCount++;
                    remaining.push_back(transState);
                }
                else
                    index = (*iter).second;

                stateTransition.at(markIndex).push_back(std::make_pair(alphabet, index));
            }
        }
        // Pushing if Final
        finalState.push_back(isFinal);
    }

    // std::cout << "Loop Enter: " << count << std::endl;
    // std::cout << "Nodes Count " << nodes.size() << std::endl;
    std::vector<std::shared_ptr<FAState>> dStates;
    const int nfaLength = stateTransition.size();

    for (int i = 0; i < nfaLength; ++i)
        dStates.push_back(std::make_shared<FAState>());
    NFA nfa(dStates);

    for (int i = 0; i < nfaLength; ++i)
    {
        dStates[i]->isFinal = finalState.at(i);
        dStates[i]->transition = utils::getTransition(stateTransition[i]);
    }
    // std::cout << "NFA STATES: " << dStates.size() << std::endl;
    return nfa;
}
// Positive or Kleene Closure
Node closure(Node op1, bool nullable = true)
{
    auto node = Node(nullable);
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

NFA directMethod(std::vector<Expression> postfix)
{
    std::vector<std::pair<std::set<int>, Expression>> nodes;
    std::deque<Node> stack;
    auto counter = 0;
    for (auto i = postfix.begin(); i != postfix.end(); ++i)
    {
        auto exp = (*i);
        if (exp.type == "OPERAND")
        {
            if (exp.sub == "NULL")
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
        else if (exp.value == "{")
        {
            auto op = stack.back();

            stack.pop_back();
            (exp.range->lower != 0) ? stack.push_back(closure(op, false)) : stack.push_back(closure(op));
            for (auto pos : op.firstPos)
            {
                nodes[pos].second.range = exp.range;
                nodes[pos].second.rangeType = "START";
            }
            // Follow Pos
            for (auto pos : op.lastPos)
            {
                nodes[pos].second.range = exp.range;
                nodes[pos].second.rangeType = nodes[pos].second.rangeType == "START" ? "BOTH" : "END";
                nodes[pos].first.insert(op.firstPos.begin(), op.firstPos.end());
            }
        }
        else if (exp.value == "*" || exp.value == "+")
        {
            auto op = stack.back();
            stack.pop_back();
            (exp.value == "+") ? stack.push_back(closure(op, false)) : stack.push_back(closure(op));

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
        else if (exp.value == "|" || exp.value == "?")
        {
            if (exp.value == "?")
                stack.push_back(Node());
            auto op1 = stack.back();
            stack.pop_back();
            auto op2 = stack.back();
            stack.pop_back();
            stack.push_back(unionPlus(op2, op1));
        }
    }
    return nodesToSate(stack.front().firstPos, nodes);
}

#endif // DIRECT_METHOD_H
