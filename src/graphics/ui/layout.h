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

#ifndef LAYOUT_H_
#define LAYOUT_H_


#include "ui_common.h"


typedef ScreenPoint Size;

struct Box {
	ScreenPoint min, max;

	Box(ScreenPoint min, ScreenPoint max) :
	min(min), max(max) {}

	Box(
			ScreenCoord min_x, ScreenCoord min_y,
			ScreenCoord width, ScreenCoord height
	) :
		min({min_x, min_y}),
		max({min_x + width, min_y + height})
	{}

	ScreenCoord width() const {
		return max.x - min.x;
	}

	ScreenCoord height() const {
		return max.y - min.y;
	}

	Size size() const {
		return {
			width(),
			height()
		};
	}
};


class Component;

typedef unsigned int LayoutHint;
const LayoutHint NOT_SPECIFIED = 0;

class LayoutManager {
public:
	LayoutManager() {}
	virtual ~LayoutManager() {}

	virtual void layout(Component&) = 0;
	virtual Size compute_preferred_size(Component&) = 0;
};

/*
 * Positions all children in the center with their preferred size.
 */
class CenterLayoutManager : public LayoutManager {
public:
	virtual void layout(Component&) override;
	virtual Size compute_preferred_size(Component&) override;
};

namespace BorderLayoutHints {
	enum : LayoutHint {
		CENTER, TOP, BOTTOM, LEFT, RIGHT
	};
}

/*
 * Positions children with TOP, BOTTOM, LEFT, RIGHT layout hints on the
 * respective edge of the screen and fills the rest of available space with
 * children with CENTER and unknown layout hints.
 */
class BorderLayoutManager : public LayoutManager {
private:
	void compute_margins(
			Component&,
			ScreenCoord& top, ScreenCoord& bottom,
			ScreenCoord& left, ScreenCoord& right,
			Size* center_size, ScreenCoord* max_width, ScreenCoord* max_height
	);
public:
	virtual void layout(Component&) override;
	virtual Size compute_preferred_size(Component&) override;
};

/*
 * Positions children in a vertical array. Width is set to maximum preferred
 * width of all children, height is set to preferred height per child. A margin
 * is inserted between the children and around the children. Array is centered
 * within parent.
 */
class VerticalFlowLayoutManager : public LayoutManager {
protected:
	ScreenCoord margin;
public:
	VerticalFlowLayoutManager(ScreenCoord margin = 15) :
		margin(margin) {}

	virtual void layout(Component&) override;
	virtual Size compute_preferred_size(Component&) override;
};


#endif /* LAYOUT_H_ */
