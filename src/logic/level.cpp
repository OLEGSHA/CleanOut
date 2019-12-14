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

#include "level.h"

#include "logic.h"


Level::Level(
		LevelId id,
		LevelBlockCoord width, LevelBlockCoord height,
		LevelBlockCoord field_height
) :
		id(id),
		width(width),
		height(height),
		field_height(field_height)
{
	size_t length = width * field_height;
	field = new Brick*[length];
	for (size_t i = 0; i < length; ++i) {
		field[i] = NULL;
	}
}

Level::~Level() {
	delete_pending_bricks();

	size_t length = width * field_height;
	for (size_t i = 0; i < length; ++i) {
		if (field[i] != NULL) delete field[i];
	}
	delete[] field;
}


size_t Level::get_field_index(LevelBlock pos) const {
	return static_cast<size_t>
			((pos.y - (height - field_height)) * width + pos.x);
}

bool Level::check_pos(LevelBlock pos) const {
	pos.y -= height - field_height;

	return !(
			is_in_range(0, pos.x, width - 1)
			&&
			is_in_range(0, pos.y, field_height - 1)
	);
}

Brick* Level::get_brick(LevelBlock pos) const {
	if (check_pos(pos)) {
		return NULL;
	}

	return field[get_field_index(pos)];
}

void Level::destroy_brick(LevelBlock pos) {
	if (check_pos(pos)) {
		return;
	}

	size_t index = get_field_index(pos);
	Brick* brick = field[index];
	field[index] = NULL;

	if (brick != NULL) {
		bricks_to_delete.push_back(brick);

		if (brick->get_needs_destruction()) {
			bricks_to_destroy--;
		}
	}
}

void Level::set_brick(LevelBlock pos, Brick* brick) {
	if (brick == NULL) {
		return;
	}

	if (check_pos(pos)) {
		delete brick;
		return;
	}

	destroy_brick(pos);
	field[get_field_index(pos)] = brick;

	if (brick->get_needs_destruction()) {
		bricks_to_destroy++;
	}
}

bool Level::is_level_cleared() const {
	return bricks_to_destroy == 0;
}


void Level::delete_pending_bricks() {
	for (Brick* brick : bricks_to_delete) {
		delete brick;
	}

	bricks_to_delete.clear();
}

void Level::render(Game& game) {
	for (LevelBlockCoord x = 0; x < width; ++x) {
		for (LevelBlock block = {x, 0}; block.y < height; ++block.y) {
			Brick* brick = get_brick(block);
			if (brick != NULL) {
				brick->render(game, block);
			}
		}
	}
}



void Level::collide(Game& context, LevelBlock pos, Ball& ball) {
	Brick* brick = get_brick(pos);

	if (brick == NULL) {
		return;
	}

	LevelPoint pos_float = {static_cast<float>(pos.x), static_cast<float>(pos.y)};
	LevelPoint& ball_pos = ball.get_position();
	LevelCoord ball_radius = ball.get_radius();

	// Collides with a horizontal border
	bool collides_vertically =
		is_in_range(pos_float.x, ball_pos.x, pos_float.x + 1)
		&&
		is_in_range(
				pos_float.y - ball_radius,
				ball_pos.y,
				pos_float.y + 1 + ball_radius
		);

	// Collides with a vertical border
	bool collides_horizontally =
		is_in_range(pos_float.y, ball_pos.y, pos_float.y + 1)
		&&
		is_in_range(
				pos_float.x - ball_radius,
				ball_pos.x,
				pos_float.x + 1 + ball_radius
		);

	// Collides with a corner
	//   Pick nearest corner and check whether the distance between the corner
	//   and the ball centre is small enough for a collision
	//   Comparing squared distances because sqrt() is expensive
	if (!(collides_vertically || collides_horizontally)) {
		collides_vertically = collides_horizontally =

			sqr(std::min(
				std::abs(ball_pos.x - pos_float.x),
				std::abs(ball_pos.x - (pos_float.x + 1))
			))
			+
			sqr(std::min(
				std::abs(ball_pos.y - pos_float.y),
				std::abs(ball_pos.y - (pos_float.y + 1))
			))

			<= sqr(ball_radius);
	}

	bool should_bounce_positive_x = ball_pos.x > pos_float.x + 0.5f;
	bool should_bounce_positive_y = ball_pos.y > pos_float.y + 0.5f;

	if (collides_vertically || collides_horizontally) {

		bool should_bounce = brick->on_collision(context, pos);

		if (should_bounce) {
			if (collides_horizontally) {
				ball.bounce_x(
						should_bounce_positive_x,
						should_bounce_positive_x
							? pos_float.x + 1
							: pos_float.x,
						collides_vertically ? NULL : &context
				);
			} else { // collides_vertically
				ball.bounce_y(
						should_bounce_positive_y,
						should_bounce_positive_y
							? pos_float.y + 1
							: pos_float.y,
						context
				);
			}
		}
	}
}
