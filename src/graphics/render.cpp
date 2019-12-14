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
//	set_color(0x33, 0x33, 0x55);
	set_color(Design::FILL);
	fill_circle(position, radius);
//	set_color(0xAA, 0xAA, 0xEE);
	set_color(Design::OUTLINE);
	draw_circle(position, radius);
	draw_arc(position, radius * 0.8, 0, -PI/2);
}



void SimpleBrick::render(Game&, LevelBlock pos) {
	set_color(0x33, 0x55, 0x33);
	fill_rectangle(pos, 1, 1);

	set_color(0xAA, 0xEE, 0xAA);
	draw_rectangle(pos, 1, 1);
}

void SturdyBrick::render(Game&, LevelBlock pos) {
	set_color(0x38, 0x38, 0x38);
	fill_rectangle(pos, 1, 1);

	ScreenCoord border = (max_health - health + 1) * (0.5f) / (max_health + 1);

	set_color(0x55, 0x55, 0x55);
	fill_rectangle(
		{
			pos.x + border,
			pos.y + border
		}, {
			pos.x + 1 - border,
			pos.y + 1 - border
		}
	);

	set_color(0xAA, 0xAA, 0xAA);
	draw_rectangle(pos, 1, 1);

	draw_rectangle(
		{
			pos.x + border,
			pos.y + border
		}, {
			pos.x + 1 - border,
			pos.y + 1 - border
		}
	);
}

void ExplosiveBrick::render(Game&, LevelBlock pos) {
	set_color(0x88, 0x88, 0x55);
	fill_rectangle(pos, 1, 1);

	set_color(0x55, 0x55, 0x33);
	fill_polygon({pos.x + 0.5f, pos.y + 0.5f}, 0.2, 3);

	set_color(0xFF, 0xFF, 0x55);
	draw_rectangle(pos, 1, 1);
	draw_polygon({pos.x + 0.5f, pos.y + 0.5f}, 0.2, 3);
}

void ExtraBallBrick::render(Game&, LevelBlock pos) {
	set_color(0x55, 0x55, 0x88);
	fill_rectangle(pos, 1, 1);

	set_color(0x33, 0x33, 0x55);
	fill_circle({pos.x + 0.5f, pos.y + 0.5f}, 0.2);

	set_color(0x88, 0x88, 0xFF);
	draw_rectangle(pos, 1, 1);
	draw_circle({pos.x + 0.5f, pos.y + 0.5f}, 0.2);
}



void CollisionSprite::do_render(Game& game, Time time) {
	const Time lifetime = 0.2f;

	if (time > lifetime) {
		die();
	}

	float fraction = time / lifetime;
	set_color((1 - fraction) * 0.25f, 0xFF, 0xFF, 0xFF);
	fill_circle(position, 0.75f * fraction);
}

void BrickBrokenSprite::do_render(Game& game, Time time) {
	const Time lifetime = 0.2f;

	if (time >= lifetime) {
		die();
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

void FloorCollisionSprite::do_render(Game& game, Time time) {
	const Time lifetime = 0.25f;

	if (time > lifetime) {
		die();
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

void BrickExplosionSprite::do_render(Game& game, Time time) {
	const Time lifetime = 0.3f;
	const LevelCoord start_size = 3;
	const LevelCoord max_size = 3.5f;

	if (time >= lifetime) {
		die();
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

