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

#include "logic.h"
#include "../workflow.h"

#include "level_builder.h"


void terminate_logic() {
	end_attempt();
}

extern const LevelId max_level;
Level* _create_level(LevelId id);

void collide_ball_with_platform(Ball& ball, Platform& platform, Game& game) {
	if (!ball.get_is_held() && ball.get_velocity_y() <= 0
		&& (ball.get_position().y - ball.get_radius()) <= PLATFORM_HEIGHT
		&& is_in_range(
			platform.get_min_x() - ball.get_radius(),
			ball.get_position().x,
			platform.get_max_x() + ball.get_radius()
		)
	) {

		Velocity v = ball.get_velocity();
		Velocity vx = v * 0.9 *

				(ball.get_position().x - platform.get_position())
				/
				(platform.get_size() / 2.0f + ball.get_radius())

		;

		ball.set_velocity(vx, sqrt(v*v - vx*vx));

		game.add_sprite(new CollisionSprite(ball.get_position()));
	}
}

void collide_ball_with_level(Ball& ball, Game& game) {
	LevelPoint pos = ball.get_position();
	LevelCoord r = ball.get_radius();

	LevelBlock min =
	{
			static_cast<LevelBlockCoord>(floorf(pos.x - r)),
			static_cast<LevelBlockCoord>(floorf(pos.y - r))
	};

	LevelBlock max =
	{
			static_cast<LevelBlockCoord>(floorf(pos.x + r)),
			static_cast<LevelBlockCoord>(floorf(pos.y + r))
	};

	for (LevelBlockCoord x = min.x; x <= max.x; ++x) {
		for (LevelBlock block = {x, min.y}; block.y <= max.y; ++block.y) {
			game.level->collide(game, block, ball);
		}
	}
}

bool collide_ball_with_floor(Ball& ball, Game& game) {
	if (ball.get_position().y - ball.get_radius() <= 0) {
		game.add_sprite(new FloorCollisionSprite(ball));

		return true;
	}

	return false;
}

vector<Ball*> __tick__balls_copy;

void tick(Game& game, Time frame_length) {
	if (game.state != RUNNING) {
		return;
	}

	Attempt *attempt = get_current_attempt();

	Level& level = *(game.level);
	game.platform.move(game, frame_length);

	__tick__balls_copy = game.get_balls();

	for (Ball *ball : __tick__balls_copy) {
		ball->move(game, frame_length);

		collide_ball_with_platform(*ball, game.platform, game);
		collide_ball_with_level(*ball, game);

		if (collide_ball_with_floor(*ball, game)) {
			if (game.get_balls().size() == 1) {

				attempt->remove_lives(1);

				if (attempt->get_lives() == 0) {
					game.state = DEFEAT;
					show_results_menu(game);
				} else {
					game.reset_balls();
				}
				break;

			} else {
				game.remove_ball(ball);
			}
		}
	}

	level.delete_pending_bricks();

	if (level.is_level_cleared()) {
		game.state = VICTORY;
		attempt->increase_score(20 * attempt->get_lives());
		show_results_menu(game);
	}
}

/*
 * Attepmt
 */

Attempt *current_attempt = nullptr;

Attempt* get_current_attempt() {
	return current_attempt;
}

void start_attempt() {
	if (current_attempt != nullptr) {
		std::cerr << "Attempted to start attempt while attempt already present"
				<< std::endl;
		return;
	}

	current_attempt = new Attempt();
}

void end_attempt() {
	if (current_attempt == nullptr) {
		return;
	}

	delete current_attempt;
	current_attempt = nullptr;
}

Attempt::Attempt() :
		lives(10),
		score(0),
		next_level(0)
{}

Lives Attempt::get_lives() {
	return lives;
}

void Attempt::add_lives(Lives mod) {
	lives += mod;
}

void Attempt::remove_lives(Lives mod) {
	if (mod >= lives) {
		lives = 0;
	} else {
		lives -= mod;
	}
}

Score Attempt::get_score() {
	return score;
}

void Attempt::increase_score(Score increment) {
	score += increment;
}

Game* Attempt::start_next_level() {
	Level* level = _create_level(next_level);

	if (next_level == max_level) {
		next_level = 0;
	} else {
		next_level++;
	}

	Game* result = new Game(level);
	result->reset_balls();
	return result;
}

/*
 * Game
 */

Game::Game(Level *level) :
	balls(),
	level(level),
	platform(),
	state(RUNNING),
	sprites()
{}

