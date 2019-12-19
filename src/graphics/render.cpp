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

#include <cmath>
#include <sstream>

#include "graphics.h"
#include "../logic/logic.h"


const ScreenCoord BRICK_MARGIN = 0.05;

void draw_brick(LevelBlock pos) {
	draw_rectangle({
		pos.x + BRICK_MARGIN,
		pos.y + BRICK_MARGIN
	}, 1 - 2*BRICK_MARGIN, 1 - 2*BRICK_MARGIN);
}

void fill_brick(LevelBlock pos) {
	fill_rectangle({
		pos.x + BRICK_MARGIN,
		pos.y + BRICK_MARGIN
	}, 1 - 2*BRICK_MARGIN, 1 - 2*BRICK_MARGIN);
}

void Platform::render() {
	const ScreenCoord radius = 0.3,
			inner_radius = radius * 0.8;

	set_color(Design::FILL);
	fill_rounded_rectangle(
			{
					get_min_x(),
					0
			},
			{
					get_max_x(),
					PLATFORM_HEIGHT
			},
			radius
	);

	set_color(Design::OUTLINE);
	draw_rounded_rectangle(
			{
					get_min_x(),
					0
			},
			{
					get_max_x(),
					PLATFORM_HEIGHT
			},
			radius
	);

	draw_arc(
			{
					get_min_x() + radius,
					PLATFORM_HEIGHT - radius
			},
			radius * 0.8,
			-PI/2, 0
	);

	draw_line(
			{
					get_min_x() + radius,
					PLATFORM_HEIGHT - radius + inner_radius
			},
			{
					get_position(),
					PLATFORM_HEIGHT - radius + inner_radius
			}
	);
}

void Ball::render() {
	set_color(Design::FILL);
	fill_circle(position, radius);

	if (is_invincible()) {
		float fraction = (invinciblity > 1.0f) ? 1 : invinciblity / 1.0f;

		set_color(0xAA, 0xAA, 0xAA);
		fill_circle(position, 0.8f * fraction * radius);
	}

	set_color(Design::OUTLINE);
	draw_circle(position, radius);
	draw_arc(position, radius * 0.8, 0, -PI/2);
}



void SimpleBrick::render(Game&, LevelBlock pos) {
	set_color(0x33, 0x55, 0x33);
	fill_brick(pos);

	set_color(0xAA, 0xEE, 0xAA);
	draw_brick(pos);
}

void SturdyBrick::render(Game&, LevelBlock pos) {
	if (health == 0) {
		set_color(0x33, 0x55, 0x33);
		fill_brick(pos);

		set_color(0xAA, 0xEE, 0xAA);
		draw_brick(pos);
		return;
	}

	set_color(0x55, 0x55, 0x55);
	fill_brick(pos);

	ScreenCoord border = ((1 - 2*BRICK_MARGIN) / 2) *
			(1 - (max_health+1.0f - health) / (max_health+1));

	set_color(0x33, 0x55, 0x33);
	fill_rectangle(
		{
			pos.x + BRICK_MARGIN + border,
			pos.y + BRICK_MARGIN + border
		}, {
			pos.x + 1 - BRICK_MARGIN - border,
			pos.y + 1 - BRICK_MARGIN - border
		}
	);

	set_color(0xEE, 0xEE, 0xEE);
	draw_brick(pos);

	draw_rectangle(
		{
			pos.x + BRICK_MARGIN + border,
			pos.y + BRICK_MARGIN + border
		}, {
			pos.x + 1 - BRICK_MARGIN - border,
			pos.y + 1 - BRICK_MARGIN - border
		}
	);
}

void ExplosiveBrick::render(Game&, LevelBlock pos) {
	set_color(0x88, 0x88, 0x55);
	fill_brick(pos);

	set_color(0x55, 0x55, 0x33);
	fill_polygon({pos.x + 0.5f, pos.y + 0.5f}, 0.2, 3);

	set_color(0xFF, 0xFF, 0x55);
	draw_brick(pos);
	draw_polygon({pos.x + 0.5f, pos.y + 0.5f}, 0.2, 3);
}

