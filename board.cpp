#include "board.hpp"

void show_binary(uint64 bits) {
	std::bitset<64> n = bits;
	for (int i = 0; i < 64; i++){
		if ((i % 8) == 0){
			std::cout << std::endl;
		}
		if (n[i]){
			std::cout << "1";
		}	else{
			std::cout << "0";
		}
	}
	std::cout << std::endl;
}

bool comp(const poseval a, const poseval b){
	return a.eval > b.eval;
}

board::board() {
	turn = 0;
	data[black] = 0;
	data[white] = 0;
	current_color = black;
	init();
}

board::~board() {}

void board::init() {
	set((uint64)1 << (3*8+3), white);
	set((uint64)1 << (4*8+4), white);
	set((uint64)1 << (3*8+4), black);
	set((uint64)1 << (4*8+3), black);
	checkcurrentmobility();
}

color board::check(const uint64 idx)const {
	if ((uint64)(data[white] & idx)!= 0){
		return white;
	}else if ((uint64)(data[black] & idx)!= 0){
		return black;
	}
	return empty;
}

int board::countdiscs(const color c)const {
	return bitmanipulations::bit_count(data[c]);
}

void board::set(const uint64 idx, const color c) {
	if (c == empty) {
		data[white] &= ~idx;
		data[black] &= ~idx;
		return;
	}
	data[c] |= idx;
	data[1-c] &= ~idx;
}

bool board::isgameover()const {
	if (checkmobility(current_color) != 0) {
		return false;
	}
	if (checkmobility(1-current_color) != 0) {
		return false;
	}
	return true;
}

color board::jadge()const {
	if (isgameover() == false) {
		return empty;
	}
	if (countdiscs(white) < countdiscs(black)) {
		return black;
	}
	if (countdiscs(white) > countdiscs(black)) {
		return white;
	}
	return draw;
}

uint64 board::checkcurrentmobility(){
	history[turn].mobility = checkmobility(current_color);
	// history[turn].expandmobility();
	return history[turn].mobility;
}

bool board::putdisc(const uint64 pos){
	uint64 flip = getflipdiscs(pos, current_color);

	if (flip == 0) {
		return false;		//incorrect input;
	}

	//打つ前の状態を保存する。
	history[turn].data[white] = data[white];
	history[turn].data[black] = data[black];
	history[turn].put_color = current_color;
	// history[turn].pass = false;

	//打った後の状態を計算する。
	data[current_color] ^= flip | pos;
	data[1-current_color] ^= flip;

	current_color = 1-current_color;
	turn++;
	//高速に実行するためcheckmobility、expandmobilityは別の場所で必要になり次第呼び出す。
	//葉ではないノードではcheckmobilityを計算する必要があるが、葉ノードでは計算する必要がないためこのようにしている。
	//葉ではないノードでcheckmobilityを計算する時は、checkcurrentmobilityを呼び出す。
	checkcurrentmobility();
	return true;
}

bool board::pass(){
	//高速化のため、パス可能かどうかはpass()の実行前に判定することとする。putdisc()と返り値を揃えるためにtrueを返すようにしている。
	// if (checkmobility(current_color) != 0) {
	// 	return false;
	// }

	// history[turn].flipped = 0;
	// history[turn].put_pos = 0;
	history[turn].put_color = current_color;
	history[turn].pass = true;

	current_color = 1-current_color;
	turn++;
	checkcurrentmobility();
	return true;
}

bool board::undo(){
	if (turn == 0) {
		return false;
	}
	history[turn].init();							//mobility_listに展開された手をクリアする。
	turn--;

	//pass
	if (history[turn].pass == true) {
		current_color = history[turn].put_color;
	}	else{
		//notpass
		data[white] = history[turn].data[white];
		data[black] = history[turn].data[black];
		current_color = history[turn].put_color;
	}
	return true;
}

