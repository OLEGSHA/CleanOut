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
}

void Brick::do_destroy(Game& game, LevelBlock pos) {
	game.add_sprite(new BrickBrokenSprite(
			{pos.x + 0.5f, pos.y + 0.5f}, false
	));
}

bool Brick::on_collision(Game& game, LevelBlock pos) {
	destroy(game, pos);
	return true;
}

bool SturdyBrick::on_collision(Game& game, LevelBlock pos) {
	if (health == 0) {
		destroy(game, pos);
	} else {
		health--;
	}

	return true;
}

void ExplosiveBrick::do_destroy(Game& game, LevelBlock pos) {
	for (LevelBlockCoord x = pos.x - 1; x <= pos.x + 1; ++x) {
		for (LevelBlock block = {x, pos.y - 1}; block.y <= pos.y + 1; ++block.y) {
			Brick* brick = game.level->get_brick(block);
			if (brick != NULL) {
				brick->destroy(game, block);
			}
		}
	}

	game.add_sprite(new BrickExplosionSprite({
		pos.x + 0.5f, pos.y + 0.5f
	}));
}

void ExtraBallBrick::do_destroy(Game& game, LevelBlock pos) {
	game.add_sprite(new BrickBrokenSprite(
			{pos.x + 0.5f, pos.y + 0.5f}, false
	));

	game.add_ball(new Ball({pos.x + 0.5f, pos.y + 0.5f}));
}
