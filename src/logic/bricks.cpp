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

#include "bricks.h"

#include "logic.h"


void Brick::destroy(Game& game, LevelBlock pos) {
	game.level->destroy_brick(pos);
	do_destroy(game, pos);
	get_current_attempt()->increase_score(get_reward());

	Bonus *bonus = create_random_bonus(static_cast<LevelPoint>(pos));
	if (bonus != nullptr) {
		game.add_bonus(bonus);
	}
}

void Brick::do_destroy(Game& game, LevelBlock pos) {
	game.add_sprite(new BrickBrokenSprite(
			pos.add(0.5f, 0.5f), false
	));
}

bool Brick::on_collision(Game& game, LevelBlock pos, Ball&) {
	destroy(game, pos);
	return true;
}

const float HEALTH_DECREASE_PER_UNIT_IMPULSE =
		0.75f // default descrease
		/
		(1.0f * DEFAULT_BALL_VELOCITY); // default impulse

bool SturdyBrick::on_collision(Game& game, LevelBlock pos, Ball& ball) {
	Health decrease = ball.get_impulse() * HEALTH_DECREASE_PER_UNIT_IMPULSE;

	if (health < decrease) {
		destroy(game, pos);
	} else {
		health -= decrease;
	}

	return true;
}

void ExplosiveBrick::do_destroy(Game& game, LevelBlock pos) {
	for (LevelBlockCoord x = pos.x - 1; x <= pos.x + 1; ++x) {
		for (LevelBlock block = {x, pos.y - 1}; block.y <= pos.y + 1; ++block.y) {
			Brick* brick = game.level->get_brick(block);
			if (brick != nullptr) {
				brick->destroy(game, block);
			}
		}
	}

	game.add_sprite(new BrickExplosionSprite({
		pos.add(0.5f, 0.5f)
	}));
}

void ExtraBallBrick::do_destroy(Game& game, LevelBlock pos) {
	game.add_sprite(new BrickBrokenSprite(
			pos.add(0.5f, 0.5f), false
	));

	Ball *ball = new Ball(pos.add(0.5f, 0.5f));

	float angle = generate_random_float() * 2*PI;
	ball->set_velocity(
			ball->get_velocity() * sin(angle),
			ball->get_velocity() * cos(angle)
	);

	game.add_ball(ball);
}
