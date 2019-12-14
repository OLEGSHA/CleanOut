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

#ifndef FONT_H_
#define FONT_H_

#include "../common.h"

#include <cstdarg>
#include <iostream>
#include <sstream>

#include "glyph.h"


class Font {
private:
	const Glyphs* glyphs;
	ScreenCoord size;

	ScreenCoord get_width(const Glyph& glyph) const;
	ScreenCoord get_advance(const Glyph& glyph) const;

public:
	Font(const Glyphs& glyphs, ScreenCoord size) :
		glyphs(&glyphs), size(size / GLYPH_HEIGHT) {}

	Font(const Font& copy, ScreenCoord new_size) :
		Font(*copy.glyphs, new_size) {}

	Font(ScreenCoord size) :
		Font(get_default_glyphs(), size) {}

	ScreenCoord render(ScreenPoint& pos, char c) const;
	ScreenCoord get_height() const;
	ScreenCoord get_width(char c) const;
	ScreenCoord get_advance(char c) const;
};

struct StringDrawer {
	std::stringstream stream;
	const Font font;

	void render(ScreenPoint);
	ScreenPoint get_dimensions();
};

template < class T >
StringDrawer operator<<(StringDrawer drawer, const T& any) {
	drawer.stream << any;
	return drawer;
}

StringDrawer draw_string(Font font);

#endif /* FONT_H_ */
