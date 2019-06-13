#include <iostream>
#include "board.hpp"
#include "evaluator.hpp"
#include "AI.hpp"
#include "game.hpp"

int main(int argc, char const *argv[]){
	std::cout << "othello programming.\n";
	game gm;
	// gm.sente();
	gm.aivsai();
	// gm.gote();
	return 0;
}
