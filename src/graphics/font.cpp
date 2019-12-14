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

#include "font.h"

#include "graphics.h"


/*
 * Font
 */

ScreenCoord Font::get_height() const {
	return size * (GLYPH_HEIGHT + 1);
}

ScreenCoord Font::get_width(const Glyph& glyph) const {
	return glyph.get_width() * size;
}

ScreenCoord Font::get_advance(const Glyph& glyph) const {
	return get_width(glyph) + 1 * size;
}

ScreenCoord Font::get_width(char c) const {
	return get_width(glyphs->get(c));
}

ScreenCoord Font::get_advance(char c) const {
	return get_advance(glyphs->get(c));
}

ScreenCoord Font::render(ScreenPoint& pos, char c) const {
	const Glyph& glyph = glyphs->get(c);

	if (!glyph.is_whitespace()) {
		glPushMatrix();
		glScalef(size, size, 1);
		glTranslatef(pos.x / size, pos.y / size, 0);

		glyph.render();

		glPopMatrix();
	}

	return get_advance(glyph);
}

/*
 * StringDrawer
 */

void StringDrawer::render(ScreenPoint origin) {
	ScreenPoint pos = origin;

	while (true) {
		char c = stream.get();
		if (stream.eof()) break;

		if (c == '\n') {
			pos.x = origin.x;
			pos.y += font.get_height();
		} else {
			pos.x += font.render(pos, c);
		}
	}
}

ScreenPoint StringDrawer::get_dimensions() {
	ScreenCoord max_width = 0;
	ScreenCoord current_width = 0;
	ScreenCoord height = font.get_height();

	while (true) {
		char c = stream.get();
		if (stream.eof()) break;

		if (c == '\n') {
			height += font.get_height();

			if (max_width < current_width) {
				max_width = current_width;
			}
			current_width = 0;
		} else {
			current_width += font.get_advance(c);
		}
	}

	stream.clear();
	stream.seekg(0, std::ios_base::beg);

	return {
		std::max(current_width, max_width),
		height
	};
}

StringDrawer draw_string(Font font) {
	return StringDrawer {
		std::stringstream(),
		font
	};
}
