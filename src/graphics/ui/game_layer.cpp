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

#include "game_layer.h"

#include "../graphics.h"
#include "../../logic/logic.h"
#include "../../workflow.h"


const ScreenCoord MARGIN = 15;

class Display : public Component {
private:
	const char * const text;
	const unsigned int width;
	unsigned int (Attempt::* method)();

	StringDrawer write_text() {
		return
				draw_string(Font(32)) << text <<
				std::setw(width) << std::setfill('0') <<
				(get_current_attempt()->*method)();
	}

protected:
	virtual void render_self() override {
		set_color(Design::OUTLINE);
		write_text().render({0, 0});
	}

public:
	Display(
			const char *name, LayoutHint hint,
			const char *text, unsigned int width,
			unsigned int (Attempt::* method)()) :
		Component(name, hint),
		text(text),
		width(width),
		method(method)
	{
		set_preferred_size(write_text().get_dimensions());
	}
};

Layer* create_game_layer(Game *game) {
	using namespace BorderLayoutHints;

	Layer *result = new Layer(new BorderLayoutManager());

	Component *game_comp = new GameComponent(game, CENTER);
	game_comp->grab_focus();

	Component *ui = new Component("UI",
			new BorderLayoutManager(), BOTTOM);
	{
		ui->add_child(
				new Display("LivesDisplay", LEFT,
						"Lives: ", 3, &Attempt::get_lives)
		);

		ui->add_child(
				new Display("ScoreDisplay", RIGHT,
						"Score: ", 5, &Attempt::get_score)
		);

		std::stringstream stream;
		stream << "Level: " << game->level->get_id();
		std::string str = stream.str();

		char *cstr = new char[str.length() + 1];
		cstr[str.length()] = '\0';
		for (size_t i = 0; i < str.length(); ++i) {
			cstr[i] = str[i];
		}

		ui->add_child(center_component(
				new Label("LevelId", cstr, Font(32), CENTER, true)
		));
	}

	result->get_root()->add_child(ui);

	result->get_root()->add_child(game_comp);
	return result;
}

/*
 * GameComponent
 */

GameComponent::~GameComponent() {
	delete game;
}

Size GameComponent::get_level_size() {
	return {
		static_cast<ScreenCoord>(get_game()->level->get_width()),
		static_cast<ScreenCoord>(get_game()->level->get_height())
	};
}

void GameComponent::apply_transform() {
	Size comp = get_bounds().size();
	Size level = get_level_size();

	float scale_factor = std::min(
			(comp.x - 2*MARGIN) / level.x,
			(comp.y - 2*MARGIN) / level.y
	);

	glTranslatef(comp.x / 2.0f, comp.y / 2.0f, 0.0f);
	glScalef(scale_factor, -scale_factor, 1.0f);
	glTranslatef(-level.x / 2.0f, -level.y / 2.0f, 0.0f);
}

void GameComponent::render_self() {
	tick();

	render_decorations();
	render_game();
}

void GameComponent::render_decorations() {
	Size level = get_level_size();

	set_color(Design::FILL);

	draw_line({0, 0/*-INFINITY*/}, {0, level.y});
	draw_line({0, level.y}, {level.x, level.y});
	draw_line({level.x, 0/*-INFINITY*/}, {level.x, level.y});
}

void GameComponent::render_game() {
	game->level->render(*game);
	game->platform.render();

	for (Ball *ball : game->get_balls()) ball->render();
	for (Bonus *bonus : game->get_bonuses()) bonus->render();

	for (
			auto sprite = game->sprites.begin();
			sprite != game->sprites.end();
			++sprite
	) {
		(**sprite).render(*game, static_cast<float>(glfwGetTime()));

		if ((**sprite).is_dead()) {
			auto to_delete = sprite;
			--sprite;

			delete *to_delete;
			game->sprites.erase(to_delete);
		}
	}
}

void GameComponent::tick() {
	::tick(*game, get_frame_length());
}

bool GameComponent::on_event(KeyEvent event) {
	if (event.is(ANY, 4,
			GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_LEFT, GLFW_KEY_RIGHT
	)) {
		game->platform.set_movement(
				event.key == GLFW_KEY_A || event.key == GLFW_KEY_LEFT,
				event.action == GLFW_PRESS,
				(event.mods & GLFW_MOD_SHIFT) != 0
		);
		return true;
	}

	if (event.is(ANY, 2,
			GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT
	)) {
		game->platform.set_movement_fast(
			event.action == GLFW_PRESS
		);
		return true;
	}

	if (event.is(PRESS, GLFW_KEY_SPACE)) {
		for (Ball *ball : game->get_balls()) {
			ball->release();
		}
		return true;
	}

	if (event.is(PRESS, GLFW_KEY_ESCAPE)) {
		pause_game(*game);
	}

	return false;
}
