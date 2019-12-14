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

#include "ball.h"

#include "logic.h"


Ball::Ball(LevelPoint position) :
	position(position)
{}

Velocity Ball::get_velocity() const {
	return velocity;
}

Velocity Ball::get_velocity_x() const {
	return velocity_vector.x;
}

Velocity Ball::get_velocity_y() const {
	return velocity_vector.y;
}

void Ball::set_velocity(Velocity x, Velocity y) {
	turn_velocity(x, y);
	velocity = sqrt(sqr(x) + sqr(y));
}

void Ball::turn_velocity(Velocity x, Velocity y) {
	velocity_vector.x = x;
	velocity_vector.y = y;
}


void Ball::bounce(
		Velocity& velocity,
		LevelCoord& coord,
		LevelPoint position,
		bool positive, LevelCoord border, Game* game
) {
	if (velocity == 0 || (positive == (velocity > 0))) {
		return;
	}

	velocity *= -1;

	LevelCoord margin = radius + 0.01;
	coord = border + (positive ? +margin : -margin);

	if (game != NULL) game->add_sprite(new CollisionSprite(position));
}

void Ball::bounce_x(bool positive, LevelCoord border, Game* game) {
	bounce(
			velocity_vector.x,
			position.x,
			position,

			positive, border, game
	);
}

void Ball::bounce_y(bool positive, LevelCoord border, Game* game) {
	bounce(
			velocity_vector.y,
			position.y,
			position,

			positive, border, game
	);
}

void Ball::accelerate(Velocity delta) {
	velocity += delta;
	velocity_vector.x += delta * velocity_vector.x/velocity;
	velocity_vector.y += delta * velocity_vector.y/velocity;
}


void Ball::move(Game& game, Time frame_length) {
	if (is_held) {
		position.x = force_in_range(
					game.platform.get_position() - game.platform.get_size()/4,
					position.x,
					game.platform.get_position() + game.platform.get_size()/4
		);

		return;
	}

	position.x += velocity_vector.x * frame_length;
	position.y += velocity_vector.y * frame_length;

	if (position.y >= game.level->get_height() - radius) {
		bounce_y(false, game.level->get_height(), game);
	}

	if (position.x <= radius) {
		bounce_x(true, 0, game);
	} else if (position.x >= game.level->get_width() - radius) {
		bounce_x(false, game.level->get_width(), game);
	}

	accelerate(BALL_ACCELERATION_PER_SECOND * frame_length);
}

void Ball::hold() {
	if (is_held) {
		return;
	}

	is_held = true;
	position.y = PLATFORM_HEIGHT + radius;
	velocity_vector = {0, 0};
}

void Ball::release() {
	if (!is_held) {
		return;
	}

	is_held = false;
}
