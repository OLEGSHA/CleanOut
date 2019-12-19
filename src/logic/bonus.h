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
#include "../graphics/design.h"


const LevelCoord BONUS_RADIUS = 1.0f / 2 / 1.5f;

class Bonus : public Collideable {
private:
	Color color;
	bool good;

protected:
	virtual void apply(Game&) = 0;

	virtual void on_collide_with_platform(Game&) override;
	virtual void on_collide_with_level_floor(Game&) override;

public:
	Bonus(LevelPoint, VelocityVector, Color, bool good);
	virtual ~Bonus() {}

	virtual void render() override;
	virtual void tick(Game&, Time frame_length) override;

	Color get_color() {
		return color;
	}

	bool is_good() {
		return good;
	}
};

class SimpleBonus : public Bonus {
public:
	using GameAction = std::function<void(Game&)>;

private:
	GameAction action;

protected:
	virtual void apply(Game&) override;

public:
	SimpleBonus(LevelPoint, VelocityVector, Color, bool good, GameAction);
	virtual ~SimpleBonus() {}

};

using BonusCreator = std::function<Bonus*(LevelPoint, VelocityVector)>;
void register_bonus_type(BonusCreator creator, float weight);
void register_simple_bonus_type(
		Color, bool good, SimpleBonus::GameAction,
		float weight
);

Bonus* create_random_bonus(LevelPoint pos);


#endif /* BONUS_H_ */
