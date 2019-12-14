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

#ifndef GLYPH_H_
#define GLYPH_H_

#include "../common.h"

#include <cstdarg>


typedef float GlyphCoord;
struct GlyphPoint {
	GlyphCoord x, y;

	operator ScreenPoint() const {
		return {static_cast<float>(x), static_cast<float>(y)};
	}
};

const GlyphCoord GLYPH_HEIGHT = 9;

/*
 * Primitives
 */
class Primitive {
public:
	virtual ~Primitive() {}
	virtual void render() = 0;
};

class LinePrimitive : public Primitive {
private:
	GlyphPoint start, end;
public:
	LinePrimitive(GlyphPoint start, GlyphPoint end) :
		start(start), end(end) {}

	void render() override;
};

class ArcPrimitive : public Primitive {
private:
	GlyphPoint center;
	GlyphCoord radius;
	float start_angle, end_angle;
public:
	ArcPrimitive(
			GlyphPoint center, GlyphCoord radius,
			float start_angle, float end_angle) :
		center(center), radius(radius),
		start_angle(start_angle), end_angle(end_angle) {}

	void render() override;
};

/*
 * Glyph
 */

class Glyph {
private:
	GlyphCoord width;

	Primitive* *primitives;
	size_t primitive_count;

public:
	Glyph(
			GlyphCoord width,
			size_t primitive_count, ...
	);

	~Glyph();

	GlyphCoord get_width() const {
		return width;
	}
	void render() const;
	bool is_whitespace() const;
};

/*
 * Glyphs
 */

class Glyphs {
private:
	const Glyph* fallback;
	const size_t max_char;
	const Glyph ** const contents;

	size_t get_index(char c) const;

public:
	Glyphs(char max_char, const Glyph* fallback);
	~Glyphs();

	const Glyph& get(char for_char) const;

	/*
	 * Associates the given glyph with the given character.
	 * Given pointer will be deleted with this Glyphs instance. Duplicate
	 * glyphs are not allowed.
	 */
	void put(char c, const Glyph* glyph);
};

Glyphs& get_default_glyphs();

void register_default_glyphs(Glyphs& output);


#endif /* GLYPH_H_ */
