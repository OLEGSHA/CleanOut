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

#include "design.h"


GLfloat normalize_channel(unsigned int channel_0_thru_255) {
	return channel_0_thru_255 / static_cast<GLfloat>(0xFF);
}

inline GLfloat mix_channels(GLfloat a, GLfloat b, float param) {
	return (a * (1 - param)) + (b * param);
}

#include <iostream>

Color Gradient::get(float param) const {
	if (param < 0 || param > 1) {
		std::cerr << "wtf " << param << std::endl;
	}
	return Color(
			mix_channels(from.alpha, to.alpha, param),
			mix_channels(from.red, to.red, param),
			mix_channels(from.green, to.green, param),
			mix_channels(from.blue, to.blue, param)
	);
}

