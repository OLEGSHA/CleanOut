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

#ifndef LAYER_H_
#define LAYER_H_

#include "component.h"


class Layer {
private:
	Component* const root;
	bool close_on_escape;

	Action close_action = nullptr;

public:
	Layer(LayoutManager*, bool close_on_escape = false);
	virtual ~Layer();

	Component*
	get_root() const
	{ return root; }

	void render();
	void on_key_event(KeyEvent event);
	void on_resize();

	void set_close_action(Action);

	void on_added();
	void on_removed();
};

#endif /* LAYER_H_ */
