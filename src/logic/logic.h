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

#ifndef LOGIC_H_
#define LOGIC_H_

#include "../common.h"

#include "bricks.h"
#include "../graphics/sprites.h"
#include "level.h"
#include "platform.h"
#include "ball.h"


class Attempt {
private:
	Lives lives;
	Score score;
	LevelId next_level;

public:
	Attempt();

	Score get_score();
	void increase_score(Score score);

	Lives get_lives();
	void add_lives(Lives mod);
	void remove_lives(Lives mod);

	Game* start_next_level();
};

Attempt* get_current_attempt();
void start_attempt();
void end_attempt();

class Game {
private:
	vector<Ball*> balls;

public:
	Level *level;

	Platform platform;

	GameState state;

	list<Sprite*> sprites;

	Game(Level*);
	~Game();

	void add_sprite(Sprite *sprite);

	void add_ball(Ball*);
	void add_held_ball();
	void reset_balls();
	void remove_ball(Ball*);

	const vector<Ball*>& get_balls() const {
		return balls;
	}
};

void tick(Game& game, Time frame_length);

void terminate_logic();


#endif /* LOGIC_H_ */
