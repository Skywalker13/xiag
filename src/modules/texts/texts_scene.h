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

#ifndef TEXTS_SCENE_H
#define TEXTS_SCENE_H

#include "texts_room.h"

typedef struct texts_scene_s texts_scene_t;

void texts_scene_free (texts_scene_t *scene);
texts_scene_t *texts_scene_add (texts_scene_t *scene,
                                char *id, char *name, texts_room_t *rooms);

texts_room_t *texts_scene_rooms_get (texts_scene_t *scene);
texts_scene_t *texts_scene_search_fromid (texts_scene_t *scene, char *id);

#endif /* TEXTS_SCENE_H */
