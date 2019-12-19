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

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "../common.h"


const LevelCoord PLATFORM_HEIGHT = 0.75f;

class Platform {
private:
	LevelCoord size;
	LevelCoord position;

	LevelCoord desired_size;

	bool is_moving_left = false, is_moving_right = false;
	bool is_moving_fast = false;

public:

	Platform();
	~Platform();

	void render();
	void tick(Game& level, Time frame_length);
	void reset(Game&);

	LevelCoord get_position() const { return position; }
	LevelCoord get_size()     const { return size; }

	void set_position(LevelCoord);
	void set_size(LevelCoord);
	void set_size_animated(LevelCoord);

	LevelCoord get_min_x() const;
	LevelCoord get_max_x() const;

	void set_movement(bool is_left, bool new_state, bool is_fast);
	void set_movement_fast(bool is_fast);
};


#endif /* PLATFORM_H_ */
