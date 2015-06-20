/*
 * XIAG: XML Interpreter for Adventure Games
 * Copyright (C) 2008 Mathieu Schroeter <mathieu.schroeter@gamesover.ch>
 *
 * This file is part of XIAG.
 *
 * XIAG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XIAG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CHAPTER_H
#define CHAPTER_H

typedef struct chapter_s chapter_t;

#include "scene.h"

void chapter_free (chapter_t *chapter);
chapter_t *chapter_add (chapter_t *chapter, char *id);

scene_t *chapter_scenes_get (chapter_t *chapter);
chapter_t *chapter_search_fromid (chapter_t *chapter, char *id);
const char *chapter_id_get (chapter_t *chapter);
void chapter_scenes_set (chapter_t *chapter, scene_t *scenes);

#endif /* CHAPTER_H */
