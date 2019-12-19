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


const float PLATFORM_SIZE_BONUS_FACTOR = 1.5f;
const float BALL_SIZE_BONUS_FACTOR = 1.5f;
const Time INVINSIBILITY_BONUS = 5.0f;

void setup_logic() {
	register_simple_bonus_type(
			Color(0xEE0000), true,
			[](Game&) {
				get_current_attempt()->add_lives(1);
			},
			0.5f
	);

	const Color PLATFORM_SIZE_COLOR(0x3333EE);

	register_simple_bonus_type(
			PLATFORM_SIZE_COLOR, true,
			[](Game& game) {
				game.platform.set_size_animated(
						game.platform.get_size() * PLATFORM_SIZE_BONUS_FACTOR
				);
			},
			1.0f
	);

	register_simple_bonus_type(
			PLATFORM_SIZE_COLOR, false,
			[](Game& game) {
				game.platform.set_size_animated(
						game.platform.get_size() / PLATFORM_SIZE_BONUS_FACTOR
				);
			},
			1.0f
	);

	register_simple_bonus_type(
			Color(0xEEEEEE), true,
			[](Game& game) {
				for (Ball *ball : game.get_balls()) {
					ball->add_invincibility(INVINSIBILITY_BONUS);
				}
			},
			0.5f
	);

	const Color BALL_SIZE_COLOR(0xEEEE33);

	register_simple_bonus_type(
			BALL_SIZE_COLOR, true,
			[](Game& game) {
				for (Ball *ball : game.get_balls()) {
					ball->set_radius_animated(
							ball->get_radius() * BALL_SIZE_BONUS_FACTOR
					);
				}
			},
			1.0f
	);

	register_simple_bonus_type(
			BALL_SIZE_COLOR, false,
			[](Game& game) {
				for (Ball *ball : game.get_balls()) {
					ball->set_radius_animated(
							ball->get_radius() / BALL_SIZE_BONUS_FACTOR
					);
				}
			},
			1.0f
	);
}

void terminate_logic() {
	end_attempt();
}

extern const LevelId max_level;
Level* _create_level(LevelId id);

std::vector<Ball*> __tick__balls_copy;
std::list<Bonus*> __tick__bonuses_copy;

void tick(Game& game, Time frame_length) {
	if (game.state != RUNNING) {
		return;
	}

	Attempt *attempt = get_current_attempt();

	Level& level = *(game.level);
	game.platform.tick(game, frame_length);

	__tick__balls_copy = game.get_balls();

	for (Ball *ball : __tick__balls_copy) {
		ball->tick(game, frame_length);

		if (ball->is_dead()) {
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

	__tick__bonuses_copy = game.get_bonuses();

	for (Bonus *bonus : __tick__bonuses_copy) {
		bonus->tick(game, frame_length);

		if (bonus->is_dead()) {
			game.remove_bonus(bonus);
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
	delete_and_clear(bonuses);

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

	platform.reset(*this);
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

void Game::add_bonus(Bonus* bonus) {
	bonuses.push_back(bonus);
}

void Game::remove_bonus(Bonus* bonus) {
	for (auto b = bonuses.cbegin(); b != bonuses.cend(); ++b) {
		if (*b == bonus) {
			bonuses.erase(b);
			// b is now invalid
			break;
		}
	}

	delete bonus;
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
		std::cerr << "Invalid level id " << id << ", assuming 3" << std::endl; // @suppress("No break at end of case")
		// fall-through
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
