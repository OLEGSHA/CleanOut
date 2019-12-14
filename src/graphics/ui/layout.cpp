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

#include "component.h"


template< class T >
void inline set_to_max(T& var, T value) {
	if (value > var) {
		var = value;
	}
}

template< class T >
void inline set_to_max(T* ptr, T value) {
	if (ptr != nullptr && value > *ptr) {
		*ptr = value;
	}
}

/*
 * Center Layout
 */

void CenterLayoutManager::layout(Component& c) {
	Box box = c.get_bounds();

	for (Component *child : c.get_children()) {
		Size preferred = child->get_preferred_size();

		child->set_bounds(Box(
				{
					(box.width() - preferred.x) / 2,
					(box.height() - preferred.y) / 2
				},
				{
					(box.width() + preferred.x) / 2,
					(box.height() + preferred.y) / 2
				}
		));
	}
}

Size CenterLayoutManager::compute_preferred_size(Component& c) {
	Size result = {0, 0};

	for (Component *child : c.get_children()) {
		Size preferred = child->get_preferred_size();

		if (preferred.x > result.x) {
			result.x = preferred.x;
		}

		if (preferred.y > result.y) {
			result.y = preferred.y;
		}
	}

	return result;
}

/*
 * Border layout
 */

void BorderLayoutManager::compute_margins(
			Component& c,
			ScreenCoord& top, ScreenCoord& bottom,
			ScreenCoord& left, ScreenCoord& right,

			Size* center_size,
			ScreenCoord* max_width, ScreenCoord* max_height
) {
	using namespace BorderLayoutHints;

	top = bottom = left = right = 0;
	if (center_size != nullptr) *center_size = {0, 0};
	if (max_width != nullptr) *max_width = 0;
	if (max_height != nullptr) *max_height = 0;

	for (Component *child : c.get_children()) {

		Size pref = child->get_preferred_size();

		switch (child->get_layout_hint()) {
		case TOP:
			set_to_max(top, pref.y);
			set_to_max(max_width, pref.x);
			break;
		case BOTTOM:
			set_to_max(bottom, child->get_preferred_size().y);
			set_to_max(max_width, pref.x);
			break;
		case LEFT:
			set_to_max(left, child->get_preferred_size().x);
			set_to_max(max_height, pref.y);
			break;
		case RIGHT:
			set_to_max(right, child->get_preferred_size().x);
			set_to_max(max_height, pref.y);
			break;
		case CENTER:
		default:
			if (center_size != nullptr) {
				set_to_max(center_size->x, pref.x);
				set_to_max(center_size->y, pref.y);
			}
			set_to_max(max_width, pref.x);
			set_to_max(max_height, pref.y);

			break;
		}

	}
}

void BorderLayoutManager::layout(Component& c) {
	using namespace BorderLayoutHints;

	Box box = c.get_bounds();
	ScreenCoord width = box.width(), height = box.height();

	ScreenCoord top, bottom, left, right;
	compute_margins(c, top, bottom, left, right, nullptr, nullptr, nullptr);

	for (Component *child : c.get_children()) {
		switch (child->get_layout_hint()) {
		case TOP:

			child->set_bounds(Box(
					0, 0,
					width, top
			));

			break;
		case BOTTOM:

			child->set_bounds(Box(
					0, height - bottom,
					width, bottom
			));

			break;
		case LEFT:

			child->set_bounds(Box(
					0, top,
					left, height - top - bottom
			));

			break;
		case RIGHT:

			child->set_bounds(Box(
					width - right, top,
					right, height - top - bottom
			));

			break;
		case CENTER:
		default:

			child->set_bounds(Box(
					left, top,
					width - left - right, height - top - bottom
			));

			break;
		}
	}
}

Size BorderLayoutManager::compute_preferred_size(Component& c) {
	ScreenCoord top, bottom, left, right;
	Size center_size;
	ScreenCoord max_width, max_height;

	compute_margins(
			c,
			top, bottom, left, right,
			&center_size, &max_width, &max_height
	);

	return {
		std::max(left + center_size.x + right, max_width),
		top + std::max(center_size.y, max_height) + bottom
	};
}

/*
 * VerticalFlowLayoutManager
 */

void VerticalFlowLayoutManager::layout(Component& c) {
	Size my_preferred = c.get_preferred_size();
	Size my_actual = c.get_bounds().size();

	ScreenCoord x = (my_actual.x - my_preferred.x) / 2 + margin;
	ScreenCoord width = my_preferred.x - 2 * margin;
	ScreenCoord y = (my_actual.y - my_preferred.y) / 2 + margin;

	for (Component *child : c.get_children()) {
		Size preferred = child->get_preferred_size();

		child->set_bounds(Box(
				x, y,
				width, preferred.y
		));

		y += preferred.y + margin;
	}
}

Size VerticalFlowLayoutManager::compute_preferred_size(Component& c) {
	ScreenCoord max_width = 0, height = margin;

	for (Component *child : c.get_children()) {
		Size preferred = child->get_preferred_size();

		set_to_max(max_width, preferred.x);
		height += preferred.y + margin;
	}

	return {
		max_width + 2 * margin,
		height
	};
}
