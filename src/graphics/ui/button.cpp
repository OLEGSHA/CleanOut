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

#include "button.h"

#include "../graphics.h"


const ScreenCoord MARGIN = 15;

Button::Button(const char *name, void (*action)(), LayoutHint hint) :
	Component(name, hint),
	action(action) {

	auto drawer = draw_string(Font(32)) << name;
	ScreenPoint dims = drawer.get_dimensions();
	set_preferred_size({
		dims.x + 4*MARGIN,
		dims.y + 2*MARGIN
	});
}

void Button::render_self() {
	if (is_focused()) {
		set_color(Design::OUTLINE);
		fill_rectangle({0, 0}, get_bounds().size());
		set_color(Design::BACKGROUND);
	} else {
		set_color(Design::FILL);
		fill_rectangle({0, 0}, get_bounds().size());
		set_color(Design::OUTLINE);
		draw_rectangle({0, 0}, get_bounds().size());
	}

	{
		auto drawer = draw_string(Font(32)) << get_name();

		Size size = drawer.get_dimensions();
		drawer.render({
			(get_bounds().width() - size.x) / 2,
			(get_bounds().height() - size.y) / 2
		});
	}
}

bool Button::on_event(KeyEvent event) {
	if (
			event.action == GLFW_PRESS
			&&
			(event.key == GLFW_KEY_SPACE || event.key == GLFW_KEY_ENTER)
	) {
		action();
		return true;
	}

	return false;
}

