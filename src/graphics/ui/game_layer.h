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

#ifndef GAME_LAYER_H_
#define GAME_LAYER_H_

#include "layer.h"


Layer* create_game_layer(Game *game);

class GameComponent : public Component {
private:
	Game * const game;

	void tick();

	void render_decorations();
	void render_game();

	Size get_level_size();

protected:
	virtual void apply_transform() override;
	virtual void render_self() override;
	virtual bool on_event(KeyEvent) override;

public:
	GameComponent(Game *game, LayoutHint hint) :
		Component("Game", hint),
		game(game)
	{}

	virtual ~GameComponent();

	Game* get_game() {
		return game;
	}

	virtual bool is_focusable() const override {
		return true;
	}
};


#endif /* GAME_LAYER_H_ */
