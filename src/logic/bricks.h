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

#ifndef BRICKS_H_
#define BRICKS_H_

#include "../common.h"


class Brick {
private:
	bool needs_destruction = true;

protected:
	void set_does_not_need_destruction() {
		needs_destruction = false;
	}

	virtual void do_destroy(Game&, LevelBlock);

public:
	virtual ~Brick() {};

	bool get_needs_destruction() {
		return needs_destruction;
	}

	void destroy(Game&, LevelBlock);

	virtual bool on_collision(Game&, LevelBlock);
	virtual void render(Game&, LevelBlock) = 0;
	virtual Score get_reward() const {
		return 1;
	}
};

class SimpleBrick : public Brick {
public:
	void render(Game&, LevelBlock) override;
};

class SturdyBrick : public Brick {
private:
	const unsigned int max_health;
	unsigned int health;
public:
	SturdyBrick(unsigned int extra_hits) :
	max_health(extra_hits), health(extra_hits) {}

	virtual bool on_collision(Game&, LevelBlock) override;
	virtual void render(Game&, LevelBlock) override;
	virtual Score get_reward() const override {
		return max_health;
	}
};

class ExplosiveBrick : public Brick {
protected:
	virtual void do_destroy(Game&, LevelBlock) override;
public:
	virtual void render(Game&, LevelBlock) override;
	virtual Score get_reward() const override {
		return 5;
	}
};

class ExtraBallBrick : public Brick {
protected:
	virtual void do_destroy(Game&, LevelBlock) override;
public:
	virtual void render(Game&, LevelBlock) override;
	virtual Score get_reward() const override {
		return 5;
	}
};


#endif /* BRICKS_H_ */
