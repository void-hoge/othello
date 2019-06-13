#ifndef EVALUATOR_HPP_INCLUDED
#define EVALUATOR_HPP_INCLUDED

#include "board.hpp"
#include <vector>

class evaluator {
protected:
    std::vector<int> weights;
public:
    virtual int evaluate(const board& bd) = 0;
};

class midevaluator: public evaluator{
public:
    int evaluate(const board& bd);
};

class wldevaluator: public evaluator{
public:
    int evaluate(const board& bd);
};

class perfectevaluator: public evaluator{
public:
    int evaluate(const board& bd);
};

#endif
