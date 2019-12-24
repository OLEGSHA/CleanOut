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


//const Velocity BALL_ACCELERATION_PER_SECOND = 0.2f;
const Velocity BALL_ACCELERATION_PER_SECOND_PER_UNIT_MASS = 0.2f;
const LevelCoord DEFAULT_BALL_RADIUS = 0.5f / 2;

const Velocity BALL_RADIUS_CHANGE_SPEED = 0.25f;

Ball::Ball(LevelPoint position, VelocityVector velocity) :
	Collideable(position, velocity, DEFAULT_BALL_RADIUS),
	desired_radius(DEFAULT_BALL_RADIUS)
{}

void Ball::accelerate(Velocity delta) {
	velocity += delta;
	velocity_vector.x += delta * velocity_vector.x / velocity;
	velocity_vector.y += delta * velocity_vector.y / velocity;
}

const float SPHERE_VOLUME_COEFF = 4 * PI / 3;
// Make sure that the ball masses 1 by default
const float BALL_DENSITY =
		1.0f /
		(SPHERE_VOLUME_COEFF *
		DEFAULT_BALL_RADIUS * DEFAULT_BALL_RADIUS * DEFAULT_BALL_RADIUS);

Ball::Mass Ball::get_mass() const {
	return BALL_DENSITY * SPHERE_VOLUME_COEFF * radius * radius * radius;
}

Ball::Impulse Ball::get_impulse() const {
	return get_mass() * get_velocity();
}

void Ball::set_radius(LevelCoord new_radius) {
	radius = new_radius;
}

void Ball::set_radius_animated(LevelCoord new_radius) {
	desired_radius = new_radius;
}


void Ball::tick(Game& game, Time frame_length) {
	if (tick_held(game, frame_length)) return;
	tick_radius(game, frame_length);

	Collideable::tick(game, frame_length);

	accelerate(
			BALL_ACCELERATION_PER_SECOND_PER_UNIT_MASS *
			frame_length / get_mass()
	);

	tick_invincibility(game, frame_length);
}

bool Ball::tick_held(Game& game, Time) {
	if (is_held) {
		position.x = force_in_range(
					game.platform.get_position() - game.platform.get_size()/4,
					position.x,
					game.platform.get_position() + game.platform.get_size()/4
		);

		position.y = PLATFORM_HEIGHT + radius;

		return true;
	}

	return false;
}

void Ball::tick_radius(Game&, Time frame_length) {
	if (radius != desired_radius) {
		bool is_growing = radius < desired_radius;
		Mass old_mass = get_mass();

		radius += (is_growing
						? +BALL_RADIUS_CHANGE_SPEED
						: -BALL_RADIUS_CHANGE_SPEED)
				* frame_length;

		float ratio = old_mass / get_mass();

		set_velocity(
				get_velocity_x() * ratio,
				get_velocity_y() * ratio
		);

		// Do not overshoot
		if ((radius < desired_radius) != is_growing) {
			radius = desired_radius;
		}
	}
}

void Ball::tick_invincibility(Game&, Time frame_length) {
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
