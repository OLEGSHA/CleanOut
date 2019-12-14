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

#ifndef DESIGN_H_
#define DESIGN_H_

#include <GLFW/glfw3.h>


GLfloat normalize_channel(unsigned int channel_0_thru_255);

struct Color {

	GLfloat alpha, red, green, blue;

	Color(GLfloat alpha, GLfloat red, GLfloat green, GLfloat blue) :
		alpha(alpha),
		red(red),
		green(green),
		blue(blue)
	{}

	Color(
			GLfloat alpha,
			unsigned int rgb
	) :
		alpha(alpha),
		red  (normalize_channel((rgb & 0x00FF0000) >> 2*8)),
		green(normalize_channel((rgb & 0x0000FF00) >> 1*8)),
		blue (normalize_channel((rgb & 0x000000FF) >> 0*8))
	{}

	Color(unsigned int argb) :
		alpha(normalize_channel((argb & 0xFF000000) >> 3*8)),
		red  (normalize_channel((argb & 0x00FF0000) >> 2*8)),
		green(normalize_channel((argb & 0x0000FF00) >> 1*8)),
		blue (normalize_channel((argb & 0x000000FF) >> 0*8))
	{}

};

namespace Design {
	const Color BACKGROUND = Color(0xFF333333);
	const Color BACKGROUND_TRANSPARENT = Color(0.85f, 0x333333);
	const Color OUTLINE = Color(0xFFEEEEEE);
	const Color FILL = Color(0.15f, 0xEEEEEE);
}


#endif /* DESIGN_H_ */
