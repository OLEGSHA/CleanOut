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
#include "collideable.h"


const Velocity DEFAULT_BALL_VELOCITY = 10.0f;

class Ball : public Collideable {
private:
	bool is_held = false;
	Time invinciblity = 0.0f;

	LevelCoord desired_radius;

protected:
	virtual void check_collisions(Game&) override;
	virtual void collide_with_level(Game&);

	virtual void on_collide_with_platform(Game&) override;
	virtual void on_collide_with_level_wall(Game&) override;
	virtual void on_collide_with_level_ceiling(Game&) override;
	virtual void on_collide_with_level_floor(Game&) override;

public:

	Ball(
			LevelPoint position = {0, 0},
			VelocityVector velocity = {0, DEFAULT_BALL_VELOCITY}
	);

	virtual void render() override;
	virtual void tick(Game&, Time frame_length) override;

	void accelerate(Velocity delta);

	void set_radius(LevelCoord);
	void set_radius_animated(LevelCoord);

	bool is_invincible() {
		return invinciblity > 0;
	}

	void add_invincibility(Time time);

	virtual void create_collision_sprite(Game&);

	bool get_is_held() { return is_held; }
	void hold();
	void release();
};


#endif /* BALL_H_ */
