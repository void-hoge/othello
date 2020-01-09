#pragma once

#include <immintrin.h>

using uint64 = unsigned long long;
const int intmax = 2147483647;

//   0 1 2 3 4 5 6 7
// a . . . . . . . .
// b . . . . . . . .
// c . . . . . . . .
// d . . . . . . . .
// e . . . . . . . .
// f . . . . . . . .
// g . . . . . . . .
// h . . . . . . . .

//   0 1 2 3 4 5 6 7
// 0 . . . . . . . .
// 1 . . . . . . . .
// 2 . . . . . . . .
// 3 . . . . . . . .
// 4 . . . . . . . .
// 5 . . . . . . . .
// 6 . . . . . . . .
// 7 . . . . . . . .


namespace bitmanipulations
{
inline uint64 delta_swap(uint64 bits, uint64 mask, int delta) {
	uint64 x = (bits ^ ((uint64)bits >> delta)) & mask;
	return bits ^ x ^ ((uint64)x << delta);
}

inline uint64 horizontal(uint64 t){
	t = delta_swap(t, 0x0F0F0F0F0F0F0F0F, 4);
	t = delta_swap(t, 0x3333333333333333, 2);
	return delta_swap(t, 0x5555555555555555, 1);
}

inline uint64 vertical(uint64 t){
	t = delta_swap(t, 0x00000000FFFFFFFF, 32);
	t = delta_swap(t, 0x0000FFFF0000FFFF, 16);
	return delta_swap(t, 0x00FF00FF00FF00FF, 8);
}

inline uint64 flip_diagonalA0H7(uint64 bits){
	bits = delta_swap(bits, 0x00000000F0F0F0F0, 28);
	bits = delta_swap(bits, 0x0000CCCC0000CCCC, 14);
	return delta_swap(bits, 0x00AA00AA00AA00AA,  7);
}

inline uint64 flip_diagonalA7H0(uint64 bits){
	bits = delta_swap(bits, 0x000000000F0F0F0F, 36);
	bits = delta_swap(bits, 0x0000333300003333, 18);
	return delta_swap(bits, 0x0055005500550055, 9);
}

inline uint64 shift_diagonalA7H0(uint64 n){
	n = (n & ~0xF0F0F0F0F0F0F0F0) | ((n & 0xF0F0F0F0F0F0F0F0) << 32) | ((n & 0xF0F0F0F0F0F0F0F0) >> 32);
	n = (n & ~0xCCCCCCCCCCCCCCCC) | ((n & 0xCCCCCCCCCCCCCCCC) << 16) | ((n & 0xCCCCCCCCCCCCCCCC) >> 48);
	return (n & ~0xAAAAAAAAAAAAAAAA) | ((n & 0xAAAAAAAAAAAAAAAA) << 8) | ((n & 0xAAAAAAAAAAAAAAAA) >> 56);
}

inline uint64 shift_diagonalA0H7(uint64 n){
	n = (n & ~0xF0F0F0F0F0F0F0F0) | ((n & 0xF0F0F0F0F0F0F0F0) >> 32) | ((n & 0xF0F0F0F0F0F0F0F0) << 32);
	n = (n & ~0xCCCCCCCCCCCCCCCC) | ((n & 0xCCCCCCCCCCCCCCCC) >> 16) | ((n & 0xCCCCCCCCCCCCCCCC) << 48);
	return (n & ~0xAAAAAAAAAAAAAAAA) | ((n & 0xAAAAAAAAAAAAAAAA) >> 8) | ((n & 0xAAAAAAAAAAAAAAAA) << 56);
}

// const int bits_count_table[256] = {
// 	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
// 	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
// 	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
// 	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
// 	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
// 	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
// 	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
// 	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
// 	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
// 	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
// 	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
// 	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
// 	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
// 	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
// 	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
// 	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
// };

inline int bit_count(uint64 bits) {
	return _popcnt64(bits);
}

// inline int bit_count(uint64 bits){
//	int num  = 0;
//
//	for (int i=0 ; i < sizeof(bits) ; i++ ) {
//		num += bits_count_table[(bits >> (8*i)) & 0xff ];
//	}
//
//	return num;
// }

// inline int bit_count(uint64 bits){
//     bits = (bits & 0x5555555555555555) + ((bits >> 1) & 0x5555555555555555);
//     bits = (bits & 0x3333333333333333) + ((bits >> 2) & 0x3333333333333333);
//     bits = (bits & 0x0f0f0f0f0f0f0f0f) + ((bits >> 4) & 0x0f0f0f0f0f0f0f0f);
//     bits = (bits & 0x00ff00ff00ff00ff) + ((bits >> 8) & 0x00ff00ff00ff00ff);
//     bits = (bits & 0x0000ffff0000ffff) + ((bits >> 16) & 0x0000ffff0000ffff);
//     return (int)((bits & 0x00000000ffffffff) + ((bits >> 32) & 0x00000000ffffffff));
// }

}	//namespace bitmanipulations
