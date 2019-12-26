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
const Velocity PLAFORM_RADIUS_CHANGE_SPEED_PER_UNIT_DIFFERENCE = 1.0f;

Platform::Platform() :
	size(DEFAULT_PLATFORM_SIZE),
	position(0),
	desired_position(0),
	velocity(0),
	visual({0, 0}),
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
	position = desired_position = game.level->get_width() / 2;
	velocity = 0;
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

/*
 * Visual
 */

const Velocity VELOCITY_PER_UNIT_IMPULSE = 0.1;
void Platform::bounce(Ball& ball) {
	visual.velocity = -VELOCITY_PER_UNIT_IMPULSE * ball.get_impulse();
	visual.height = 0;
}

const VelocityVector
		DECELERATION_PER_UNIT_POSITION_PER_SECOND = {500, 30},
		DRAG_PER_UNIT_VELOCITY_PER_SECOND = {/*25*/30, 5};

void Platform::tick_visual(Game&, Time frame_length) {
	visual.height += visual.velocity * frame_length;

	visual.velocity +=
			DECELERATION_PER_UNIT_POSITION_PER_SECOND.y *
				visual.height * (-1) * frame_length
			+
			DRAG_PER_UNIT_VELOCITY_PER_SECOND.y *
				visual.velocity * (-1) * frame_length;
}

void Platform::move(Game& game, Time frame_length) {
	Velocity max_velocity = DEFAULT_BALL_VELOCITY; // Not any slower
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

	desired_position = force_in_range(
			size/2,
			desired_position + vel * frame_length,
			game.level->get_width() - size/2
	);

	position += velocity * frame_length;

	velocity +=
			DECELERATION_PER_UNIT_POSITION_PER_SECOND.x *
				(position - desired_position) * (-1) * frame_length
			+
			DRAG_PER_UNIT_VELOCITY_PER_SECOND.x *
				velocity * (-1) * frame_length;
}

void Platform::tick(Game& game, Time frame_length) {
	if (size != desired_size) {
		bool is_growing = size < desired_size;

		size += PLAFORM_RADIUS_CHANGE_SPEED_PER_UNIT_DIFFERENCE
				* (desired_size - size) * frame_length;

		// Do not overshoot
		if ((size < desired_size) != is_growing) {
			size = desired_size;
		}
	}

	move(game, frame_length);
	tick_visual(game, frame_length);
}
