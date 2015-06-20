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

#ifndef SCENE_H
#define SCENE_H

typedef struct scene_s scene_t;

#include "chapter.h"
#include "room.h"

void scene_free (scene_t *scene);
scene_t *scene_add (scene_t *scene, char *id, chapter_t *parent);

room_t *scene_rooms_get (scene_t *scene);
scene_t *scene_search_fromid (scene_t *scene, char *id);
const char *scene_id_get (scene_t *scene);
void scene_rooms_set (scene_t *scene, room_t *rooms);
chapter_t *scene_parent_get (scene_t *scene);

#endif /* SCENE_H */
