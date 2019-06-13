#ifndef  AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include "board.hpp"
#include "evaluator.hpp"

class AI {
protected:
    const unsigned presearch_depth;
    const unsigned normal_depth;
    const unsigned wld_depth;
    const unsigned perftct_depth;
public:
    AI(): presearch_depth(6), normal_depth(11),wld_depth(20),perftct_depth(18){};

    virtual void move(board& bd) = 0;
    int node_coount;
};

class alphabetaAI : public AI{
private:
    evaluator* eval;
    void sort(board& bd, const int limit);
    int alphabeta_recursion(board& bd, const int limit, int alpha, int beta);
public:
    void move(board& bd);
};

#endif
