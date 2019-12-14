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

#include "workflow.h"

#include <string>

#include "logic/logic.h"
#include "graphics/graphics.h"


void main_loop() {

	while (!should_close()) {
		render();
	}

}

void start_next_level() {
	Game *game = get_current_attempt()->start_next_level();

	remove_all_layers();
	add_layer(create_game_layer(game));
}

void start_game() {
	end_attempt();
	start_attempt();
	start_next_level();
}

void exit() {
	glfwSetWindowShouldClose(get_window_handle(), GLFW_TRUE);
}

void show_controls_menu() {
	Layer *layer = new Layer(new CenterLayoutManager());

	Component *center = new Container("Center", new BorderLayoutManager()); // @suppress("Ambiguous problem")
	{
		center->add_child(
				new Label(
						"Text",
						"A, Left Arrow - move platform left\n"
						"D, Right Arrow - move platform right\n"
						"Space - release ball\n"
						"Shift - hold to move platform faster",
						Font(32), BorderLayoutHints::CENTER
				)
		);

		center->add_child(
				new Label("Title", "Controls", Font(64), BorderLayoutHints::TOP)
		);

		Button *done_button = new Button("Done", remove_top_layer,
				BorderLayoutHints::BOTTOM);
		done_button->grab_focus();
		center->add_child(center_component(done_button));
	}
	layer->get_root()->add_child(center);
	add_layer(layer);
}

void show_main_menu() {
	remove_all_layers();
	end_attempt();

	Layer *layer = new Layer(new CenterLayoutManager());

	Component *center = new Container("Center", new BorderLayoutManager()); // @suppress("Ambiguous problem")
	{
		Component *buttons = new Container("Buttons",
				new VerticalFlowLayoutManager(), BorderLayoutHints::CENTER);
		{
			Button *play_button = new Button("Play", start_game);
			play_button->grab_focus();
			buttons->add_child(play_button);

			buttons->add_child(new Button("Controls", show_controls_menu));
			buttons->add_child(new Button("Exit", exit));
		}
		center->add_child(buttons);

		center->add_child(
				new Label("Title", "ClearOut", Font(64), BorderLayoutHints::TOP)
		);
	}
	layer->get_root()->add_child(center);
	add_layer(layer);
}

const char* allocate_score_string() {
	std::stringstream stream;
	stream << "Score: " << get_current_attempt()->get_score();
	std::string str = stream.str();

	char *result = new char[str.length() + 1];
	result[str.length()] = '\0';
	for (size_t i = 0; i < str.length(); ++i) {
		result[i] = str[i];
	}

	return result;
}

void show_results_menu(Game& game) {
	Layer *layer = new Layer(new CenterLayoutManager());

	Component *center = new Container("Center", new BorderLayoutManager()); // @suppress("Ambiguous problem")
	{
		Component *buttons = new Container("Buttons",
				new VerticalFlowLayoutManager(), BorderLayoutHints::CENTER);
		{
			Button *play_button = new Button(
					game.state == VICTORY ? "Next Level" : "Play Again",
					game.state == VICTORY ? start_next_level : start_game
			);
			play_button->grab_focus();
			buttons->add_child(play_button);

			buttons->add_child(new Button("Main Menu", show_main_menu));
			buttons->add_child(new Button("Exit", exit));
		}
		center->add_child(buttons);

		Component *results = new Container("Results",
				new VerticalFlowLayoutManager(), BorderLayoutHints::TOP);
		{
			results->add_child(
					new Label(
							"Statement",
							game.state == VICTORY ? "Level clear" : "Game over",
							Font(64)
					)
			);

			const char *text = allocate_score_string();

			results->add_child(
					new Label(
							"Score",
							text,
							Font(32),
							NOT_SPECIFIED,
							true
					)
			);
		}
		center->add_child(results);
	}
	layer->get_root()->add_child(center);

	add_layer(layer);
}
