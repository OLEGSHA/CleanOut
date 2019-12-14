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

#include "glyph.h"

#include "graphics.h"


/*
 * Primitives
 */

void LinePrimitive::render() {
	draw_line(start, end);
}

void ArcPrimitive::render() {
	draw_arc(center, radius, start_angle, end_angle);
}

/*
 * Glyph
 */

Glyph::Glyph(
		GlyphCoord width,
		size_t primitive_count, ...
) :
	width(width), primitive_count(primitive_count) {

	va_list args;
	va_start(args, primitive_count);

	primitives = new Primitive*[primitive_count];

	for (size_t i = 0; i < primitive_count; ++i) {
		primitives[i] = va_arg(args, Primitive*); // @suppress("C-Style cast instead of C++ cast")
	}

	va_end(args);
}

Glyph::~Glyph() {
	for (size_t i = 0; i < primitive_count; ++i) {
		delete primitives[i];
	}

	delete[] primitives;
}

bool Glyph::is_whitespace() const {
	return primitive_count == 0;
}

void Glyph::render() const {
	for (size_t i = 0; i < primitive_count; ++i) {
		primitives[i]->render();
	}
}

/*
 * Glyphs
 */

Glyphs::Glyphs(char max_char, const Glyph* fallback) :
	fallback(fallback),
	max_char(get_index(max_char)),
	contents(new const Glyph*[this->max_char + 1])
{
	for (size_t i = 0; i <= this->max_char; ++i) contents[i] = this->fallback;
}

Glyphs::~Glyphs() {
	for (size_t i = 0; i <= max_char; ++i) {
		if (contents[i] != fallback) {
			delete contents[i];
		}
	}

	delete fallback;
	delete[] contents;
}

size_t Glyphs::get_index(char c) const {
	return static_cast<unsigned char>(c);
}

const Glyph& Glyphs::get(char for_char) const {
	size_t index = get_index(for_char);
	if (index > max_char) {
		return *fallback;
	}

	return *contents[index];
}

void Glyphs::put(char c, const Glyph* glyph) {
	if (contents[get_index(c)] != fallback) {
		delete contents[get_index(c)];
	}

	contents[get_index(c)] = glyph;
}

Glyphs default_glyphs('z', new Glyph(4, 4,
		new LinePrimitive({0,0}, {0,7}),
		new LinePrimitive({0,7}, {4,7}),
		new LinePrimitive({4,7}, {4,0}),
		new LinePrimitive({4,0}, {0,0})
));

Glyphs& get_default_glyphs() {
	return default_glyphs;
}

/*
 * Default font
 */

