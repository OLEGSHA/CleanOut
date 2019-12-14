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

#ifndef BALL_H_
#define BALL_H_

#include "../common.h"


const Velocity DEFAULT_BALL_VELOCITY = 10.0f;
const Velocity BALL_ACCELERATION_PER_SECOND = 0.2f;
const LevelCoord DEFAULT_BALL_RADIUS = 0.5f / 2;

class Ball {
private:
	LevelPoint position;
	Velocity velocity = DEFAULT_BALL_VELOCITY;
	VelocityVector velocity_vector = {0, DEFAULT_BALL_VELOCITY};
	LevelCoord radius = DEFAULT_BALL_RADIUS;

	bool is_held = false;

	void bounce(
			Velocity& velocity,
			LevelCoord& coord,
			LevelPoint position,
			bool positive, LevelCoord border, Game* game
	);

public:

	Ball(LevelPoint position = {0, 0});

	void render();
	void move(Game& level, Time frame_length);

	LevelPoint& get_position() {
		return position;
	}

	Velocity get_velocity() const;
	Velocity get_velocity_x() const;
	Velocity get_velocity_y() const;

	void set_velocity(Velocity x, Velocity y);
	void turn_velocity(Velocity x, Velocity y);

	void bounce_x(bool positive, LevelCoord border, Game* game);
	void bounce_y(bool positive, LevelCoord border, Game* game);

	void bounce_x(bool positive, LevelCoord border, Game& game) {
		bounce_x(positive, border, &game);
	}

	void bounce_y(bool positive, LevelCoord border, Game& game) {
		bounce_y(positive, border, &game);
	}

	void accelerate(Velocity delta);

	LevelCoord get_radius() {
		return radius;
	}

	void set_radius(unsigned int new_radius) {
		radius = new_radius;
	}

	bool get_is_held() { return is_held; }
	void hold();
	void release();
};


#endif /* BALL_H_ */
