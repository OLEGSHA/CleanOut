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

#include "graphics.h"

#include "../logic/logic.h"

#include "ui/ui.h"

GLFWwindow *window;

Time last_frame_length;
Time last_frame;

void on_key_event(GLFWwindow*, int key, int scanmode, int action, int mods);
void on_resize(GLFWwindow*, int width, int height);

void setup_graphics() {
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(800, 600, "ClearOut", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, on_key_event);
	glfwSetWindowSizeCallback(window, on_resize);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	on_resize(window, 800, 600);

	glClearColor(
			Design::BACKGROUND.red,
			Design::BACKGROUND.green,
			Design::BACKGROUND.blue,
			0.0f
	);

	register_default_glyphs(get_default_glyphs());
}

void terminate_graphics() {
	remove_all_layers();
	glfwTerminate();
}

bool should_close() {
	return glfwWindowShouldClose(window);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	render_layers();

	Time this_frame = glfwGetTime();
	last_frame_length = this_frame - last_frame;
	last_frame = this_frame;

	glfwSwapBuffers(window);

	glfwPollEvents();
}

GLFWwindow* get_window_handle() {
	return window;
}

Time get_frame_length() {
	return last_frame_length;
}

WindowDimensions get_window_size() {
	WindowDimensions result;
	glfwGetWindowSize(window, &result.width, &result.height);
	return result;
}

void set_color(Color c) {
	glColor4f(c.red, c.green, c.blue, c.alpha);
}

void set_color(unsigned int a, unsigned int r, unsigned int g, unsigned int b) {
	set_color(normalize_channel(a), r, g, b);
}

void set_color(unsigned int r, unsigned int g, unsigned int b) {
	set_color(1.0f, r, g, b);
}

void set_color(GLfloat a, unsigned int r, unsigned int g, unsigned int b) {
	glColor4f(
			normalize_channel(r),
			normalize_channel(g),
			normalize_channel(b),
			a
	);
}

inline void vertex(ScreenPoint p) {
	glVertex2f(p.x, p.y);
}

void draw_line(ScreenPoint a, ScreenPoint b) {
	glBegin(GL_LINES);
		vertex(a);
		vertex(b);
	glEnd();
}

void fill_triangle(ScreenPoint a, ScreenPoint b, ScreenPoint c) {
	glBegin(GL_TRIANGLES);
		vertex(a);
		vertex(b);
		vertex(c);
	glEnd();
}

void draw_rectangle(ScreenPoint min, ScreenPoint max) {
	glBegin(GL_LINE_LOOP);
		vertex(min);
		vertex({min.x, max.y});
		vertex(max);
		vertex({max.x, min.y});
	glEnd();
}

void fill_rectangle(ScreenPoint min, ScreenPoint max) {
	fill_triangle(min, {min.x, max.y}, max);
	fill_triangle(min, {max.x, min.y}, max);
}

void draw_rectangle(ScreenPoint min, ScreenCoord width, ScreenCoord height) {
	draw_rectangle(min, {min.x + width, min.y + height});
}

void fill_rectangle(ScreenPoint min, ScreenCoord width, ScreenCoord height) {
	fill_rectangle(min, {min.x + width, min.y + height});
}

void draw_rectangle_centered(
		ScreenPoint center,
		ScreenCoord width, ScreenCoord height
) {
	draw_rectangle(
			{
					center.x - width / 2,
					center.y - height / 2
			},
			width, height
	);
}

void fill_rectangle_centered(
		ScreenPoint center,
		ScreenCoord width, ScreenCoord height
) {
	fill_rectangle(
			{
					center.x - width / 2,
					center.y - height / 2
			},
			width, height
	);
}

void do_pseudo_sector(
		ScreenPoint center, ScreenCoord radius,
		unsigned int vertices,
		float start, float end,
		bool fill
) {
	glBegin(fill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
		if (fill) vertex(center);

		for (float vertex = 0; vertex <= vertices; vertex++) {
			float angle = start + vertex / vertices * (end - start);
			glVertex2f(
					radius * sin(angle) + center.x,
					radius * cos(angle) + center.y
			);
		}

	glEnd();
}

void fill_polygon(
		ScreenPoint center, ScreenCoord radius,
		unsigned int vertices
) {
	do_pseudo_sector(center, radius, vertices, 0, 2*PI, true);
}

void draw_polygon(
		ScreenPoint center, ScreenCoord radius,
		unsigned int vertices
) {
	do_pseudo_sector(center, radius, vertices, 0, 2*PI, false);
}

unsigned int inline get_circle_vertices(ScreenCoord radius) {
	return 64 * radius;
}

void fill_circle(ScreenPoint center, ScreenCoord radius) {
	do_pseudo_sector(
			center, radius,
			get_circle_vertices(radius),
			0, 2*PI,
			true
	);
}

void draw_circle(ScreenPoint center, ScreenCoord radius) {
	do_pseudo_sector(
			center, radius,
			get_circle_vertices(radius),
			0, 2*PI,
			false
	);
}

void fill_sector(
		ScreenPoint center, ScreenCoord radius,
		float start, float end
) {
	do_pseudo_sector(
			center, radius,
			get_circle_vertices(radius),
			start, end,
			true
	);
}
void draw_arc(
		ScreenPoint center, ScreenCoord radius,
		float start, float end
) {
	do_pseudo_sector(
			center, radius,
			get_circle_vertices(radius),
			start, end,
			false
	);
}

void draw_rounded_rectangle(
		ScreenPoint min, ScreenPoint max,
		ScreenCoord radius
) {
	ScreenPoint
		center_min = {
				min.x + radius,
				min.y + radius
		},
		center_max = {
				max.x - radius,
				max.y - radius
		};

	draw_arc(center_min, radius, -PI, -PI/2);
	draw_line({min.x, center_min.y}, {min.x, center_max.y});
	draw_arc({center_min.x, center_max.y}, radius, -PI/2, 0);
	draw_line({center_min.x, max.y}, {center_max.x, max.y});
	draw_arc(center_max, radius, 0, PI/2);
	draw_line({max.x, center_max.y}, {max.x, center_min.y});
	draw_arc({center_max.x, center_min.y}, radius, PI/2, PI);
	draw_line({center_min.x, min.y}, {center_max.x, min.y});
}

void fill_rounded_rectangle(
		ScreenPoint min, ScreenPoint max,
		ScreenCoord radius
) {
	ScreenPoint
		center_min = {
				min.x + radius,
				min.y + radius
		},
		center_max = {
				max.x - radius,
				max.y - radius
		};

	fill_sector(center_min, radius, -PI, -PI/2);
	fill_sector({center_min.x, center_max.y}, radius, -PI/2, 0);
	fill_sector(center_max, radius, 0, PI/2);
	fill_sector({center_max.x, center_min.y}, radius, PI/2, PI);

	fill_rectangle({center_min.x, min.y}, {center_max.x, max.y});
	fill_rectangle({min.x, center_min.y}, {center_min.x, center_max.y});
	fill_rectangle({center_max.x, center_min.y}, {max.x, center_max.y});
}



void on_key_event(
		GLFWwindow *given_window,
		int key, int scanmode, int action, int mods
) {
	if (window != given_window) {
		return;
	}

	dispatch_key_event({
		key, scanmode, action, mods
	});
}

void on_resize(GLFWwindow*, int width, int height) {
	if (width == 0 && height == 0) {
		return;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);

	dispatch_resize();
}