uint64 board::checkmobility(const color c)const {
	namespace bm = bitmanipulations;

	//left
	uint64 	o = (data[c] & 0x7F7F7F7F7F7F7F7F) << 1,
			p = data[1-c] & 0xFEFEFEFEFEFEFEFE;
	uint64 left = (~(p | o) & (p + o)) & 0xFEFEFEFEFEFEFEFE;

	//right
	o = (bm::horizontal(data[c]) & 0x7F7F7F7F7F7F7F7F) << 1;
	p = bm::horizontal(data[1-c]) & 0xFEFEFEFEFEFEFEFE;
	uint64 right = bm::horizontal((~(p | o) & (p + o)) & 0xFEFEFEFEFEFEFEFE);

	//upper
	o = (bm::flip_diagonalA7H0(data[c]) & 0x7F7F7F7F7F7F7F7F) << 1;
	p = bm::flip_diagonalA7H0(data[1-c]) & 0xFEFEFEFEFEFEFEFE;
	uint64 upper = bm::flip_diagonalA7H0((~(p | o) & (p + o)) & 0xFEFEFEFEFEFEFEFE);

	//lower
	o = (bm::flip_diagonalA0H7(data[c]) & 0x7F7F7F7F7F7F7F7F) << 1;
	p = bm::flip_diagonalA0H7(data[1-c]) & 0xFEFEFEFEFEFEFEFE;
	uint64 lower = bm::flip_diagonalA0H7((~(p | o) & (p + o)) & 0xFEFEFEFEFEFEFEFE);

	//lowerleft
	o = (bm::shift_diagonalA7H0(data[c] & ~0x80808080808080FF)) << 1;
	p = bm::shift_diagonalA7H0(data[1-c] & ~0xFF01010101010101);
	uint64 lowerleft = bm::shift_diagonalA0H7(~(p | o) & (p + o)) & ~0xFF01010101010101;

	//upperleft
	o = (bm::shift_diagonalA0H7(data[c] & ~0xFF80808080808080)) << 1;
	p = bm::shift_diagonalA0H7(data[1-c] & ~ 0x01010101010101FF);
	uint64 upperleft = bm::shift_diagonalA7H0(~(p | o) & (p + o)) & ~0x01010101010101FF;

	//lowerright
	o = (bm::shift_diagonalA7H0(bm::horizontal(data[c] & ~0x01010101010101FF))) << 1;
	p = bm::shift_diagonalA7H0(bm::horizontal(data[1-c] & ~0xFF80808080808080));
	uint64 lowerright = bm::horizontal(bm::shift_diagonalA0H7(~(p | o) & (p + o))) & ~0xFF80808080808080;

	//upperright
	o = (bm::shift_diagonalA0H7(bm::horizontal(data[c] & ~0xFF01010101010101))) << 1;
	p = bm::shift_diagonalA0H7(bm::horizontal(data[1-c] & ~0x80808080808080FF));
	uint64 upperright = bm::horizontal(bm::shift_diagonalA7H0(~(p | o) & (p + o))) & ~0x80808080808080FF;

	uint64 blank = ~(data[white] | data[black]);
	return (left | right | upper | lower | upperleft | upperright | lowerleft | lowerright) & blank;
}

uint64 board::getblank(){
	return ~(data[white] | data[black]);
}

uint64 board::getflipdiscs(const uint64 put, const color c)const {
	namespace bm = bitmanipulations;
	//left
	uint64 	o = data[c],
			p = data[1-c],
			q = (put & 0x7F7F7F7F7F7F7F7F) << 1;
	uint64 left = p^((p + q) & ~q & ~o);
	// left *= ((left & ~0x7F7F7F7F7F7F7F7F) == 0) * (((p + q) & o) != 0);
	left &= (uint64)(0-(((left & ~0x7F7F7F7F7F7F7F7F) == 0) & (((p + q) & o) != 0)));

	//right
	o = bm::horizontal(data[c]);
	p = bm::horizontal(data[1-c]);
	q = (bm::horizontal(put & 0xFEFEFEFEFEFEFEFE)) << 1;
	uint64 right = bm::horizontal(p^((p + q) & ~q & ~o));
	// right *= ((right & ~0xFEFEFEFEFEFEFEFE) == 0) * (((p + q) & o) != 0);
	right &= (uint64)(0-(((right & ~0xFEFEFEFEFEFEFEFE) == 0) & (((p + q) & o) != 0)));

	//upper
	o = bm::flip_diagonalA7H0(data[c]);
	p = bm::flip_diagonalA7H0(data[1-c]);
	q = (bm::flip_diagonalA7H0(put & 0xFFFFFFFFFFFFFF00)) << 1;
	uint64 upper = bm::flip_diagonalA7H0(p^((p + q) & ~q & ~o));
	// upper *= ((upper & ~0xFFFFFFFFFFFFFF00) == 0) * (((p + q) & o) != 0);
	upper &= (uint64)(0-(((upper & ~0xFFFFFFFFFFFFFF00) == 0) & (((p + q) & o) != 0)));

	//lower
	o = bm::flip_diagonalA0H7(data[c]);
	p = bm::flip_diagonalA0H7(data[1-c]);
	q = (bm::flip_diagonalA0H7(put & 0x00FFFFFFFFFFFFFF)) << 1;
	uint64 lower = bm::flip_diagonalA0H7(p^((p + q) & ~q & ~o));
	// lower *= ((lower & ~0x00FFFFFFFFFFFFFF) == 0) * (((p + q) & o) != 0);
	lower &= (uint64)(0-(((lower & ~0x00FFFFFFFFFFFFFF) == 0) & (((p + q) & o) != 0)));

	//lowerleft
	o = bm::shift_diagonalA7H0(data[c]);
	p = bm::shift_diagonalA7H0(data[1-c]);
	q = (bm::shift_diagonalA7H0(put & ~0x80808080808080FF)) << 1;
	uint64 lowerleft = bm::shift_diagonalA0H7(p^((p + q) & ~q & ~o));
	// lowerleft *= ((lowerleft & 0x80808080808080FF) == 0) * (((p + q) & o) != 0);
	lowerleft &= (uint64)(0-(((lowerleft & 0x80808080808080FF) == 0) & (((p + q) & o) != 0)));

	//upperleft
	o = bm::shift_diagonalA0H7(data[c]);
	p = bm::shift_diagonalA0H7(data[1-c]);
	q = (bm::shift_diagonalA0H7(put & ~0xFF80808080808080)) << 1;
	uint64 upperleft = bm::shift_diagonalA7H0(p^((p + q) & ~q & ~o));
	// upperleft *= ((upperleft & 0xFF80808080808080) == 0) * (((p + q) & o) != 0);
	upperleft &= (uint64)(0-(((upperleft & 0xFF80808080808080) == 0) & (((p + q) & o) != 0)));

	//upperright
	o = bm::shift_diagonalA7H0(bm::horizontal(data[c]));
	p = bm::shift_diagonalA7H0(bm::horizontal(data[1-c]));
	q = (bm::shift_diagonalA7H0(bm::horizontal(put & ~0x1010101010101FF))) << 1;
	uint64 upperright = bm::horizontal(bm::shift_diagonalA0H7(p^((p + q) & ~q & ~o)));
	// upperright *= ((upperright & 0x1010101010101FF) == 0) * (((p + q) & o) != 0);
	upperright &= (uint64)(0-(((upperright & 0x1010101010101FF) == 0) & (((p + q) & o) != 0)));

	//lowerright
	o = bm::shift_diagonalA0H7(bm::horizontal(data[c]));
	p = bm::shift_diagonalA0H7(bm::horizontal(data[1-c]));
	q = (bm::shift_diagonalA0H7(bm::horizontal(put & ~0xFF01010101010101))) << 1;
	uint64 lowerright = bm::horizontal(bm::shift_diagonalA7H0(p^((p + q) & ~q & ~o)));
	// lowerright *= ((lowerright & 0xFF01010101010101) == 0) * (((p + q) & o) != 0);
	lowerright &= (uint64)(0-(((lowerright & 0xFF01010101010101) == 0) & (((p + q) & o) != 0)));

	return left | right | upper | lower | upperleft | upperright | lowerleft | lowerright;
}

