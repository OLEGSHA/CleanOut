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

#ifndef BUTTON_H_
#define BUTTON_H_

#include "component.h"


class Button : public Component {

private:
	void (*action)();

protected:
	virtual void render_self() override;
	virtual bool on_event(KeyEvent) override;

public:
	Button(const char *name, void (*action)(), LayoutHint hint = NOT_SPECIFIED);

	virtual bool is_focusable() const override {
		return true;
	}

};


#endif /* BUTTON_H_ */
