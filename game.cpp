#include "game.hpp"
#include <iostream>

game::game(){
    ai = new alphabetaAI;
}

game::~game(){
    // delete ai;
    ai = NULL;
}

void game::playermove(){
    int x, y;
    std::cout << "あなたの手番です。" << std::endl;
    if (bd.checkmobility(bd.getcurrentcolor()) == 0) {
        bd.pass();
        std::cout << "あなたはパスです。" << std::endl;
        return;
    }
    std::cout << "手を入力してください。" << std::endl;
    std::cin >> x >> y;
    uint64 pos = (uint64)1 << (x*8+y);
    while ((!bd.putdisc(pos))&&((bd.checkmobility(bd.getcurrentcolor())&pos) == 0)) {
        std::cout << "そこには打てません。" << std::endl;
        std::cout << "手を入力してください。" << std::endl;
        std::cin >> x >> y;
        pos = (uint64)1 << (x*8+y);
    }
}

void game::aimove(){
    std::cout << "AIの手番です。" << std::endl;
    std::cout << "思考中......" << std::endl;
    ai->move(bd);
}

void game::sente(){
    while (bd.isgameover()== false) {
        bd.showCUI();
        playermove();
        bd.showCUI();
        aimove();
    }
    bd.showCUI();

    std::cout << "あなた: " << bd.countdiscs(black) << std::endl;
    std::cout << "AI: " << bd.countdiscs(white) << std::endl;
}

void game::gote(){
    while (bd.isgameover() == false) {
        bd.showCUI();
        aimove();
        bd.showCUI();
        playermove();
    }
    bd.showCUI();

    std::cout << "AI: " << bd.countdiscs(black) << std::endl;
    std::cout << "あなた: " << bd.countdiscs(white) << std::endl;
}

void game::aivsai(){
    while (bd.isgameover() == false){
        bd.showCUI();
        aimove();
    }
    bd.showCUI();
    std::cout << "black: " << bd.countdiscs(black) << std::endl;
    std::cout << "white: " << bd.countdiscs(white) << std::endl;
}
