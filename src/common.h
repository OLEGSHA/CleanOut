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
#include <functional>


extern const char *TITLE;
extern const char *VERSION;
extern const char *COPYRIGHT;
extern const char *FULL_NAME;

const float PI = 3.1415926f;

using Action = std::function<void(void)>;


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
void inline delete_and_clear(std::vector<T*>& col) {
	for (T *ptr : col) {
		delete ptr;
	}

	col.clear();
}

/*
 * Deletes all pointers in the list and clears it.
 */

template< class T >
void inline delete_and_clear(std::list<T*>& col) {
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

template< class Coord >
struct AbstractPoint {
	using Self = AbstractPoint<Coord>;

	Coord x, y;

	friend bool operator==(const Self& lhs, const Self& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	friend bool operator!=(const Self& lhs, const Self& rhs) {
		return !(lhs == rhs);
	};

	Self& operator+=(const Self& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	friend Self operator+(Self lhs, const Self& rhs) {
		lhs += rhs;
		return lhs;
	}

	Self& operator*=(Coord rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	friend Self operator*(Self lhs, Coord rhs) {
		lhs *= rhs;
		return lhs;
	}

	template< class OtherCoord >
	operator AbstractPoint<OtherCoord>() const {
		return {
			static_cast<OtherCoord>(x),
			static_cast<OtherCoord>(y)
		};
	}

	template< class OtherCoord >
	AbstractPoint<OtherCoord> add(OtherCoord x_mod, OtherCoord y_mod) {
		return {
			static_cast<OtherCoord>(x) + x_mod,
			static_cast<OtherCoord>(y) + y_mod
		};
	}
};

/*
 * A coordinate in the screen coordinate system.
 */
using ScreenCoord = float;

/*
 * A point in the screen coordinate system.
 */
using ScreenPoint = AbstractPoint<ScreenCoord>;

/*
 * Time in seconds.
 */
using Time = float;

/*
 * An integer coordinate in level coordinate system. Used to represent blocks.
 */
using LevelBlockCoord = signed int;
using LevelBlock = AbstractPoint<LevelBlockCoord>;

using LevelCoord = ScreenCoord;
using LevelPoint = AbstractPoint<LevelCoord>;

using Velocity = ScreenCoord;
using VelocityVector = AbstractPoint<Velocity>;

using Score = unsigned int;
using Lives = unsigned int;

enum GameState {
	RUNNING, PAUSED, VICTORY, DEFEAT
};


class Brick;
class Level;
class Platform;
class Ball;
class Game;
class Bonus;


#endif /* COMMON_H_ */
