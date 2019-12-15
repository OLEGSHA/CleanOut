/*
 * CleanOut the game
 * Copyright (C) 2019  Javapony
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef COMMON_H_
#define COMMON_H_


#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include <iomanip>

using std::vector;
using std::list;
using std::cerr;


extern const char *TITLE;
extern const char *VERSION;
extern const char *COPYRIGHT;
extern const char *FULL_NAME;

/*
 * Checks if min <= x <= max.
 */
template< class T >
bool inline is_in_range(T min, T x, T max) {
	return min <= x && x <= max;
}

/*
 * Returns min if x < min, max if x > max, x otherwise.
 */
template< class T >
T inline force_in_range(T min, T x, T max) {
	if (min > x) {
		return min;
	} else if (max < x) {
		return max;
	} else {
		return x;
	}
}

/*
 * Deletes all pointers in the vector and clears it.
 */

template< class T >
void inline delete_and_clear(vector<T*>& col) {
	for (T *ptr : col) {
		delete ptr;
	}

	col.clear();
}

/*
 * Deletes all pointers in the list and clears it.
 */

template< class T >
void inline delete_and_clear(list<T*>& col) {
	for (T *ptr : col) {
		delete ptr;
	}

	col.clear();
}

/*
 * Returns the argument squared.
 */
template < class T >
T inline sqr(T x) {
	return x * x;
}

/*
 * A coordinate in the screen coordinate system.
 */
typedef float ScreenCoord;

/*
 * A point in the screen coordinate system.
 */
struct ScreenPoint {
	ScreenCoord x, y;
};

/*
 * Time in seconds.
 */
typedef float Time;

/*
 * An integer coordinate in level coordinate system. Used to represent blocks.
 */
typedef signed int LevelBlockCoord;

struct LevelBlock {
	LevelBlockCoord x, y;

	operator ScreenPoint() const {
		return {static_cast<float>(x), static_cast<float>(y)};
	}
};

typedef float LevelCoord;
struct LevelPoint {
	LevelCoord x, y;

	operator ScreenPoint() const {
		return {x, y};
	}
};

typedef float Velocity;
struct VelocityVector {
	Velocity x, y;
};

typedef unsigned int Score;
typedef unsigned int Lives;

enum GameState {
	RUNNING, VICTORY, DEFEAT
};


class Brick;
class Level;
class Platform;
class Ball;
class Game;


#endif /* COMMON_H_ */
