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

#include "collideable.h"

#include "logic.h"


Collideable::Collideable(
		LevelPoint position,
		VelocityVector velocity,
		LevelCoord radius) :
	position(position),
	radius(radius)
{
	set_velocity(velocity.x, velocity.y);
}

Velocity Collideable::get_velocity() const {
	return velocity;
}

Velocity Collideable::get_velocity_x() const {
	return velocity_vector.x;
}

Velocity Collideable::get_velocity_y() const {
	return velocity_vector.y;
}

void Collideable::set_velocity(Velocity x, Velocity y) {
	turn_velocity(x, y);
	velocity = sqrt(sqr(x) + sqr(y));
}

void Collideable::turn_velocity(Velocity x, Velocity y) {
	velocity_vector.x = x;
	velocity_vector.y = y;
}



void Collideable::bounce(
		Velocity& velocity,
		LevelCoord& coord,
		bool positive, LevelCoord border
) {
	if (velocity == 0 || (positive == (velocity > 0))) {
		return;
	}

	velocity *= -1;

	const LevelCoord margin = radius + 0.01;
	coord = border + (positive ? +margin : -margin);
}

void Collideable::bounce_x(bool positive, LevelCoord border) {
	bounce(
			velocity_vector.x,
			position.x,

			positive, border
	);
}

void Collideable::bounce_y(bool positive, LevelCoord border) {
	bounce(
			velocity_vector.y,
			position.y,

			positive, border
	);
}



void Collideable::tick(Game& game, Time frame_length) {
	position += velocity_vector * frame_length;
	check_collisions(game);
}

void Collideable::check_collisions(Game& game) {
	collide_with_platform(game);
	collide_with_bounds(game);
}

void Collideable::collide_with_platform(Game& game) {
	if (velocity_vector.y < 0
		&& (position.y - radius) <= PLATFORM_HEIGHT
		&& is_in_range(
			game.platform.get_min_x() - radius,
			position.x,
			game.platform.get_max_x() + radius
		)
	) {
		on_collide_with_platform(game);
	}
}

void Collideable::collide_with_bounds(Game& game) {
	if (position.y >= game.level->get_height() - radius) {
		on_collide_with_level_ceiling(game);
	}

	if (!is_in_range(
			radius,
			position.x,
			game.level->get_width() - radius)) {
		on_collide_with_level_wall(game);
	}

	if (position.y - radius <= 0) {
		on_collide_with_level_floor(game);
	}
}

void Collideable::on_collide_with_level_ceiling(Game& game) {
	bounce_y(false, game.level->get_height());
}

void Collideable::on_collide_with_level_wall(Game& game) {
	if (position.x <= radius) {
		bounce_x(true, 0);
	} else if (position.x >= game.level->get_width() - radius) {
		bounce_x(false, game.level->get_width());
	}
}

void Collideable::die() {
	dead = true;
}
