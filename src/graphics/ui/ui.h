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

#ifndef UI_H_
#define UI_H_

#include "layer.h"
#include "game_layer.h"

#include "button.h"
#include "label.h"


void render_layers();

void add_layer(Layer*);
void remove_top_layer();
void remove_all_layers();

void dispatch_key_event(KeyEvent);
void dispatch_resize();


Component* center_component(Component*);


#endif /* UI_H_ */
