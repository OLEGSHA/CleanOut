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

#include "platform.h"


#include "logic.h"


LevelCoord Platform::get_min_x() const {
	return position - size/2;
}

LevelCoord Platform::get_max_x() const {
	return position + size/2;
}

void Platform::set_movement(bool is_left, bool new_state, bool is_fast) {
	(is_left ? is_moving_left : is_moving_right) = new_state;
	set_movement_fast(is_fast);
}

void Platform::set_movement_fast(bool is_fast) {
	is_moving_fast = is_fast;
}

void Platform::move(Game& game, Time frame_length) {
	const Velocity velocity_value = is_moving_fast ? 20 : 10;

	int direction = 0;

	if (is_moving_left) direction--;
	if (is_moving_right) direction++;

	Velocity vel = direction * velocity_value;

	position = force_in_range(
			size/2,
			position + vel * frame_length,
			game.level->get_width() - size/2
	);
}
