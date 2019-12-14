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

#include "label.h"

#include "../graphics.h"


Label::Label(
		const char *name,
		const char *text,
		Font font, LayoutHint hint,
		bool delete_text
) :
	Component(name, hint),
	text(text),
	delete_text(delete_text),
	font(font)
{
	set_preferred_size((
		draw_string(get_font()) << get_text()
	).get_dimensions());
}

Label::~Label() {
	if (delete_text) {
		delete[] text;
	}
}

void Label::render_self() {
	set_color(Design::OUTLINE);
	(draw_string(get_font()) << get_text()).render({0, 0});
}

