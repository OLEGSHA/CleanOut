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

#include "layer.h"
#include "../graphics.h"


const ScreenCoord MARGIN = 15;

Layer::Layer(LayoutManager *manager) :
	root(new Component("Root", manager)) {} // @suppress("Ambiguous problem")

Layer::~Layer() {
	delete root;
}

void Layer::render() {
	set_color(Design::BACKGROUND_TRANSPARENT);

	WindowDimensions dims = get_window_size();
	fill_rectangle({0, 0}, {
			static_cast<ScreenCoord>(dims.width),
			static_cast<ScreenCoord>(dims.height)
	});

	root->render();
}

void Layer::on_key_event(KeyEvent event) {
	root->process_event(event);
}

void Layer::on_resize() {
	WindowDimensions window_size = get_window_size();
	root->set_bounds(Box(
			MARGIN, MARGIN,
			window_size.width - 2*MARGIN, window_size.height - 2*MARGIN
	));
}
