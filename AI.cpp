#include "AI.hpp"
#include <iostream>

namespace bm = bitmanipulations;

void alphabetaAI::move(board& bd){
    // show_binary(bd.gethash());
    uint64 mobility = bd.checkcurrentmobility();

    if (mobility == 0) {
        bd.pass();
        return;
    }
    if (bm::bit_count(mobility) == 1) {
        bd.putdisc(mobility);
        return;
    }
    int limit;
    eval = new midevaluator();
    sort(bd, presearch_depth);
    int evalue, alpha = -intmax, beta = intmax;
    poseval p;
    if (bm::bit_count(bd.getblank()) <= wld_depth) {
        limit = intmax;
        if (bm::bit_count(bd.getblank()) <= perftct_depth) {
            eval = new perfectevaluator();
        }   else{
            eval = new wldevaluator();
        }
    }   else{
        limit = normal_depth;
    }
    bd.history[bd.getturn()].expandmobility();
    int mobility_size = bm::bit_count(mobility);
    for (int i = 0; i < mobility_size; i++) {
        bd.putdisc(bd.history[bd.getturn()].mobility_list[i].pos);
        evalue = -alphabeta_recursion(bd, limit-1, -beta, -alpha);
        bd.undo();
        if (evalue > alpha) {
            alpha = evalue;
            p = bd.history[bd.getturn()].mobility_list[i];
        }
    }

    eval = NULL;
    bd.putdisc(p.pos);
}

void alphabetaAI::sort(board &bd, const int limit){
    uint64 mobility = bd.history[bd.getturn()].mobility;
    bd.history[bd.getturn()].expandmobility();
    int evalue;
    int mobility_size = bm::bit_count(mobility);
    for (int i = 0; i < mobility_size; ++i){
        bd.putdisc(bd.history[bd.getturn()].mobility_list[i].pos);
        evalue = -alphabeta_recursion(bd, limit-1, -intmax, intmax);
        bd.undo();
        bd.history[bd.getturn()].mobility_list[i].eval = evalue;
    }
    std::stable_sort(bd.history[bd.getturn()].mobility_list.begin(), bd.history[bd.getturn()].mobility_list.begin(), comp);
}


int alphabetaAI::alphabeta_recursion(board &bd, const int limit, int alpha, int beta){
    if (limit == 0) {
        return eval -> evaluate(bd);
    }
    uint64 mobility = bd.history[bd.getturn()].mobility;
    int score;
    if ((bd.getblank() == mobility) && (bm::bit_count(mobility) == 1)) {
        bd.putdisc(mobility);
        score = -(eval -> evaluate(bd));
        bd.undo();
        return score;
    }
    if (mobility == 0) {
        uint64 antimobility = bd.checkmobility(1-bd.getcurrentcolor());
        if ((antimobility == bd.getblank()) && (bm::bit_count(antimobility) == 1)) {
            bd.pass();
            bd.putdisc(antimobility);
            score = eval -> evaluate(bd);
            bd.undo();
            bd.undo();
            return score;
        }
        if (antimobility == 0) {
            return eval -> evaluate(bd);
        }
        bd.pass();
        score = -alphabeta_recursion(bd, limit, -beta, -alpha);
        bd.undo();
        return score;
    }
    bd.history[bd.getturn()].expandmobility();
    int mobility_size = bm::bit_count(mobility);
    for (int i = 0; i < mobility_size; i++) {
        bd.putdisc(bd.history[bd.getturn()].mobility_list[i].pos);
        score = -alphabeta_recursion(bd, limit-1, -beta, -alpha);
        bd.undo();
        if (score >= beta) {
            return score;
        }
        alpha = std::max(alpha, score);
    }
    return alpha;
}
