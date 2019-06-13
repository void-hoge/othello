#include "evaluator.hpp"

namespace bm = bitmanipulations;

int midevaluator::evaluate(const board& bd){
    if (bd.getboard(bd.getcurrentcolor()) == 0) {
        return -2147483647;
    }
    int mobility_score = bm::bit_count(bd.history[bd.getturn()].mobility);
    int stable_score = bm::bit_count(bd.getstablediscs(bd.getcurrentcolor())) - bm::bit_count(bd.getstablediscs(1-bd.getcurrentcolor()));
    return mobility_score + stable_score;
}

int wldevaluator::evaluate(const board& bd){
    // std::cout << "wld" << std::endl;
    int hoge = bd.countdiscs(bd.getcurrentcolor()) - bd.countdiscs(1-bd.getcurrentcolor());

    if (hoge > 0) {
        return 1;       //win
    }else if(hoge < 0){
        return -1;      //lose
    }else{
        return 0;       //draw
    }
}

int perfectevaluator::evaluate(const board& bd){
    // std::cout << "perfect" << std::endl;
    return bd.countdiscs(bd.getcurrentcolor()) - bd.countdiscs(1-bd.getcurrentcolor());
}
