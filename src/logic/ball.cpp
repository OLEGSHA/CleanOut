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


const Velocity BALL_ACCELERATION_PER_SECOND = 0.2f;
const LevelCoord DEFAULT_BALL_RADIUS = 0.5f / 2;

Ball::Ball(LevelPoint position, VelocityVector velocity) :
	Collideable(position, velocity, DEFAULT_BALL_RADIUS)
{}

void Ball::accelerate(Velocity delta) {
	velocity += delta;
	velocity_vector.x += delta * velocity_vector.x / velocity;
	velocity_vector.y += delta * velocity_vector.y / velocity;
}


void Ball::tick(Game& game, Time frame_length) {
	if (is_held) {
		position.x = force_in_range(
					game.platform.get_position() - game.platform.get_size()/4,
					position.x,
					game.platform.get_position() + game.platform.get_size()/4
		);

		position.y = PLATFORM_HEIGHT + radius;

		return;
	}

	Collideable::tick(game, frame_length);

	accelerate(BALL_ACCELERATION_PER_SECOND * frame_length);

	if (is_invincible()) {
		invinciblity -= frame_length;

		if (invinciblity < 0) {
			invinciblity = 0;
		}
	}
}

void Ball::check_collisions(Game& game) {
	Collideable::check_collisions(game);
	collide_with_level(game);
}

void Ball::collide_with_level(Game& game) {
	LevelBlock min =
	{
			static_cast<LevelBlockCoord>(floorf(position.x - radius)),
			static_cast<LevelBlockCoord>(floorf(position.y - radius)),
	};

	LevelBlock max =
	{
		static_cast<LevelBlockCoord>(floorf(position.x + radius)),
		static_cast<LevelBlockCoord>(floorf(position.y + radius)),
	};

	for (LevelBlockCoord x = min.x; x <= max.x; ++x) {
		for (LevelBlock block = {x, min.y}; block.y <= max.y; ++block.y) {
			game.level->collide(game, block, *this);
		}
	}
}

void Ball::create_collision_sprite(Game& game) {
	game.add_sprite(new CollisionSprite(position));
}

void Ball::on_collide_with_platform(Game& game) {
	Velocity vx = velocity * 0.9 *

			(position.x - game.platform.get_position())
			/
			(game.platform.get_size() / 2.0f + radius)

	;

	set_velocity(vx, sqrt(sqr(velocity) - sqr(vx)));

	create_collision_sprite(game);
}

void Ball::on_collide_with_level_wall(Game& game) {
	Collideable::on_collide_with_level_wall(game);
	create_collision_sprite(game);
}

void Ball::on_collide_with_level_ceiling(Game& game) {
	Collideable::on_collide_with_level_ceiling(game);
	create_collision_sprite(game);
}

void Ball::on_collide_with_level_floor(Game& game) {
	if (is_invincible()) {
		bounce_y(true, radius);
		create_collision_sprite(game);
		return;
	}

	game.add_sprite(new FloorCollisionSprite(*this));
	die();
}

void Ball::hold() {
	if (is_held) {
		return;
	}

	is_held = true;
	position.y = PLATFORM_HEIGHT + radius;
	velocity_vector = {0, -velocity};
}

void Ball::release() {
	if (!is_held) {
		return;
	}

	is_held = false;
}

void Ball::add_invincibility(Time time) {
	invinciblity += time;
}