void ExtraBallBrick::render(Game&, LevelBlock pos) {
	set_color(0x55, 0x55, 0x88);
	fill_brick(pos);

	set_color(0x33, 0x33, 0x55);
	fill_circle({pos.x + 0.5f, pos.y + 0.5f}, 0.2);

	set_color(0x88, 0x88, 0xFF);
	draw_brick(pos);
	draw_circle({pos.x + 0.5f, pos.y + 0.5f}, 0.2);
}



void CollisionSprite::do_render(Game&, Time time) {
	const Time lifetime = 0.2f;

	if (time > lifetime) {
		die();
		return;
	}

	float fraction = time / lifetime;
	set_color((1 - fraction) * 0.25f, 0xFF, 0xFF, 0xFF);
	fill_circle(position, 0.75f * fraction);
}

void BrickBrokenSprite::do_render(Game&, Time time) {
	const Time lifetime = 0.2f;

	if (time >= lifetime) {
		die();
		return;
	}

	float fraction = 1 - time / lifetime;

	if (powerful) {
		LevelCoord size = (1 - fraction) * 2.0f + 1.0f;
		set_color(fraction * 1.0f, 0xFF, 0xCC, 0xAA);
		fill_rectangle_centered(position, size, size);

		set_color(fraction * 1.0f, 0xFF, 0xFF, 0xFF);
	} else {
		set_color(fraction * 1.0f, 0xAA, 0xAA, 0xAA);
	}

	fill_rectangle_centered(position, fraction * 1.0f, fraction * 1.0f);
}

void FloorCollisionSprite::do_render(Game&, Time time) {
	const Time lifetime = 0.25f;

	if (time > lifetime) {
		die();
		return;
	}

	Time frame_length = get_frame_length();

	position.x += velocity.x * frame_length;
	position.y += velocity.y * frame_length;

	float fraction = time / lifetime;

	set_color((1 - fraction) * 1.0f, 0xFF, 0xCC, 0xAA);
	fill_circle(start_pos, radius + 5*radius * fraction);

	set_color((1 - fraction) * 1.0f, 0xAA, 0xAA, 0xEE);
	draw_circle(position, radius);
}

void BrickExplosionSprite::do_render(Game&, Time time) {
	const Time lifetime = 0.3f;
	const LevelCoord start_size = 3;
	const LevelCoord max_size = 3.5f;

	if (time >= lifetime) {
		die();
		return;
	}

	float fraction =
			max_size - sqr(
					time *
					(sqrt(max_size) + sqrt(max_size - start_size))
					/ lifetime

					- sqrt(max_size - start_size)
			)
	;

	set_color(time / lifetime * 1.0f, 0xFF, 0xCC, 0xAA);
	fill_rectangle_centered(position, fraction * 1.0f, fraction * 1.0f);
}

void BonusCollectedSprite::do_render(Game&, Time time) {
	const Time lifetime = 0.15f;
	const LevelCoord start_size = BONUS_RADIUS;
	const LevelCoord max_size = BONUS_RADIUS * 2;
	unsigned int vertices = good ? 4 : 5;

	if (time >= lifetime) {
		die();
		return;
	}

	LevelCoord size = time / lifetime * (max_size - start_size) + start_size;
	float alpha = (1 - time / lifetime) * 1.0f;

	if (good) {
		set_color(alpha, 0x55, 0xCC, 0x55);
	} else {
		set_color(alpha, 0xCC, 0x55, 0x55);
	}

	fill_polygon(position, size * 1.5f, vertices);

	if (good) {
		set_color(alpha, 0xAA, 0xEE, 0xAA);
	} else {
		set_color(alpha, 0xEE, 0xAA, 0xAA);
	}
	fill_polygon(position, size, vertices);
}

void BonusFloorCollisionSprite::do_render(Game&, Time time) {
	const Time lifetime = 0.1f;
	unsigned int vertices = good ? 4 : 5;

	position += velocity * get_frame_length();

	if (time >= lifetime) {
		die();
		return;
	}

	float fraction = 1 - time / lifetime;

	set_color(fraction * 1.0f, 0xAA, 0xAA, 0xAA);
	fill_polygon(position, fraction * BONUS_RADIUS, vertices);
}



void Bonus::render() {
	unsigned int vertices = good ? 4 : 5;

	set_color(color);
	fill_polygon(position, radius, vertices);

	set_color(Design::OUTLINE);
	draw_polygon(position, radius, vertices);
}
