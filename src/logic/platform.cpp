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


const LevelCoord DEFAULT_PLATFORM_SIZE = 3;
const Velocity PLAFORM_RADIUS_CHANGE_SPEED = 1.0f;

Platform::Platform() :
	size(DEFAULT_PLATFORM_SIZE),
	position(0),
	desired_size(size)
{}

Platform::~Platform() {}

void Platform::set_position(LevelCoord new_position) {
	position = new_position;
}

void Platform::set_size(LevelCoord new_size) {
	size = new_size;
}

void Platform::set_size_animated(LevelCoord new_size) {
	desired_size = new_size;
}

void Platform::reset(Game& game) {
	size = desired_size = DEFAULT_PLATFORM_SIZE;
	position = game.level->get_width() / 2;
}

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

void Platform::tick(Game& game, Time frame_length) {
	if (size != desired_size) {
		bool is_growing = size < desired_size;

		size += (is_growing
						? +PLAFORM_RADIUS_CHANGE_SPEED
						: -PLAFORM_RADIUS_CHANGE_SPEED)
				* frame_length;

		// Do not overshoot
		if ((size < desired_size) != is_growing) {
			size = desired_size;
		}
	}

	Velocity max_velocity = 0;
	for (Ball *ball : game.get_balls()) {
		Velocity v = ball->get_velocity();
		if (max_velocity < v) {
			max_velocity = v;
		}
	}

	const Velocity velocity_value = (is_moving_fast ? 2 : 1) * max_velocity;

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
