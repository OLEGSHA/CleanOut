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

#include "ui.h"


std::vector<Layer*> layers;
std::vector<Layer*> layers_copy;

void render_layers() {
	layers_copy.clear();
	for (Layer *layer : layers) {
		layers_copy.push_back(layer);
	}

	for (Layer *layer : layers_copy) {
		layer->render();
	}
}

void add_layer(Layer* layer) {
	layers.push_back(layer);
	layer->on_added();
}

void remove_top_layer() {
	if (layers.empty()) {
		return;
	}

	Layer *layer = layers.back();
	layers.pop_back();
	layer->on_removed();
	delete layer;
}

void remove_all_layers() {
	for (Layer *layer : layers) {
		delete layer;
	}

	layers.clear();
}

void dispatch_key_event(KeyEvent event) {
	if (layers.empty()) {
		return;
	}

	layers.back()->on_key_event(event);
}

void dispatch_resize() {
	for (Layer *layer : layers) {
		layer->on_resize();
	}
}

Component* center_component(Component* content) {
	Component *result = new Component(
			"Centerer", new CenterLayoutManager(), content->get_layout_hint()
	);

	result->add_child(content);

	return result;
}
