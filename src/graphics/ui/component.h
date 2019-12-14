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

#ifndef COMPONENT_H_
#define COMPONENT_H_


#include "layout.h"


class Component {
private:
	const char * const name;

	Box bounds = {{0,0}, {0,0}};
	Component *parent = nullptr;
	size_t index_in_parent = 0;

	bool has_focus = false;
	bool is_valid = false;

	LayoutManager * const layout_manager;
	LayoutHint layout_hint;

	Size preferred_size_cache = {0, 0};
	bool is_preferred_size_cache_valid = false;

	Size preferred_size = {0, 0};
	bool is_preferred_size_set = false;

	std::vector<Component*> children;


	void
	render_children();

	bool
	dispatch_event(KeyEvent);

	void
	do_layout();

	Component* get_next_focus_candidate(bool can_use_children);
	void focus_next();
	Component* get_previous_focus_candidate();
	void focus_previous();
	Component* get_last_deep_child();
	Component* find_focused();

	bool try_moving_focus(KeyEvent);

protected:
	virtual void apply_transform();
	virtual void render_self() {}
	virtual bool on_event(KeyEvent) { return false; }

public:
	Component(
			const char *name,
			LayoutManager* layout_manager,
			LayoutHint hint
	);

	Component(
			const char *name,
			LayoutHint hint
	) : Component(name, nullptr, hint) {}

	Component(
			const char *name,
			LayoutManager* layout_manager
	) : Component(name, layout_manager, NOT_SPECIFIED) {}

	Component(
			const char *name
	) : Component(name, nullptr, NOT_SPECIFIED) {}

	virtual ~Component();

	const char*
	get_name() const
	{ return name; }

	Box
	get_bounds() const
	{ return bounds; }

	bool
	is_focused() const
	{ return has_focus; }

	void
	grab_focus();

	Component*
	get_parent() const
	{ return parent; }

	Component*
	get_root();

	bool
	is_root() const
	{ return parent == nullptr; }

	const std::vector<Component*>
	get_children() const
	{ return children; }

	virtual
	Size
	get_preferred_size();

	void
	set_preferred_size(Size);

	virtual
	bool
	is_focusable() const
	{ return false; }

	void
	render();

	void
	add_child(Component*);

	void
	layout();

	void
	invalidate();

	LayoutHint
	get_layout_hint() const
	{ return layout_hint; }

	void
	set_layout_hint(LayoutHint);

	void
	set_bounds(Box bounds);

	bool
	process_event(KeyEvent event);
};

std::ostream& operator<<(std::ostream&, const Component&);

typedef Component Container;

#endif /* COMPONENT_H_ */