Game::~Game() {
	delete_and_clear(balls);
	delete_and_clear(sprites);

	delete level;
}

void Game::add_sprite(Sprite* sprite) {
	sprites.push_back(sprite);
}

void Game::add_ball(Ball* ball) {
	balls.push_back(ball);
}

void Game::add_held_ball() {
	Ball *ball = new Ball();
	ball->hold();
	ball->get_position().x = platform.get_position() + 0.3;

	add_ball(ball);
}

void Game::reset_balls() {
	delete_and_clear(balls);

	platform.set_position(level->get_width() / 2);
	add_held_ball();
}

void Game::remove_ball(Ball *ball) {
	for (auto b = balls.cbegin(); b != balls.cend(); ++b) {
		if (*b == ball) {
			balls.erase(b);
			// b is now invalid
			break;
		}
	}

	delete ball;
}

/*
 * Levels
 */

template< class WALLS, class CENTER >
void draw_tile(Level *level, LevelBlock pos, WALLS walls, CENTER center) {
	for (LevelBlockCoord x = pos.x - 1; x <= pos.x + 1; ++x) {
		for (LevelBlock block = {x, pos.y - 1}; block.y <= pos.y + 1; ++block.y)
		{
			level->set_brick(block,
					(pos.x == block.x && pos.y == block.y)
						? static_cast<Brick*>(new CENTER(center))
						: static_cast<Brick*>(new WALLS(walls))
			);
		}
	}
}

const LevelId max_level = 3;

Level* _create_level(LevelId id) {
	Level *level;

	switch (id) {

	case 0: {

		level = new Level(id, 10, 10, 5);
		LevelBuilder level_builder(*level);

		level_builder.draw_line(HORIZONTAL, 9, 1, 8, SimpleBrick());
		level_builder.draw_line(HORIZONTAL, 8, 2, 7, SimpleBrick());
		level_builder.draw_line(HORIZONTAL, 7, 3, 6, SimpleBrick());
		level_builder.draw_line(HORIZONTAL, 6, 2, 7, SimpleBrick());
		level_builder.draw_line(HORIZONTAL, 5, 1, 8, SimpleBrick());

		level->set_brick({4, 7}, new SturdyBrick(2));
		level->set_brick({5, 7}, new SturdyBrick(2));

	} break;

	case 1: {

		level = new Level(id, 15, 15, 10);
		LevelBuilder level_builder(*level);

		level_builder.draw_line(HORIZONTAL, 7, 1, 13, SturdyBrick(4));
		level_builder.draw_line(VERTICAL, 1, 8, 12, SturdyBrick(4));
		level_builder.draw_line(VERTICAL, 13, 8, 12, SturdyBrick(4));

		for (LevelBlockCoord y = 7 + 1; y < 13; ++y) {
			level_builder.draw_line(HORIZONTAL, y, 2, 12, SimpleBrick());
		}

	} break;

	case 2: {

		level = new Level(id, 20, 16, 10);
		LevelBuilder level_builder(*level);

		level_builder.draw_line(HORIZONTAL, 10, 2, 17, SturdyBrick(2));
		level_builder.draw_line(HORIZONTAL, 9, 1, 18, SimpleBrick());
		level_builder.draw_line(HORIZONTAL, 11, 2, 17, SturdyBrick(4));

		level->set_brick({3, 11}, new ExplosiveBrick());
		level->set_brick({16, 11}, new ExplosiveBrick());

	} break;

	default:
		std::cerr << "Invalid level id " << id << ", assuming 3" << std::endl;
		/* no break */
	case 3: {

		level = new Level(id, 20, 16, 10);
		LevelBuilder level_builder(*level);

		draw_tile(level, {3, 13}, SimpleBrick(), ExtraBallBrick());
		draw_tile(level, {5, 10}, SturdyBrick(3), ExplosiveBrick());
		draw_tile(level, {3, 7}, SimpleBrick(), ExtraBallBrick());

		draw_tile(level, {16, 13}, SimpleBrick(), ExtraBallBrick());
		draw_tile(level, {14, 10}, SturdyBrick(3), ExplosiveBrick());
		draw_tile(level, {16, 7}, SimpleBrick(), ExtraBallBrick());

		level_builder.draw_line(HORIZONTAL, 7, 5, 14, SimpleBrick());
		level_builder.draw_line(HORIZONTAL, 10, 7, 12, SturdyBrick(1));
		level_builder.draw_line(HORIZONTAL, 13, 5, 14, SturdyBrick(2));

	} break;

	}

	return level;
}
