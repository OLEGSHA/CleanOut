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

#include "sprites.h"

#include "../logic/logic.h"


void Sprite::render(Game& game, Time now) {
	if (is_dead()) return;

	if (start_time < 0) {
		start_time = now;
	}

	do_render(game, now - start_time);
}

FloorCollisionSprite::FloorCollisionSprite(Ball& reference) :
		Sprite(reference.get_position())
{
	start_pos = position;
	radius = reference.get_radius();
	velocity = { +reference.get_velocity_x(), +reference.get_velocity_y() };
}

BonusCollectedSprite::BonusCollectedSprite(Bonus& reference) :
		Sprite(reference.get_position()),
		good(reference.is_good())
{}

BonusFloorCollisionSprite::BonusFloorCollisionSprite(Bonus& reference) :
		Sprite(reference.get_position()),
		good(reference.is_good())
{
	start_pos = position;
	velocity = { +reference.get_velocity_x(), +reference.get_velocity_y() };
}
