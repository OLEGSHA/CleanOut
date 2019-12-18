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

#ifndef SPRITES_H_
#define SPRITES_H_

#include "../common.h"


class Sprite {
private:
	bool dead = false;
	Time start_time = -1;

protected:
	LevelPoint position;

	void die() {
		dead = true;
	}

	virtual void do_render(Game& game, Time time) = 0;

public:
	Sprite(LevelPoint pos) :
	position(pos) {}

	virtual ~Sprite() {}

	bool is_dead() {
		return dead;
	}

	void render(Game& game, Time now);
};

class CollisionSprite : public Sprite {
public:
	CollisionSprite(LevelPoint pos) :
	Sprite(pos) {}

	void do_render(Game& game, Time time) override;
};

class BrickBrokenSprite : public Sprite {
private:
	bool powerful;
public:
	BrickBrokenSprite(LevelPoint pos, bool powerful) :
	Sprite(pos), powerful(powerful) {}

	void do_render(Game& game, Time time) override;
};

class FloorCollisionSprite : public Sprite {
private:
	LevelPoint start_pos;
	LevelCoord radius;
	VelocityVector velocity;

public:
	FloorCollisionSprite(Ball& reference);

	void do_render(Game& game, Time time) override;
};

class BrickExplosionSprite : public Sprite {
public:
	BrickExplosionSprite(LevelPoint pos) :
	Sprite(pos) {}

	void do_render(Game& game, Time time) override;
};

class BonusCollectedSprite : public Sprite {
public:
	BonusCollectedSprite(LevelPoint pos) :
	Sprite(pos) {}

	void do_render(Game& game, Time time) override;
};

class BonusFloorCollisionSprite : public Sprite {
private:
	LevelPoint start_pos;
	VelocityVector velocity;

public:
	BonusFloorCollisionSprite(Bonus& reference);

	void do_render(Game& game, Time time) override;
};


#endif /* SPRITES_H_ */
