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

#ifndef UI_COMMON_H_
#define UI_COMMON_H_

#include "../../common.h"

// To define macros in KeyEvent.is
#include <GLFW/glfw3.h>

#include <cstdarg>


using GLKey = int;
enum GLKeyAction {
		PRESS, RELEASE, ANY
};


struct KeyEvent {
	int key;
	int scanmode;
	int action;
	int mods;

	inline bool is(
			GLKeyAction accepted_action,
			std::size_t key_count,
			...
	) {
		if (accepted_action == ANY) {
			if (
					(action != GLFW_PRESS) && (action != GLFW_RELEASE)
			) {
				return false;
			}
		} else {
			if (
					(accepted_action == PRESS) != (action == GLFW_PRESS)
			) {
				return false;
			}
		}

		va_list accepted_keys;
		va_start(accepted_keys, key_count);

		for (std::size_t i = 0; i < key_count; ++i) {
			if (key == va_arg(accepted_keys, GLKey)) { // @suppress("C-Style cast instead of C++ cast")
				return true;
			}
		}

		va_end(accepted_keys);

		return false;
	}

	inline bool is(
				GLKeyAction accepted_action,
				GLKey key
	) {
		return is(accepted_action, 1, key);
	}
};


#endif /* UI_COMMON_H_ */
