#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "board.hpp"
#include "AI.hpp"

#include <vector>

class game{
private:
	AI* ai;
	board bd;
	void playermove();
	void aimove();
public:
	game();
	~game();

	void sente();
	void gote();
	void aivsai();
};

#endif
