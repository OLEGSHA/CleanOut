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


#include "bonus.h"

#include "logic.h"


/*
 * Bonus
 */


const Velocity BONUS_ACCELERATION_PER_SECOND = 15.0f;

Bonus::Bonus(LevelPoint position, VelocityVector velocity) :
		Collideable(position, velocity, BONUS_RADIUS)
{}

void Bonus::tick(Game& game, Time frame_length) {
	Collideable::tick(game, frame_length);
	set_velocity(
			velocity_vector.x,
			velocity_vector.y - BONUS_ACCELERATION_PER_SECOND * frame_length
	);
}

void Bonus::on_collide_with_platform(Game& game) {
	apply(game);
	game.add_sprite(new BonusCollectedSprite(position));
	die();
}

void Bonus::on_collide_with_level_floor(Game& game) {
	game.add_sprite(new BonusFloorCollisionSprite(*this));
	die();
}

/*
 * Bonus registry
 */

struct BonusRegistryEntry {
	BonusCreator creator;
	float weight;
};

std::vector<BonusRegistryEntry> bonus_registry;
float bonus_registry_total_weight = 0;

void register_bonus_type(BonusCreator creator, float weight) {
	bonus_registry.push_back({creator, weight});
	bonus_registry_total_weight += weight;
}

const Velocity BONUS_VELOCITY = 10.0f;

const float BONUS_CHANCE = 0.1f; // out of 1.0f

Bonus* create_random_bonus(LevelPoint pos) {
	if (generate_random_float() > BONUS_CHANCE) {
		return nullptr;
	}

	float target = generate_random_float() * bonus_registry_total_weight;

	auto itr = bonus_registry.cbegin();

	while (itr != bonus_registry.cend() && itr->weight < target) {
		target -= itr->weight;
		itr++;
	}

	BonusCreator creator =
			(itr != bonus_registry.cend())
					? itr->creator
					: bonus_registry.back().creator;

	float angle = generate_random_float() * PI/2 - PI/4;

	return creator(pos, {
			BONUS_VELOCITY * sinf(angle),
			BONUS_VELOCITY * cosf(angle)
	});
}



void ExtraLifeBonus::apply(Game&) {
	get_current_attempt()->add_lives(1);
}

const float PLATFORM_SIZE_BONUS_FACTOR = 1.5f;

void LongerPlatformBonus::apply(Game& game) {
	game.platform.set_size(game.platform.get_size() * PLATFORM_SIZE_BONUS_FACTOR);
}

void ShorterPlatformBonus::apply(Game& game) {
	game.platform.set_size(game.platform.get_size() / PLATFORM_SIZE_BONUS_FACTOR);
}

const Time INVINSIBILITY_BONUS = 5.0f;

void InvincibilityBonus::apply(Game& game) {
	for (Ball *ball : game.get_balls()) {
		ball->add_invincibility(INVINSIBILITY_BONUS);
	}
}

const float BALL_SIZE_BONUS_FACTOR = 1.5f;

void LargerBallBonus::apply(Game& game) {
	for (Ball *ball : game.get_balls()) {
		ball->set_radius(ball->get_radius() * BALL_SIZE_BONUS_FACTOR);
	}
}

void SmallerBallBonus::apply(Game& game) {
	for (Ball *ball : game.get_balls()) {
		ball->set_radius(ball->get_radius() / BALL_SIZE_BONUS_FACTOR);
	}
}
