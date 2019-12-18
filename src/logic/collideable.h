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

#ifndef COLLIDEABLE_H_
#define COLLIDEABLE_H_

#include "../common.h"


class Collideable {
private:
	void bounce(
			Velocity& velocity,
			LevelCoord& coord,
			bool positive, LevelCoord border
	);

protected:
	LevelPoint position;
	LevelCoord radius;
	Velocity velocity;
	VelocityVector velocity_vector;

	bool dead = false;

	virtual void check_collisions(Game&);
	void collide_with_platform(Game&);
	void collide_with_bounds(Game&);

	virtual void on_collide_with_platform(Game&) = 0;
	virtual void on_collide_with_level_wall(Game&);
	virtual void on_collide_with_level_ceiling(Game&);
	virtual void on_collide_with_level_floor(Game&) = 0;

	virtual void die();

public:
	Collideable(
			LevelPoint position,
			VelocityVector velocity,
			LevelCoord radius
	);
	virtual ~Collideable() {}

	LevelPoint& get_position() {
		return position;
	}

	LevelCoord get_radius() {
		return radius;
	}

	bool is_dead() {
		return dead;
	}

	virtual void render() = 0;
	virtual void tick(Game&, Time frame_length);

	Velocity get_velocity() const;
	Velocity get_velocity_x() const;
	Velocity get_velocity_y() const;

	void set_velocity(Velocity x, Velocity y);
	void turn_velocity(Velocity x, Velocity y);

	void bounce_x(bool positive, LevelCoord border);
	void bounce_y(bool positive, LevelCoord border);
};


#endif /* COLLIDEABLE_H_ */