void register_default_glyphs(Glyphs& output) {
	typedef ArcPrimitive Arc;
	typedef LinePrimitive Line;



	output.put(' ', new Glyph(2, 0));



	output.put('0', new Glyph(4, 5,
			new Arc ({2,2}, 2, PI/2, 3*PI/2),
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,2}, {4,5}),
			new Line({3.5f, 2.5f}, {0.5f, 4.5f})
	));
	output.put('1', new Glyph(4, 3,
			new Arc ({0,0}, 2, 0, PI/2),
			new Line({2,0}, {2,7}),
			new Line({0,7}, {4,7})
	));
	output.put('2', new Glyph(4, 4,
			new Arc ({2,2}, 2, 0, 3*PI/2),
			new Arc ({2,6}, 2, PI, 3*PI/2),
			new Line({0,6}, {0,7}),
			new Line({0,7}, {4,7})
	));
	output.put('3', new Glyph(4, 3,
			new Line({0,0}, {4,0}),
			new Line({4,0}, {2,3}),
			new Arc ({2,5}, 2, -PI/2, PI)
	));
	output.put('4', new Glyph(4, 3,
			new Line({4,4}, {0,4}),
			new Line({0,4}, {3,0}),
			new Line({3,0}, {3,7})
	));
	output.put('5', new Glyph(4, 4,
			new Line({4,0}, {0,0}),
			new Line({0,0}, {0,3}),
			new Line({0,3}, {2,3}),
			new Arc ({2,5}, 2, -PI/2, PI)
	));
	output.put('6', new Glyph(4, 3,
			new Arc ({2,2}, 2, PI/2, 3*PI/2),
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, 0, 2*PI)
	));
	output.put('7', new Glyph(4, 2,
			new Line({0,0}, {4,0}),
			new Line({4,0}, {0,7})
	));
	output.put('8', new Glyph(4, 2,
			new Arc ({2, 1.5f}, 1.5f, 0, 2*PI),
			new Arc ({2,5}, 2, 0, 2*PI)
	));
	output.put('9', new Glyph(4, 3,
			new Arc ({2,2}, 2, 0, 2*PI),
			new Line({4,2}, {4,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2)
	));



	output.put('.', new Glyph(1, 1,
			new Arc ({0.5f, 6.5f}, 0.5f, 0, 2*PI)
	));
	output.put(',', new Glyph(1, 1,
			new Arc ({-2,7}, 2, 0, PI/2)
	));
	output.put(':', new Glyph(1, 2,
			new Arc ({0.5f, 3}, 0.5f, 0, 2*PI),
			new Arc ({0.5f, 6}, 0.5f, 0, 2*PI)
	));
	output.put('-', new Glyph(2, 1,
			new Line({0,3}, {2,3})
	));



	output.put('A', new Glyph(4, 4,
			new Arc ({2,2}, 2, PI/2, 3*PI/2),
			new Line({0,7}, {0,2}),
			new Line({4,7}, {4,2}),
			new Line({0,3}, {4,3})
	));
	output.put('B', new Glyph(4, 6,
			new Arc ({2, 1.5f}, 1.5f, 0, PI),
			new Arc ({2,5}, 2, 0, PI),
			new Line({0,0}, {0,7}),
			new Line({0,0}, {2,0}),
			new Line({0,3}, {2,3}),
			new Line({0,7}, {2,7})
	));
	output.put('C', new Glyph(4, 5,
			new Line({4,0}, {2,0}),
			new Arc ({2,2}, 2, PI, 3*PI/2),
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, 3*PI/2, 2*PI),
			new Line({2,7}, {4,7})
	));
	output.put('D', new Glyph(4, 6,
			new Line({0,0}, {2,0}),
			new Arc ({2,2}, 2, PI/2, PI),
			new Line({4,2}, {4,5}),
			new Arc ({2,5}, 2, 0, PI/2),
			new Line({0,7}, {2,7}),
			new Line({0,0}, {0,7})
	));
	output.put('E', new Glyph(4, 4,
			new Line({0,0}, {0,7}),
			new Line({0,0}, {4,0}),
			new Line({0,3}, {3,3}),
			new Line({0,7}, {4,7})
	));
	output.put('F', new Glyph(4, 3,
			new Line({0,0}, {0,7}),
			new Line({0,0}, {4,0}),
			new Line({0,3}, {3,3})
	));
	output.put('G', new Glyph(4, 5,
			new Arc ({2,2}, 2, PI/2, 3*PI/2),
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,5}, {4,4}),
			new Line({4,4}, {2,4})
	));
	output.put('H', new Glyph(4, 3,
			new Line({0,0}, {0,7}),
			new Line({4,0}, {4,7}),
			new Line({0,3}, {4,3})
	));
	output.put('I', new Glyph(3, 3,
			new Line({1.5f, 0}, {1.5f, 7}),
			new Line({0,0}, {3,0}),
			new Line({0,7}, {3,7})
	));
	output.put('J', new Glyph(3, 3,
			new Line({0,0}, {3,0}),
			new Line({3,0}, {3,4}),
			new Arc ({0,4}, 3, 0, PI/2)
	));
	output.put('K', new Glyph(4, 3,
			new Line({0,0}, {0,7}),
			new Line({0,3}, {4,0}),
			new Line({0,3}, {4,7})
	));
	output.put('L', new Glyph(4, 2,
			new Line({0,0}, {0,7}),
			new Line({0,7}, {4,7})
	));
	output.put('M', new Glyph(5, 4,
			new Line({0,7}, {0,0}),
			new Line({0,0}, {2.5f, 5}),
			new Line({2.5f, 5}, {5,0}),
			new Line({5,0}, {5,7})
	));
	output.put('N', new Glyph(4, 3,
			new Line({0,7}, {0,0}),
			new Line({0,0}, {4,7}),
			new Line({4,7}, {4,0})
	));
	output.put('O', new Glyph(4, 4,
			new Arc ({2,2}, 2, PI/2, 3*PI/2),
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,2}, {4,5})
	));
	output.put('P', new Glyph(4, 6,
			new Line({0,0}, {2,0}),
			new Arc ({2,2}, 2, PI/2, PI),
			new Line({4,2}, {4,3}),
			new Arc ({2,3}, 2, 0, PI/2),
			new Line({0,5}, {2,5}),
			new Line({0,0}, {0,7})
	));
	output.put('Q', new Glyph(4, 5,
			new Arc ({2,2}, 2, PI/2, 3*PI/2),
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,2}, {4,5}),
			new Line({2,7}, {4,8})
	));
	output.put('R', new Glyph(4, 7,
			new Line({0,0}, {2,0}),
			new Arc ({2,2}, 2, PI/2, PI),
			new Line({4,2}, {4,3}),
			new Arc ({2,3}, 2, 0, PI/2),
			new Line({0,5}, {2,5}),
			new Line({0,0}, {0,7}),
			new Arc ({2,7}, 2, PI/2, PI)
	));
	output.put('S', new Glyph(4, 7,
			new Line({4,0}, {1.5f, 0}),
			new Arc ({1.5f, 1.5f}, 1.5f, PI, 2*PI),
			new Line({1.5f, 3}, {2.5f, 3}),
			new Arc ({2.5f, 4.5f}, 1.5f, PI/2, PI),
			new Line({4, 4.5f}, {4, 5.5f}),
			new Arc ({2.5f, 5.5f}, 1.5f, 0, PI/2),
			new Line({2.5f, 7}, {0, 7})
	));
	output.put('T', new Glyph(4, 2,
			new Line({0,0}, {4,0}),
			new Line({2,0}, {2,7})
	));
	output.put('U', new Glyph(4, 3,
			new Line({0,0}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,5}, {4,0})
	));
	output.put('V', new Glyph(4, 2,
			new Line({0,0}, {2,7}),
			new Line({2,7}, {4,0})
	));
	output.put('W', new Glyph(5, 4,
			new Line({0,0}, {1,7}),
			new Line({1,7}, {2.5f, 3}),
			new Line({2.5f, 3}, {4,7}),
			new Line({4,7}, {5,0})
	));
	output.put('X', new Glyph(4, 2,
			new Line({0,0}, {4,7}),
			new Line({4,0}, {0,7})
	));
	output.put('Y', new Glyph(4, 3,
			new Line({0,0}, {2,3}),
			new Line({4,0}, {2,3}),
			new Line({2,3}, {2,7})
	));
	output.put('Z', new Glyph(4, 3,
			new Line({0,0}, {4,0}),
			new Line({4,0}, {0,7}),
			new Line({0,7}, {4,7})
	));



	output.put('a', new Glyph(4, 7,
			new Line({0,2}, {3,2}),
			new Arc ({3,3}, 1, PI/2, PI),
			new Line({4,3}, {4,7}),
			new Arc ({2.5f, 5.5f}, 1.5f, 0, PI/2),
			new Line({2.5f, 7}, {1.5f, 7}),
			new Arc ({1.5f, 5.5f}, 1.5f, PI, 2*PI),
			new Line({1.5f, 4}, {4,4})
	));
	output.put('b', new Glyph(4, 4,
			new Line({0,0}, {0,7}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({4,4}, {4,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2)
	));
	output.put('c', new Glyph(3, 5,
			new Line({3,2}, {2,2}),
			new Arc ({2,4}, 2, PI, 3*PI/2),
			new Line({0,4}, {0,5}),
			new Arc ({2,5}, 2, 3*PI/2, 2*PI),
			new Line({2,7}, {3,7})
	));
	output.put('d', new Glyph(4, 4,
			new Line({4,0}, {4,7}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({0,4}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2)
	));
	output.put('e', new Glyph(4, 4,
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({0,4}, {4,4}),
			new Arc ({3,4}, 3, -PI/2, 0),
			new Line({3,7}, {4,7})
	));
	output.put('f', new Glyph(3, 4,
			new Arc ({3, 1.5f}, 1.5f, PI, 3*PI/2),
			new Line({1.5f, 1.5f}, {1.5f, 5.5f}),
			new Arc ({0, 5.5f}, 1.5f, 0, PI/2),
			new Line({0,3}, {3,3})
	));
	output.put('g', new Glyph(4, 6,
			new Line({4,2}, {4,7}),
			new Arc ({2,7}, 2, 0, PI/2),
			new Line({2,9}, {0,9}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({0,4}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2)
	));
	output.put('h', new Glyph(4, 3,
			new Line({0,0}, {0,7}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({4,4}, {4,7})
	));
	output.put('i', new Glyph(3, 4,
			new Line({0,7}, {3,7}),
			new Line({1.5f, 7}, {1.5f, 3}),
			new Line({1.5f, 3}, {0,3}),
			new Arc ({1.5f, 1.5f}, 0.5f, 0, 2*PI)
	));
	output.put('j', new Glyph(2, 4,
			new Arc ({0,7}, 2, 0, PI/2),
			new Line({2,7}, {2,3}),
			new Line({2,3}, {0,3}),
			new Arc ({1.5f, 1.5f}, 0.5f, 0, 2*PI)
	));
	output.put('k', new Glyph(3, 3,
			new Line({0,0}, {0,7}),
			new Line({0, 4.5f}, {3,2}),
			new Line({0, 4.5f}, {3,7})
	));
	output.put('l', new Glyph(1.5f, 2,
			new Line({0,0}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, 0)
	));
	output.put('m', new Glyph(6, 5,
			new Line({0,7}, {0,2}),
			new Arc ({1.5f, 3.5f}, 1.5f, PI/2, 3*PI/2),
			new Line({3, 3.5f}, {3,7}),
			new Arc ({4.5f, 3.5f}, 1.5f, PI/2, 3*PI/2),
			new Line({6, 3.5f}, {6,7})
	));
	output.put('n', new Glyph(4, 3,
			new Line({0,2}, {0,7}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({4,4}, {4,7})
	));
	output.put('o', new Glyph(5, 1,
			new Arc ({2.5f, 4.5f}, 2.5f, 0, 2*PI)
	));
	output.put('p', new Glyph(4, 4,
			new Line({0,2}, {0,9}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({4,4}, {4,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2)
	));
	output.put('q', new Glyph(4, 4,
			new Line({4,2}, {4,9}),
			new Arc ({2,4}, 2, PI/2, 3*PI/2),
			new Line({0,4}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2)
	));
	output.put('r', new Glyph(3, 3,
			new Line({0,2}, {0,7}),
			new Arc ({2,4}, 2, PI, 3*PI/2),
			new Line({2,2}, {3,2})
	));
	output.put('s', new Glyph(4, 5,
			new Line({4,2}, {1,2}),
			new Arc ({1,3}, 1, PI, 2*PI),
			new Line({1,4}, {2.5f, 4}),
			new Arc ({2.5f, 5.5f}, 1.5f, 0, PI),
			new Line({2.5f, 7}, {0,7})
	));
	output.put('t', new Glyph(3, 3,
			new Line({1,0}, {1,5}),
			new Arc ({3,5}, 2, 3*PI/2, 2*PI),
			new Line({0,3}, {3,3})
	));
	output.put('u', new Glyph(4, 3,
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,2}, {4,7})
	));
	output.put('v', new Glyph(4, 2,
			new Line({0,2}, {2,7}),
			new Line({2,7}, {4,2})
	));
	output.put('w', new Glyph(6, 5,
			new Line({0,2}, {0, 5.5f}),
			new Arc ({1.5f, 5.5f}, 1.5f, -PI/2, PI/2),
			new Line({3,2}, {3, 5.5f}),
			new Arc ({4.5f, 5.5f}, 1.5f, -PI/2, PI/2),
			new Line({6,2}, {6, 5.5f})
	));
	output.put('x', new Glyph(4, 2,
			new Line({0,2}, {4,7}),
			new Line({0,7}, {4,2})
	));
	output.put('y', new Glyph(4, 5,
			new Line({0,2}, {0,5}),
			new Arc ({2,5}, 2, -PI/2, PI/2),
			new Line({4,2}, {4,7}),
			new Arc ({2,7}, 2, 0, PI/2),
			new Line({2,9}, {0,9})
	));
	output.put('z', new Glyph(4, 3,
			new Line({0,2}, {4,2}),
			new Line({4,2}, {0,7}),
			new Line({0,7}, {4,7})
	));
}
