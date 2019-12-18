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

#include "random.h"

#include <random>

std::mt19937 *generator;
std::uniform_real_distribution<> floats(0.0f, 1.0f);

void setup_random() {
	std::random_device random_device;
	generator = new std::mt19937(random_device());
}

void terminate_random() {
	delete generator;
}

float generate_random_float() {
	return floats(*generator);
}

