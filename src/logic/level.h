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

#ifndef LEVEL_H_
#define LEVEL_H_

#include "../common.h"

#include "bricks.h"


typedef unsigned int LevelId;

class Level {
private:
	LevelId id;

	LevelBlockCoord width, height;

	Brick** field;
	bool* corpses_field;
	LevelBlockCoord field_height;
	std::list<Brick*> bricks_to_delete = std::list<Brick*>();

	unsigned int bricks_to_destroy = 0;

	/*
	 * Checks whether the block _does_not_ represent a brick on the field.
	 */
	bool check_pos(LevelBlock) const;

	/*
	 * Computes the field 1D index for the given valid (see check_pos()) block.
	 */
	size_t get_field_index(LevelBlock) const;

	void render_corpse(LevelBlock) const;

public:
	Level(
			LevelId id,
			LevelBlockCoord width, LevelBlockCoord height,
			LevelBlockCoord field_height
	);
	~Level();

	void render(Game& game);

	LevelId get_id() {
		return id;
	}

	LevelBlockCoord get_width() const  { return width; }
	LevelBlockCoord get_height() const { return height; }

	Brick* get_brick(LevelBlock) const;
	void set_brick(LevelBlock, Brick*);
	void destroy_brick(LevelBlock);

	bool is_level_cleared() const;

	bool has_corpse(LevelBlock) const;

	/*
	 * Handles a single collision if necessary.
	 */
	void collide(Game&, LevelBlock, Ball&);

	/*
	 * Deletes (frees memory) of the bricks that have been semantically removed.
	 */
	void delete_pending_bricks();
};


#endif /* LEVEL_H_ */
