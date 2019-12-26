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

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <GLFW/glfw3.h>

#include "../common.h"
#include "font.h"
#include "ui/ui.h"

#include "design.h"


void setup_graphics();
void terminate_graphics();

bool should_close();
void render();

GLFWwindow* get_window_handle();

float get_frame_length();

struct WindowDimensions {
	int width, height;
};

WindowDimensions get_window_size();

void set_color(Color c);
void set_color(unsigned int a, unsigned int r, unsigned int g, unsigned int b);
void set_color(unsigned int r, unsigned int g, unsigned int b);
void set_color(GLfloat a, unsigned int r, unsigned int g, unsigned b);

void draw_line(ScreenPoint, ScreenPoint);

void fill_triangle(ScreenPoint, ScreenPoint, ScreenPoint);

void draw_rectangle(ScreenPoint min, ScreenPoint max);
void fill_rectangle(ScreenPoint min, ScreenPoint max);

void draw_rectangle(ScreenPoint min, ScreenCoord width, ScreenCoord height);
void fill_rectangle(ScreenPoint min, ScreenCoord width, ScreenCoord height);

void draw_rectangle_centered(
		ScreenPoint center,
		ScreenCoord width, ScreenCoord height
);
void fill_rectangle_centered(
		ScreenPoint center,
		ScreenCoord width, ScreenCoord height
);

void do_pseudo_sector(
		ScreenPoint center, ScreenCoord radius,
		unsigned int vertices,
		float start, float end,
		bool fill
);

void fill_polygon(ScreenPoint center, ScreenCoord radius, unsigned int vertices);
void draw_polygon(ScreenPoint center, ScreenCoord radius, unsigned int vertices);

void fill_circle(ScreenPoint center, ScreenCoord radius);
void draw_circle(ScreenPoint center, ScreenCoord radius);

void fill_sector(ScreenPoint center, ScreenCoord radius, float start, float end);
void draw_arc(ScreenPoint center, ScreenCoord radius, float start, float end);

void draw_rounded_rectangle(
		ScreenPoint min, ScreenPoint max,
		ScreenCoord radius
);
void fill_rounded_rectangle(
		ScreenPoint min, ScreenPoint max,
		ScreenCoord radius
);

#endif /* GRAPHICS_H_ */
