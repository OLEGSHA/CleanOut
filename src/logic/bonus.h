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

#ifndef BONUS_H_
#define BONUS_H_

#include "../common.h"
#include "collideable.h"


const LevelCoord BONUS_RADIUS = 1.0f / 2 / 1.5f;

class Bonus : public Collideable {
protected:
	virtual void apply(Game&) = 0;

	virtual void on_collide_with_platform(Game&) override;
	virtual void on_collide_with_level_floor(Game&) override;

public:
	Bonus(LevelPoint, VelocityVector);
	virtual ~Bonus() {}

	virtual void tick(Game&, Time frame_length) override;
};

using BonusCreator = std::function<Bonus*(LevelPoint, VelocityVector)>;
void register_bonus_type(BonusCreator creator, float weight);

Bonus* create_random_bonus(LevelPoint pos);



class ExtraLifeBonus : public Bonus {
protected:
	virtual void apply(Game&) override;

public:
	ExtraLifeBonus(LevelPoint pos, VelocityVector vel) :
		Bonus(pos, vel) {}
	virtual ~ExtraLifeBonus() {}

	virtual void render() override;
};

class LongerPlatformBonus : public Bonus {
protected:
	virtual void apply(Game&) override;

public:
	LongerPlatformBonus(LevelPoint pos, VelocityVector vel) :
		Bonus(pos, vel) {}
	virtual ~LongerPlatformBonus() {}

	virtual void render() override;
};

class ShorterPlatformBonus : public Bonus {
protected:
	virtual void apply(Game&) override;

public:
	ShorterPlatformBonus(LevelPoint pos, VelocityVector vel) :
		Bonus(pos, vel) {}
	virtual ~ShorterPlatformBonus() {}

	virtual void render() override;
};

class InvincibilityBonus : public Bonus {
protected:
	virtual void apply(Game&) override;

public:
	InvincibilityBonus(LevelPoint pos, VelocityVector vel) :
		Bonus(pos, vel) {}
	virtual ~InvincibilityBonus() {}

	virtual void render() override;
};

class LargerBallBonus : public Bonus {
protected:
	virtual void apply(Game&) override;

public:
	LargerBallBonus(LevelPoint pos, VelocityVector vel) :
		Bonus(pos, vel) {}
	virtual ~LargerBallBonus() {}

	virtual void render() override;
};

class SmallerBallBonus : public Bonus {
protected:
	virtual void apply(Game&) override;

public:
	SmallerBallBonus(LevelPoint pos, VelocityVector vel) :
		Bonus(pos, vel) {}
	virtual ~SmallerBallBonus() {}

	virtual void render() override;
};


#endif /* BONUS_H_ */
