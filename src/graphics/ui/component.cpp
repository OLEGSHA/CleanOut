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

/*
 * Focus logic reused from Crystal Farm:
 * https://github.com/OLEGSHA/crystal-farm/
 */

#include "component.h"

#include "../graphics.h"


Component::Component(
		const char *name,
		LayoutManager *layout_manager,
		LayoutHint layout_hint
):
	name(name),
	layout_manager(
			layout_manager == nullptr
			? new CenterLayoutManager()
			: layout_manager
	),
	layout_hint(layout_hint)
{}

Component::~Component() {
	delete layout_manager;

	for (Component* child : children) {
		delete child;
	}
}

void Component::set_bounds(Box bounds) {
	this->bounds = bounds;
	invalidate();
}

Component* Component::get_root() {
	Component* comp = this;

	while (!comp->is_root()) {
		comp = comp->get_parent();
	}

	return comp;
}

void Component::set_layout_hint(LayoutHint hint) {
	this->layout_hint = hint;
	invalidate();
}

void Component::apply_transform() {
	glTranslatef(bounds.min.x, bounds.min.y, 0);
}

void Component::render() {
	if (!is_valid) {
		layout();
	}

	glPushMatrix();
	apply_transform();

	render_self();
	render_children();

	glPopMatrix();
}

void Component::render_children() {
	for (Component *child : children) {
		child->render();
	}
}

void Component::add_child(Component* child) {
	child->parent = this;
	child->index_in_parent = children.size();

	children.push_back(child);
	invalidate();
}

Size Component::get_preferred_size() {
	if (is_preferred_size_set) {
		return preferred_size;
	}

	if (is_preferred_size_cache_valid) {
		return preferred_size_cache;
	}

	preferred_size_cache = layout_manager->compute_preferred_size(*this);
	is_preferred_size_cache_valid = true;

	return preferred_size_cache;
}

void Component::set_preferred_size(Size preferred_size) {
	this->preferred_size = preferred_size;
	is_preferred_size_set = true;
	invalidate();
}

void Component::invalidate() {
	is_valid = false;
	is_preferred_size_cache_valid = false;
}

void Component::layout() {
	if (parent != nullptr) {
		parent->layout();
	} else {
		do_layout();
	}
}

void Component::do_layout() {
	layout_manager->layout(*this);
	is_valid = true;

	for (Component *child : children) {
		child->do_layout();
	}
}

bool Component::try_moving_focus(KeyEvent event) {
	if (event.is(PRESS, GLFW_KEY_TAB)) {
		if ((event.mods & GLFW_MOD_SHIFT) == 0) {
			focus_next();
		} else {
			focus_previous();
		}

		return true;
	}

	if (event.is(PRESS, GLFW_KEY_UP)) {
		focus_previous();
		return true;
	}

	if (event.is(PRESS, GLFW_KEY_DOWN)) {
		focus_next();
		return true;
	}

	return false;
}

bool Component::process_event(KeyEvent event) {
	if (has_focus) {
		if (!on_event(event)) {
			return try_moving_focus(event);
		}

		return true;
	} else {
		for (Component *child : children) {
			if (child->process_event(event)) {
				return true;
			}
		}

		return false;
	}
}

/*
 * Focus transfer
 */

Component* Component::get_next_focus_candidate(bool can_use_children) {
	if (can_use_children) {
		if (!get_children().empty()) {
			return get_children().at(0);
		}
	}

	Component *parent = get_parent();
	if (parent != nullptr) {
		if (index_in_parent != parent->get_children().size() - 1) {
			return parent->get_children().at(index_in_parent + 1);
		}

		return parent->get_next_focus_candidate(false);
	}

	return this;
}

void Component::focus_next() {
	Component* component = this;

	while (true) {

		component = component->get_next_focus_candidate(true);
		if (component == this) {
			return;
		}

		if (component->is_focusable()) {
			this->has_focus = false;
			component->has_focus = true;
			return;
		}

	}
}

Component* Component::get_previous_focus_candidate() {
	Component *parent = get_parent();
	if (parent != nullptr) {
		if (index_in_parent != 0) {
			return parent->get_children().at(index_in_parent - 1)
					->get_last_deep_child();
		}

		return parent;
	}

	return get_last_deep_child();
}

void Component::focus_previous() {
	Component* component = this;

	while (true) {

		component = component->get_previous_focus_candidate();
		if (component == this) {
			return;
		}

		if (component->is_focusable()) {
			this->has_focus = false;
			component->has_focus = true;
			return;
		}

	}
}

Component* Component::get_last_deep_child() {
	if (!get_children().empty()) {
		return get_children().at(get_children().size() - 1)
				->get_last_deep_child();
	}

	return this;
}

Component* Component::find_focused() {
	if (is_focused()) {
		return this;
	}

	for (Component *child : get_children()) {
		Component *result = child->find_focused();
		if (result != nullptr) {
			return result;
		}
	}

	return nullptr;
}

void Component::grab_focus() {
	Component* root = get_root();
	Component* focused = root->find_focused();

	if (focused != nullptr) {
		focused->has_focus = false;
	}

	has_focus = true;
}

/*
 * Misc
 */

std::ostream& operator<<(std::ostream& stream, const Component& comp) {
	stream << comp.get_name();
	return stream;
}