uint64 board::getstablediscs(const color c)const {
	namespace bm = bitmanipulations;
	uint64 left = data[c] & 0xFF000000000000FF;
	uint64 s = left + 0x0100000000000001;
	left = (left ^ s) & ~s;

	uint64 right = bm::horizontal(data[c]) & 0xFF000000000000FF;
	s = right + 0x0100000000000001;
	right = bm::horizontal((right ^ s) & ~s);

	uint64 upper = bm::flip_diagonalA0H7(data[c]) & 0xFF000000000000FF;
	s = upper + 0x0100000000000001;
	upper = bm::flip_diagonalA0H7((upper ^ s) & ~s);

	uint64 lower = bm::flip_diagonalA7H0(data[c]) & 0xFF000000000000FF;
	s = lower + 0x0100000000000001;
	lower = bm::flip_diagonalA7H0((lower ^ s) & ~s);

	return left | right | upper | lower;
}

uint64 board::getcornerdiscs(const color c)const {
	return data[c] & 0x8100000000000018;
}

void board::setCUI() {
	char x,y;
	color c;
	std::cin >> x >> y;
	std::cin >> c;
	set(coordinateToIdx(x,y), c);
}

void board::showCUI()const {
	uint64 mobility = history[turn].mobility;
	std::cout << "+-0-+-1-+-2-+-3-+-4-+-5-+-6-+-7-+" << std::endl;
	char al = '0';
	for (int i = 0; i < 8; i++) {
		std::cout << al;
		al++;
		for (int j = 0; j < 8; j++) {
			switch (check((uint64)1 << ((i*8)+j))) {
				case white:
					std::cout << " o |";
					break;
				case black:
					std::cout << " * |";
					break;
				case empty:
					if ((mobility & (uint64)1<<(i*8+j)) != 0) {
						std::cout << " . |";
					}else{
						std::cout << "   |";
					}
					break;
			}
		}
		std::cout << std::endl;
		std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	}
	std::cout << std::endl;
}

uint64 board::coordinateToIdx(const char x, const char y)const {
	//x: a~h, ancii: 97 ~ 104
	//y: 0~7, ancii: 48 ~ 55
	int i = x-97;
	int j = y-48;
	return (uint64)1 << ((i*8)+j);
}

unsigned int board::gethash()const {
	unsigned int uintmax = 0-1;
	unsigned int white_hash = (unsigned int)(data[white]&intmax) ^ (unsigned int)(data[white]>>32);
	unsigned int black_hash = (unsigned int)(data[black]&intmax) ^ (unsigned int)(data[black]>>32);
	return white_hash ^ black_hash;
}
