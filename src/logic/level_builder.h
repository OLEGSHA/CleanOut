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

#ifndef LEVEL_BUILDER_H_
#define LEVEL_BUILDER_H_

#include "../common.h"

#include "level.h"


enum LevelBuilderLineDirection {
	HORIZONTAL, VERTICAL
};

class LevelBuilder {
private:
	Level& level;

public:
	LevelBuilder(Level& level) : level(level) {}
	~LevelBuilder() {}

	Level& get_level() { return level; }

	template < class T >
	void draw_line(
			LevelBuilderLineDirection direction,
			LevelBlockCoord i,
			LevelBlockCoord j1, LevelBlockCoord j2,
			T templ
	) {
		for (LevelBlockCoord j = j1; j <= j2; ++j) {
			LevelBlock block =
				(direction == HORIZONTAL)
					? LevelBlock {j, i}
					: LevelBlock {i, j}
			;

			level.set_brick(block, new T(templ));
		}
	}

	template < class T >
	void fill_rectangle(LevelBlock min, LevelBlock max, T templ) {
		for (LevelBlockCoord x = min.x; x <= max.x; ++x) {
			for (LevelBlock block = {x, min.y}; block.y <= max.y; ++block.y) {
				level.set_brick(block, new T(templ));
			}
		}
	}

};


#endif /* LEVEL_BUILDER_H_ */
