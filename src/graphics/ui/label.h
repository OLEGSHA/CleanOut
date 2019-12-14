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

#ifndef LABEL_H_
#define LABEL_H_

#include "component.h"
#include "../font.h"


class Label : public Component {
private:
	const char * const text;
	const bool delete_text;
	const Font font;

protected:
	virtual void render_self() override;

public:
	Label(
			const char *name,
			const char *text, Font font,
			LayoutHint hint = NOT_SPECIFIED,
			bool delete_text = false
	);

	~Label();

	const char* get_text() { return text; }
	Font get_font() { return font; }
};


#endif /* LABEL_H_ */
